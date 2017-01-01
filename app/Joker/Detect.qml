import QtQuick 2.0
import QtQuick.Controls 1.1
import "qrc:/qml/colors.js" as Colors

Item {
    id: lineDetect
    Binding { target: model; property: "time"; value: x*settings.horizontalTimePerPixel }
    property int modelType: type

    Rectangle {
        id: detectRectangle
        color: Qt.lighter(Colors.colorFromDetectType(type))
        width: 18
        height: 18
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        visible: window.edition

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
                    if (mouse.modifiers & Qt.ControlModifier) {
                        // snap x to whole frame
                        console.log("detect dragged with ctrl " + movement)
                        lineDetect.x = lineDetect.x + snapToFrame(movement)
                    }
                    else {
                        console.log("detect dragged without ctrl " + movement)
                    }
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
