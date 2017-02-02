import QtQuick 2.0
import QtQuick.Controls 1.1
import "qrc:/qml/colors.js" as Colors
import "qrc:/qml/symbols.js" as Symbols
import "qrc:/fonts/fontawesome.js" as FontAwesome
import PhImport 1.0 as Ph

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
        anchors.verticalCenter: parent.bottom
        anchors.verticalCenterOffset: -4
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
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.top
            height: 2
            width: 0
            color: "crimson"

            NumberAnimation on width {
                id: createAnimation
                from: 0
                to: detectRectangle.width
                duration: 250
            }

            Component.onCompleted: createAnimation.start()
        }

        MouseArea {
            id: detectMouseArea
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton

            property int startX: 0
            property var mouseDragTarget

            onPressed: {
                startX = lineDetect.x
                mouseDragTarget = stripContainer.initDrag()
            }

            onReleased: {
                stripContainer.finishDrag(mouseDragTarget)
            }

            drag{
                target: detectMouseArea.mouseDragTarget
                axis: Drag.XAxis
                smoothed: true

                onActiveChanged: {
                    if (detectMouseArea.drag.active) {
                        console.log("active")
                        detectMouseArea.mouseDragTarget.onDragged.connect(onDragged)
                    }
                }
            }

            function onDragged(dragX, dragY) {
                console.log(dragX)
                setTime((startX + dragX) * settings.horizontalTimePerPixel)
            }

            onDoubleClicked: {
                attach()
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
        Menu {
            title: "Change type"

            MenuItem {
                text: "Labial"
                onTriggered: {
                    model.type = Ph.PhStripDetect.Labial
                }
            }
            MenuItem {
                text: "Dental"
                onTriggered: {
                    model.type = Ph.PhStripDetect.Dental
                }
            }
            MenuItem {
                text: "Neutral"
                onTriggered: {
                    model.type = Ph.PhStripDetect.Unknown
                }
            }
            MenuItem {
                text: "Aperture"
                onTriggered: {
                    model.type = Ph.PhStripDetect.Aperture
                }
            }
            MenuItem {
                text: "Bowl"
                onTriggered: {
                    model.type = Ph.PhStripDetect.Bowl
                }
            }
            MenuItem {
                text: "Advance"
                onTriggered: {
                    model.type = Ph.PhStripDetect.Advance
                }
            }
        }
    }

    function attach() {
        // attach this detect
        console.log("attach " + index + " " + x)
        stripLineContainer.attachDetect(index, x)
    }

    function setTime(time) {
        model.time = time
    }
}
