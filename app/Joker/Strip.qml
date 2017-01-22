import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

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
        x: -stripContainer.contentX
        anchors.top: parent.top
        anchors.bottom: parent.bottom

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

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: "#77000000"
        height: parent.height * parent.currentTrackNumber  / 4
        opacity: window.edition

        Behavior on opacity {
            NumberAnimation {
                easing.type: Easing.Linear
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: 100
                easing.type: Easing.Linear
            }
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height * (3 - parent.currentTrackNumber) / 4
        color: "#77000000"
        opacity: window.edition

        Behavior on opacity {
            NumberAnimation {
                easing.type: Easing.Linear
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: 100
                easing.type: Easing.Linear
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
                doc.addLine(stripContextMenu.time, textY);
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
        onWheel: {
            console.log("wheel " + wheel.angleDelta.x + " " + wheel.angleDelta.y + " " + wheel.pixelDelta.x + " " + wheel.pixelDelta.y);
            var wheelStepSize = 120;
            var frameChange = wheel.angleDelta.y/wheelStepSize;
            playbackController.onFrameScroll(frameChange);
        }
    }

    // ruler
    Row {
        id: stripRulerRepeater
        x: settings.firstFootTime/settings.horizontalTimePerPixel - stripContainer.contentX
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
        x: -stripContainer.contentX
        height: parent.height
        visible: settings.displayCuts

        Repeater {
            model: doc.cutModel
            delegate: Cut {
                x: time/settings.horizontalTimePerPixel
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }
        }
    }

    Item {
        x: -stripContainer.contentX
        height: parent.height

        Repeater {
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
        x: -stripContainer.contentX
        height: parent.height

        Repeater {
            model: doc.lineModel
            delegate: Line {}
        }

//        NumberAnimation {
//            id: animateOpacity
//            target: stripLineRepeater
//            properties: "x"
//            from: -87202560 / settings.horizontalTimePerPixel
//            to: -87202560 / settings.horizontalTimePerPixel - 4000
//            duration: 10000
//            loops: Animation.Infinite
//            running: true
//       }
    }

    // sync bar
    Rectangle {
        x: delayX
        y: 0
        width: 4
        height: parent.height
        color: "#AAFF566C"
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

    function editTextAt(x, y) {
        for (var i = 0; i < stripLineRepeater.children.length; ++i) {
            var line = stripLineRepeater.children[i];
            if (line.objectName !== "Line") {
                continue;
            }
            var success = line.editTextAt(x, y);
            if (success) {
                return true;
            }
        }
        return false;
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
        for (var i = 0; i < stripLineRepeater.children.length; ++i) {
            var line = stripLineRepeater.children[i];
            if (line.objectName !== "Line") {
                continue;
            }
            var lineSuccess = line.isAt(x, y);
            if (lineSuccess) {
                var success = line.deleteDetectAt(x, y);
                if (success) {
                    return true;
                }

                console.log("deleting line " + i)
                doc.lineModel.remove(i)
                return true;
            }
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
}
