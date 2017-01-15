import QtQuick 2.0
import QtQuick.Controls 1.1
import "qrc:/qml/colors.js" as Colors
import "qrc:/qml/symbols.js" as Symbols
import "qrc:/fonts/fontawesome.js" as FontAwesome
import PhImport 1.0

Item {
    id: lineDetect
    Binding { target: model; property: "time"; value: x*settings.horizontalTimePerPixel }
    property int modelType: type

    // Load the "FontAwesome" font for the detection icons.
    FontLoader {
        source: "qrc:/fonts/fontawesome-webfont.ttf"
    }

    Rectangle {
        id: detectRectangle
        color: Colors.colorFromDetectType(type)
        opacity: 0.8
        width: 18
        height: 18
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        visible: window.edition

        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "black"
            styleColor: "black"
            font.bold: true
            font.pixelSize: parent.width - 2
            font.family: "FontAwesome"
            text: Symbols.symbolFromDetectType(type)
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.bottom
            height: 2
            color: "crimson"
        }

        Drag.keys: "Ctrl"

        MouseArea {
            id: detectMouseArea
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            drag{
                target: parent
                axis: Drag.XAxis
                smoothed: true
            }

            onDoubleClicked: {
                attach()
            }

            property int startX: 0

            onPressed: {
                startX  = mouse.x
            }

            onPositionChanged: {
                if (drag.active) {
                    var movement = mouseX - startX;
                    // snap x to whole frame
                    console.log("detect dragged " + movement)
                    lineDetect.x = lineDetect.x + snapToFrame(movement)
                }
            }
        }

        MouseArea {
            id: rightPressArea
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            enabled: window.edition
            onClicked: {
                console.log("index: " + model.index);
                detectContextMenu.index = model.index;
                detectContextMenu.popup();
            }
        }
    }

    Menu {
        id: detectContextMenu
        title: "Edit"
        property int index: 0
        MenuItem {
            text: FontAwesome.Icon.link + " Attach detect"
            onTriggered: {
                console.log("Attach " + detectContextMenu.index);
                attach()
            }
        }
        MenuItem {
            text: "Delete detect"
            onTriggered: {
                console.log("Detect " + detectContextMenu.index);
                stripLineContainer.lineModel.unlinkedDetects.remove(detectContextMenu.index);
            }
        }
    }

    function attach() {
        // attach this detect
        console.log("attach " + index + " " + x)
        stripLineContainer.attachDetect(index, x)
    }
}
