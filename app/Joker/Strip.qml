import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1
import PhImport 1.0

Item {
    id: stripContainer

    property int delayX: window.edition ? width / 2 : width / 6
    property int contentX: jokerWindow.stripTime / settings.horizontalTimePerPixel - delayX
    property bool editing: false
    property int currentTrackNumber: 0
    property double currentTextY: height * currentTrackNumber / 4

    Behavior on delayX {
        NumberAnimation {
            easing.type: Easing.InOutCubic
        }
    }

    // the font name is passed from here
    FontLoader { id: stripFont; name: settings.textFontFamily }

    // background image
    // TODO: inverted colors, solid color if background is disabled
    Item {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        x: -stripContainer.contentX

        Image {
            source: stripBackgroundUrl
            fillMode: Image.TileHorizontally
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            x: Math.min(videoEngine.timeIn, doc.timeIn) / settings.horizontalTimePerPixel
            width: Math.max(videoEngine.timeOut, doc.timeOut) / settings.horizontalTimePerPixel - x

            onXChanged: console.log(videoEngine.timeIn + " " + doc.timeIn)
            onWidthChanged: console.log(videoEngine.timeOut + " " + doc.timeOut)
        }
    }

    EditionMask {
        anchors.fill: parent
        currentTrackNumber: parent.currentTrackNumber
    }

    Item {
        id: c
        x: -stripContainer.contentX
        anchors.top: parent.top
        anchors.bottom: parent.bottom

//        NumberAnimation {
//            id: xAnimation
//            target: c
//            properties: "x"
//            from: -86400000 / settings.horizontalTimePerPixel
//            to: -86400000 / settings.horizontalTimePerPixel -1000*10
//            duration: 2000*10
//            loops: Animation.Infinite
//            running: true
//       }

        // ruler
        Row {
            id: stripRulerRepeater
            x: settings.firstFootTime/settings.horizontalTimePerPixel //- stripContainer.contentX
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            visible: settings.displayFeet

            Repeater {
                // try to avoid updating the model when it is not visible
                model: settings.displayFeet * (doc.timeOut - settings.firstFootTime) / settings.timeBetweenTwoFeet
                delegate: RulerTick {
                    width: settings.timeBetweenTwoFeet/settings.horizontalTimePerPixel
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }
            }
        }

        Item {
            height: parent.height
            visible: settings.displayCuts

            Repeater {
                id: stripCutRepeater
                model: doc.cutModel
                delegate: Cut {
                    x: time/settings.horizontalTimePerPixel
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }
            }
        }

        Item {
            height: parent.height

            Repeater {
                id: stripLoopRepeater
                model: doc.loopModel
                delegate: Loop {
                    x: time/settings.horizontalTimePerPixel
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }
            }
        }

        Item {
            id: stripLineRepeater
            height: parent.height

            Repeater {
                model: jokerWindow.filteredLineModel
                delegate: Line { }
            }
        }
    }

    Menu {
        id: stripContextMenu
        title: "Edit"
        property int mouseX: 0
        property int mouseY: 0
        property int time: (mouseX - stripContainer.delayX) * settings.horizontalTimePerPixel + jokerWindow.stripTime
        MenuItem {
            text: "Add phrase"
            shortcut: "Return"
            onTriggered: {
                var trackHeight = stripContainer.height / 4;
                var textY = Math.round((stripContextMenu.mouseY - trackHeight / 2) / stripContainer.height * 4) / 4;
                console.log("add line " + stripContextMenu.time + " " + textY);
                doc.addLine(stripContextMenu.time, textY, PhStripDetect.MouthOpen);
            }
        }
        MenuItem {
            text: "Add loop"
            shortcut: "0"
            onTriggered: {
                console.log("add loop " + stripContextMenu.time);
                doc.loopModel.add(stripContextMenu.time)
            }
        }
        MenuItem {
            text: "Add cut"
            shortcut: "1"
            onTriggered: {
                console.log("add cut " + stripContextMenu.time);
                doc.cutModel.add(stripContextMenu.time)
            }
        }
    }

    MouseArea {
        id: stripArea
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        enabled: window.edition
        onClicked: {
            console.log("click " + mouse.x + " " + mouse.y);
            stripContextMenu.mouseX = mouse.x;
            stripContextMenu.mouseY = mouse.y;
            stripContextMenu.popup()
        }
    }

    MouseArea {
        id: scrollStripArea
        anchors.fill: parent
        propagateComposedEvents: true

        onWheel: {
            console.log("wheel " + wheel.angleDelta.x + " " + wheel.angleDelta.y + " " + wheel.pixelDelta.x + " " + wheel.pixelDelta.y);
            var wheelStepSize = 120;
            var frameChange = wheel.angleDelta.y/wheelStepSize;
            playbackController.onFrameScroll(frameChange);
        }

        // pass other events to the items below in the z-stack
        onClicked: mouse.accepted = false;
        onPressed: mouse.accepted = false;
        onReleased: mouse.accepted = false;
        onDoubleClicked: mouse.accepted = false;
        onPositionChanged: mouse.accepted = false;
        onPressAndHold: mouse.accepted = false;
    }

    // sync bar
    Rectangle {
        x: delayX - width/2
        y: 0
        width: 4
        height: parent.height
        color: window.edition ? "#77FF566C" : "#AAFF566C"
    }

    function initDrag() {
        console.debug("init drag");
        var component = Qt.createComponent("SnapDragTarget.qml");
        var object = component.createObject(stripContainer);
        object.lineHeight = height/4
        return object
    }

    function finishDrag(object) {
        console.log("finish drag");
        if (object) {
            object.destroy();
            object = undefined
        }
    }

    function snapToFrame(pixelChange) {
        var pixelPerFrame = jokerWindow.timePerFrame / settings.horizontalTimePerPixel;
        var timeChange = pixelChange * settings.horizontalTimePerPixel;
        // round to frame
        var frameChange = Math.round(timeChange / jokerWindow.timePerFrame);
        timeChange = frameChange * jokerWindow.timePerFrame;
        pixelChange = timeChange / settings.horizontalTimePerPixel;
        return pixelChange;
    }

    function findLineAt(x, y) {
        for (var i = 0; i < stripLineRepeater.children.length; ++i) {
            var line = stripLineRepeater.children[i];
            if (line.objectName !== "Line") {
                continue;
            }

            var lineX = x - line.x
            var lineY = y - line.y

            if (lineY === 0 && lineX >= -0.1 && lineX <= line.width + 0.1) {
                return line
            }

            // empty line
            if (lineY === 0 && lineX >= -0.1 && line.empty) {
                return line
            }
        }
        return 0
    }

    function findLoopAt(x) {
        for (var i = 0; i < stripLoopRepeater.count; ++i) {
            var loop = stripLoopRepeater.itemAt(i);
            var loopX = x - loop.x
            console.log("loop " + loopX)

            if (loopX >= -0.1 && loopX <= 0.1) {
                return loop
            }
        }
        return 0
    }

    function findCutAt(x) {
        for (var i = 0; i < stripCutRepeater.count; ++i) {
            var cut = stripCutRepeater.itemAt(i);
            var cutX = x - cut.x
            console.log("cut " + cutX)

            if (cutX >= -0.1 && cutX <= 0.1) {
                return cut
            }
        }
        return 0
    }

    function addDetectAt(x, y, type) {
        for (var i = 0; i < stripLineRepeater.children.length; ++i) {
            var line = stripLineRepeater.children[i];
            if (line.objectName !== "Line") {
                continue;
            }
            var success = line.addDetectAt(x, y, type);
            if (success) {
                return true;
            }
        }
        return false;
    }

    function moveDetectAt(x, y, frameChange) {
        for (var i = 0; i < stripLineRepeater.children.length; ++i) {
            var line = stripLineRepeater.children[i];
            if (line.objectName !== "Line") {
                continue;
            }
            var success = line.moveDetectAt(x, y, frameChange);
            if (success) {
                return true;
            }
        }

        return false;
    }

    function deleteAt(x, y) {
        // find loop
        var loop = findLoopAt(x)
        if (loop) {
            loop.remove();
            return true;
        }

        // find cut
        var cut = findCutAt(x)
        if (cut) {
            cut.remove();
            return true;
        }

        // find line
        var line = findLineAt(x, y);
        console.log(line);
        if (line) {
            if (line.deleteDetectAt(x, y)) {
                return true;
            }

            console.log("deleting line")
            line.remove()
            return true;
        }

        return false;
    }

    function shiftDetectAt(x, y, shift) {
        for (var i = 0; i < stripLineRepeater.children.length; ++i) {
            var line = stripLineRepeater.children[i];
            if (line.objectName !== "Line") {
                continue;
            }
            var success = line.shiftDetectAt(x, y, shift);
            if (success) {
                return;
            }
        }
    }

    function toggleAttachDetectAt(x, y) {
        for (var i = 0; i < stripLineRepeater.children.length; ++i) {
            var line = stripLineRepeater.children[i];
            if (line.objectName !== "Line") {
                continue;
            }
            var success = line.toggleAttachDetectAt(x, y);
            if (success) {
                return;
            }
        }
    }

    function addOrEditOrClose(textX, textY, type) {
        var line = findLineAt(textX, textY);
        console.log(line);

        if (line) {
            if (line.empty) {
                // close
                line.close(textX, type)
            }
            else {
                line.editTextAt(textX)
            }
        }
        else {
            var time = textX * settings.horizontalTimePerPixel
            console.log("Add line " + time + " " + (strip.currentTrackNumber / 4));
            doc.addLine(time, strip.currentTrackNumber / 4, type);
        }
    }
}
