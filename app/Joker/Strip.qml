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
    FontLoader { id: stripFont; source: textFontUrl }

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
        MenuItem {
            text: "Add phrase"
            onTriggered: {
                var time = (stripContextMenu.mouseX - delayX) * settings.horizontalTimePerPixel + jokerWindow.stripTime;
                var trackHeight = stripContainer.height / 4;
                var textY = Math.round((stripContextMenu.mouseY - trackHeight / 2) / stripContainer.height * 4) / 4;
                console.log("add line " + time + " " + textY);
                doc.lineModel.addDetect(time, textY);
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

    // ruler
    Row {
        id: stripRulerRepeater
        x: settings.firstFootTime/settings.horizontalTimePerPixel - stripContainer.contentX
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: settings.displayFeet

        Repeater {
            model: (doc.timeOut - settings.firstFootTime) / settings.timeBetweenTwoFeet
            delegate: Item {
                width: settings.timeBetweenTwoFeet/settings.horizontalTimePerPixel
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                property int rulerIndex: index

                Rectangle {
                    color: settings.invertColor? "white":"#808080"
                    anchors.top: parent.top
                    width: 1000/settings.horizontalTimePerPixel
                    x: -width/2
                    height: parent.height/2
                }

                Rectangle {
                    color: settings.invertColor? "white":"#808080"
                    anchors.top: parent.top
                    width: 1000/settings.horizontalTimePerPixel
                    x: -width/2 + parent.width/2
                    height: parent.height/2
                }

                // ruler disc
                Rectangle {
                    width: 4000/settings.horizontalTimePerPixel
                    height: width
                    color: settings.invertColor? "white":"#808080"
                    radius: width*0.5
                    x: parent.width/2 - width/2
                    y: parent.height/2 + width/4
                }

                Text {
                    color: settings.invertColor? "white":"#808080"
                    x: -width/2
                    y: parent.height/2
                    text: parent.rulerIndex
                    font.pixelSize: parent.height/3
                }
            }
        }
    }

    Component {
        id: offDetectDelegate
        Item {
            width: duration/settings.horizontalTimePerPixel
            height: parent.height

            Canvas {
               id: offCanvas
               anchors.fill: parent
               antialiasing: true
               visible: content.length===0

               onPaint: paintCanvas();

               function paintCanvas() {
                   var ctx = offCanvas.getContext("2d");
                   ctx.save();
                   // color should be the same as the text
                   ctx.fillStyle = "black";
                   ctx.fillRect(0, arrowUpCanvas.height*9/10, arrowUpCanvas.width, arrowUpCanvas.height/10);
                   ctx.restore();
                }
            }
        }
    }

    Component {
        id: semiOffDetectDelegate
        Item {
            width: duration/settings.horizontalTimePerPixel
            height: parent.height

            Canvas {
               id: semiOffCanvas
               anchors.fill: parent
               antialiasing: true
               visible: content.length===0

               onPaint: paintCanvas();

               function paintCanvas() {
                   var ctx = semiOffCanvas.getContext("2d");
                   ctx.save();
                   // color should be the same as the text
                   ctx.fillStyle = "grey";
                   ctx.fillRect(0, arrowUpCanvas.height*9/10, arrowUpCanvas.width, arrowUpCanvas.height/10);
                   ctx.restore();
                }
            }
        }
    }

    Component {
        id: arrowUpDetectDelegate
        Item {
            width: duration/settings.horizontalTimePerPixel
            height: parent.height

            Canvas {
               id: arrowUpCanvas
               anchors.fill: parent
               antialiasing: true
               visible: content.length===0

               onPaint: paintCanvas();

               function paintCanvas() {
                   var ctx = arrowUpCanvas.getContext("2d");
                   ctx.save();

                   // color should be the same as the text
                   ctx.fillStyle = "black";

                   var thickness = arrowUpCanvas.height/10;
                   var nose = arrowUpCanvas.height/3;

                   ctx.beginPath();
                   ctx.moveTo(0, thickness);
                   ctx.lineTo(thickness, 0);
                   ctx.lineTo(arrowUpCanvas.width, arrowUpCanvas.height - thickness);
                   ctx.lineTo(arrowUpCanvas.width - thickness, arrowUpCanvas.height);
                   ctx.closePath();
                   ctx.fill();

                   ctx.beginPath();
                   ctx.moveTo(arrowUpCanvas.width, arrowUpCanvas.height);
                   ctx.lineTo(arrowUpCanvas.width - nose, arrowUpCanvas.height);
                   ctx.lineTo(arrowUpCanvas.width, arrowUpCanvas.height - nose);
                   ctx.closePath();
                   ctx.fill();

                   ctx.restore();
                }
            }
        }
    }

    Component {
        id: arrowDownDetectDelegate
        Item {
            width: duration/settings.horizontalTimePerPixel
            height: parent.height

            Canvas {
               id: arrowDownCanvas
               anchors.fill: parent
               antialiasing: true
               visible: content.length===0

               onPaint: paintCanvas();

               function paintCanvas() {
                   var ctx = arrowDownCanvas.getContext("2d");

                   // color should be the same as the text
                   ctx.fillStyle = "black";

                   var thickness = arrowDownCanvas.height/10;
                   var nose = arrowDownCanvas.height/3;

                   ctx.beginPath();
                   ctx.moveTo(arrowDownCanvas.width - thickness, arrowDownCanvas.height);
                   ctx.lineTo(0, arrowDownCanvas.height - thickness);
                   ctx.lineTo(thickness, arrowDownCanvas.height);
                   ctx.lineTo(arrowDownCanvas.width, thickness);

                   ctx.closePath();
                   ctx.fill();

                   ctx.beginPath();
                   ctx.moveTo(arrowDownCanvas.width, 0);
                   ctx.lineTo(arrowDownCanvas.width - nose, 0);
                   ctx.lineTo(arrowDownCanvas.width, nose);
                   ctx.closePath();
                   ctx.fill();

                   ctx.restore();
                }
            }
        }
    }

    Item {
        x: -stripContainer.contentX
        height: parent.height
        visible: settings.displayCuts

        Repeater {
            model: doc.cutModel
            delegate: Item {
                id: cutDelegate
                x: time/settings.horizontalTimePerPixel
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                Rectangle {
                    color: settings.invertColor? "AAFFFFFF":"#AA000000"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: settings.cutWidth

                    MouseArea {
                        id: rightPressArea
                        enabled: window.edition
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onClicked: cutDelegate.showContextMenu()
                    }
                }

                Menu {
                    id: cutContextMenu
                    title: "Edit"
                    property int index: 0
                    MenuItem {
                        text: "Delete cut"
                        onTriggered: {
                            console.log("Cut " + cutContextMenu.index);
                            doc.cutModel.remove(cutContextMenu.index);
                        }
                    }
                }

                function showContextMenu() {
                    console.log("index: " + model.index);
                    cutContextMenu.index = model.index;
                    cutContextMenu.popup();
                }
            }
        }
    }

    Item {
        x: -stripContainer.contentX
        height: parent.height

        Repeater {
            model: doc.loopModel
            delegate: Item {
                id: loopDelegate
                x: time/settings.horizontalTimePerPixel
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                Rectangle {
                    color: settings.invertColor? "white":"black"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    x: -width/2
                    width: parent.height / 40

                    MouseArea {
                        enabled: window.edition
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onClicked: loopDelegate.showContextMenu()
                    }
                }

                Rectangle {
                    color: settings.invertColor? "white":"black"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: parent.height/3
                    anchors.bottomMargin: parent.height/3
                    x: -width/2
                    width: parent.height / 40
                    rotation: 45

                    MouseArea {
                        enabled: window.edition
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onClicked: loopDelegate.showContextMenu()
                    }
                }

                Rectangle {
                    color: settings.invertColor? "white":"black"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: parent.height/3
                    anchors.bottomMargin: parent.height/3
                    x: -width/2
                    width: parent.height / 40
                    rotation: -45

                    MouseArea {
                        enabled: window.edition
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onClicked: loopDelegate.showContextMenu()
                    }
                }

                Text {
                    text: label
                    color: "gray"
                    font.pixelSize: parent.height/4
                    font.family: "Arial"
                    x: 10
                    y: parent.height * 2 / 3

                    MouseArea {
                        enabled: window.edition
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onClicked: loopDelegate.showContextMenu()
                    }
                }

                Menu {
                    id: loopContextMenu
                    title: "Edit"
                    property int index: 0
                    MenuItem {
                        text: "Delete loop"
                        onTriggered: {
                            console.log("Loop " + loopContextMenu.index);
                            doc.loopModel.remove(loopContextMenu.index);
                        }
                    }
                }

                function showContextMenu() {
                    console.log("index: " + model.index);
                    loopContextMenu.index = model.index;
                    loopContextMenu.popup();
                }
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

    function addDetectAt(x, y) {
        for (var i = 0; i < stripLineRepeater.children.length; ++i) {
            var line = stripLineRepeater.children[i];
            if (line.objectName !== "Line") {
                continue;
            }
            var success = line.addDetectAt(x, y);
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
}
