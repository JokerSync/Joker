import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1

Rectangle {
    height: panelRow.height + 2
    color: "#aa0d0d0d"
    radius: 0
    border.width: 0
    border.color: "#aa6d7dff"

    RowLayout {
        id: panelRow

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right

        height: 20

        Text {
            id: mediaPanelTcLabel
            y: 8
            anchors.verticalCenter: parent.verticalCenter
            color: "#ffffff"
            text: tcLabelText
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            styleColor: "#ffffff"
            font.pixelSize: 12
        }

        Slider {
            id: timeSlider
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            Layout.fillWidth: true
            value: playbackController.relativeTime

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
            Binding { target: playbackController; property: "relativeTime"; value: timeSlider.value }
        }

        Button {
            objectName: "BackButton"
            text: "Back"
            onClicked: {
                playbackController.onBack()
            }
        }

        Button {
            objectName: "FastRewindButton"
            text: "FR"
            onClicked: {
                playbackController.onRewind()
            }
        }

        Button {
            objectName: "PreviousFrameButton"
            text: "<<"
            onClicked: {
                playbackController.onPreviousFrame()
            }
        }

        Button {
            objectName: "PlayButton"
            text: "Play/Pause"
            onClicked: {
                playbackController.onPlayPause()
            }
        }

        Button {
            objectName: "NextFrameButton"
            text: ">>"
            onClicked: {
                playbackController.onNextFrame()
            }
        }

        Button {
            objectName: "FastForwardButton"
            text: "FF"
            onClicked: {
                playbackController.onFastForward()
            }
        }
    }
}
