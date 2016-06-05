import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import "fontawesome.js" as FontAwesome

Rectangle {
    id: mediaPanelContainer
    height: panelRow.height + 2
    color: "#aa0d0d0d"
    radius: 0
    border.width: 0
    border.color: "#aa6d7dff"

    property color controlColor: "white"
    property int relativeTime: timeSlider.relativeTime

    // Load the "FontAwesome" font for the monochrome icons.
    FontLoader {
        source: "fonts/fontawesome-webfont.ttf"
    }

    RowLayout {
        id: panelRow

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10

        height: 40

        property int pixelSize: 20

        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            color: mediaPanelContainer.controlColor
            styleColor: mediaPanelContainer.controlColor
            font.bold: true
            font.pixelSize: parent.pixelSize
            font.family: "FontAwesome"
            // FIXME this is not really "stop!"
            text: FontAwesome.Icon.stop
            MouseArea {
                anchors.fill: parent
                onClicked: playbackController.onBack()
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            color: mediaPanelContainer.controlColor
            styleColor: mediaPanelContainer.controlColor
            font.bold: true
            font.pixelSize: parent.pixelSize
            font.family: "FontAwesome"
            text: FontAwesome.Icon.fast_backward
            MouseArea {
                anchors.fill: parent
                onClicked: playbackController.onRewind()
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            color: mediaPanelContainer.controlColor
            styleColor: mediaPanelContainer.controlColor
            font.bold: true
            font.pixelSize: parent.pixelSize
            font.family: "FontAwesome"
            text: FontAwesome.Icon.step_backward
            MouseArea {
                anchors.fill: parent
                onClicked: playbackController.onPreviousFrame()
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            color: mediaPanelContainer.controlColor
            styleColor: mediaPanelContainer.controlColor
            font.bold: true
            font.pixelSize: parent.pixelSize
            font.family: "FontAwesome"
            text: playbackController.rate === 0 ? FontAwesome.Icon.play : FontAwesome.Icon.pause
            MouseArea {
                anchors.fill: parent
                onClicked: playbackController.onPlayPause()
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            color: mediaPanelContainer.controlColor
            styleColor: mediaPanelContainer.controlColor
            font.bold: true
            font.pixelSize: parent.pixelSize
            font.family: "FontAwesome"
            text: FontAwesome.Icon.step_forward
            MouseArea {
                anchors.fill: parent
                onClicked: playbackController.onNextFrame()
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            color: mediaPanelContainer.controlColor
            styleColor: mediaPanelContainer.controlColor
            font.bold: true
            font.pixelSize: parent.pixelSize
            font.family: "FontAwesome"
            text: FontAwesome.Icon.fast_forward
            MouseArea {
                anchors.fill: parent
                onClicked: playbackController.onFastForward()
            }
        }

        Text {
            id: mediaPanelTcLabel
            anchors.verticalCenter: parent.verticalCenter
            text: tcLabelText
            color: mediaPanelContainer.controlColor
            styleColor: mediaPanelContainer.controlColor
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            font.pixelSize: parent.pixelSize
        }

        Slider {
            id: timeSlider
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            Layout.fillWidth: true
            minimumValue: 0.0
            maximumValue: playbackController.length*1.0
            stepSize: 1.0
            value: playbackController.relativeTime

            onValueChanged: console.log(value + " " + playbackController.length + " " + playbackController.relativeTime)

            style: SliderStyle {
                groove: Item {
                    implicitWidth: 200
                    implicitHeight: 2

                    Rectangle {
                        anchors.fill: parent
                        color: "gray"
                    }

                    Rectangle {
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.top: parent.top
                        width: styleData.handlePosition
                        color: "blue"
                    }
                }
                handle: Rectangle {
                    anchors.centerIn: parent
                    color: "blue"
                    border.width: 0
                    width: control.pressed ? 18 : 12
                    height: width
                    radius: width*0.5
                }
            }

            // setup QML->C++ binding
            Binding {
                target: playbackController
                property: "relativeTime"
                value: timeSlider.value
            }
        }

        Text {
            id: mediaPanelRateLabel
            anchors.verticalCenter: parent.verticalCenter
            color: mediaPanelContainer.controlColor
            styleColor: mediaPanelContainer.controlColor
            text: "x" + playbackController.rate
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            font.pixelSize: parent.pixelSize
        }

        Text {
            id: mediaPanelFpsLabel
            anchors.verticalCenter: parent.verticalCenter
            text: playbackController.averageFps + " fps"
            color: mediaPanelContainer.controlColor
            styleColor: mediaPanelContainer.controlColor
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            font.pixelSize: parent.pixelSize
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            videoOverlay.showPanelPermanent()
        }
        onExited: {
            // this will hide the panel after some time
            videoOverlay.showPanel()
        }

        // pass other events to the items below in the z-stack
        onClicked: mouse.accepted = false;
        onPressed: mouse.accepted = false;
        onReleased: mouse.accepted = false;
        onDoubleClicked: mouse.accepted = false;
        onPositionChanged: mouse.accepted = false;
        onPressAndHold: mouse.accepted = false;
    }
}
