import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

Item {
    id: stripContainer
    width: 600
    height: 400

    property int delayX: width / 6
    property int contentX: jokerWindow.stripTime / horizontalTimePerPixel - delayX
    property bool editing: false

    // the font name is passed from here
    FontLoader { id: stripFont; source: textFontUrl }

    // background image
    // TODO: inverted colors, solid color if background is disabled
    Image {
        id: stripBackground
        objectName: "stripBackground"
        source: stripBackgroundUrl
        fillMode: Image.TileHorizontally
        anchors.top: parent.top
        x: 0
        width: 2*parent.width
        anchors.bottom: parent.bottom
        transform: Translate { x: (-jokerWindow.stripTime / horizontalTimePerPixel - stripBackground.width/2 / 6) % (stripBackground.width/2) }
    }

    Menu {
        id: stripContextMenu
        title: "Edit"
        property int mouseX: 0
        property int mouseY: 0
        MenuItem {
            text: "Add phrase"
            onTriggered: {
                var time = (stripContextMenu.mouseX - delayX) * horizontalTimePerPixel + jokerWindow.stripTime;
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
        onClicked: {
            console.log("click " + mouse.x + " " + mouse.y);
            stripContextMenu.mouseX = mouse.x;
            stripContextMenu.mouseY = mouse.y;
            stripContextMenu.popup()
        }
    }

    // ruler
    ListView {
        anchors.fill: parent
        orientation: ListView.Horizontal
        contentX: stripContainer.contentX
        interactive: false
        model: rulerModel
        visible: displayRuler
        cacheBuffer: 2*stripContainer.width

        delegate: Item {
            width: duration/horizontalTimePerPixel
            height: parent.height

            Rectangle {
                color: invertColor? "white":"#808080"
                anchors.top: parent.top
                width: 1000/horizontalTimePerPixel
                x: -width/2
                height: parent.height/2
            }

            Rectangle {
                color: invertColor? "white":"#808080"
                anchors.top: parent.top
                width: 1000/horizontalTimePerPixel
                x: -width/2 + parent.width/2
                height: parent.height/2
            }

            // ruler disc
            Rectangle {
                width: 4000/horizontalTimePerPixel
                height: width
                color: invertColor? "white":"#808080"
                radius: width*0.5
                x: parent.width/2 - width/2
                y: parent.height/2 + width/4
            }

            Text {
                color: invertColor? "white":"#808080"
                x: -width/2
                y: parent.height/2
                text: name
                font.pixelSize: parent.height/3
            }
        }
    }

    // FIXME color, font, inverted color are not implemented
    Component {
        id: stripTextDelegate
        Item {
            id: stripTextContainer
            width: (timeOut - timeIn)/horizontalTimePerPixel
            height: parent.height

            Rectangle {
                border.width: 1
                border.color: "#30000000"
                color: "#00FFFFFF"
                anchors.fill: parent
                visible: content.length > 0
            }

            TextInput {
                id: stripTextItem
                text: model.content
                font.pixelSize: parent.height
                font.family: stripFont.name
                font.weight: textBoldness * 99/5
                transform: Scale {  xScale: stripTextContainer.width/stripTextItem.width;
                                    yScale: 1;}
                smooth: true // smooth scaling
//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: {
//                        stripTextItem.color = Qt.rgba(Math.random(), Math.random(), Math.random(), 1);
//                    }
//                }
            }

            Binding { target: model; property: "content"; value: stripTextItem.text }
        }
    }

    Component {
        id: offDetectDelegate
        Item {
            width: duration/horizontalTimePerPixel
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
            width: duration/horizontalTimePerPixel
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
            width: duration/horizontalTimePerPixel
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
            width: duration/horizontalTimePerPixel
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

    Component {
        id: trackDelegate
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                width: parent.width
                height: parent.height
                orientation: ListView.Horizontal
                contentX: stripContainer.contentX
                interactive: false
                model: offDetect
                delegate: offDetectDelegate
                cacheBuffer: 2*stripContainer.width
            }

            ListView {
                width: parent.width
                height: parent.height
                orientation: ListView.Horizontal
                contentX: stripContainer.contentX
                interactive: false
                model: semiOffDetect
                delegate: semiOffDetectDelegate
                cacheBuffer: 2*stripContainer.width
            }

            ListView {
                width: parent.width
                height: parent.height
                orientation: ListView.Horizontal
                contentX: stripContainer.contentX
                interactive: false
                model: arrowUpDetect
                delegate: arrowUpDetectDelegate
                cacheBuffer: 2*stripContainer.width
            }

            ListView {
                width: parent.width
                height: parent.height
                orientation: ListView.Horizontal
                contentX: stripContainer.contentX
                interactive: false
                model: arrowDownDetect
                delegate: arrowDownDetectDelegate
                cacheBuffer: 2*stripContainer.width
            }
        }
    }

    Item {
        x: -stripContainer.contentX
        height: parent.height
        visible: displayCuts

        Repeater {
            model: doc.cutModel
            delegate: Item {
                id: cutDelegate
                x: time/horizontalTimePerPixel
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                Rectangle {
                    color: invertColor? "AAFFFFFF":"#AA000000"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: cutWidth

                    MouseArea {
                        id: rightPressArea
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
                x: time/horizontalTimePerPixel
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                Rectangle {
                    color: invertColor? "white":"black"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    x: -width/2
                    width: parent.height / 40

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onClicked: loopDelegate.showContextMenu()
                    }
                }

                Rectangle {
                    color: invertColor? "white":"black"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: parent.height/3
                    anchors.bottomMargin: parent.height/3
                    x: -width/2
                    width: parent.height / 40
                    rotation: 45

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onClicked: loopDelegate.showContextMenu()
                    }
                }

                Rectangle {
                    color: invertColor? "white":"black"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: parent.height/3
                    anchors.bottomMargin: parent.height/3
                    x: -width/2
                    width: parent.height / 40
                    rotation: -45

                    MouseArea {
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
    }

    // sync bar
    Rectangle {
        x: parent.width/6
        y: 0
        width: 4
        height: parent.height
        color: "#AAFF566C"
    }

    function snapToFrame(pixelChange) {
        var pixelPerFrame = jokerWindow.timePerFrame / horizontalTimePerPixel;
        var timeChange = pixelChange * horizontalTimePerPixel;
        // round to frame
        var frameChange = Math.round(timeChange / jokerWindow.timePerFrame);
        timeChange = frameChange * jokerWindow.timePerFrame;
        pixelChange = timeChange / horizontalTimePerPixel;
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
}
