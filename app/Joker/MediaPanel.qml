import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    //width: 300
    height: panelColumn.height + 20
    color: "#aa0d0d0d"
    radius: 0
    border.width: 0
    border.color: "#aa6d7dff"

    Column {
        id: panelColumn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        spacing: 10

        Text {
            id: mediaPanelTcLabel
            y: 8
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#ffffff"
            text: tcLabelText
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            styleColor: "#ffffff"
            font.pixelSize: 38
        }

        Slider {
            id: timeSlider
            height: 40
            anchors.left: parent.left
            anchors.right: parent.right
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

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                objectName: "BackButton"
                text: "Back"
                onClicked: {
                    playbackController.onBack()
                    timer.restart();
                }
            }

            Button {
                objectName: "FastRewindButton"
                text: "FR"
                onClicked: {
                    playbackController.onRewind()
                    timer.restart();
                }
            }

            Button {
                objectName: "PreviousFrameButton"
                text: "<<"
                onClicked: {
                    playbackController.onPreviousFrame()
                    timer.restart();
                }
            }

            Button {
                objectName: "PlayButton"
                text: "Play/Pause"
                onClicked: {
                    playbackController.onPlayPause()
                    timer.restart();
                }
            }

            Button {
                objectName: "NextFrameButton"
                text: ">>"
                onClicked: {
                    playbackController.onNextFrame()
                    timer.restart();
                }
            }

            Button {
                objectName: "FastForwardButton"
                text: "FF"
                onClicked: {
                    playbackController.onFastForward()
                    timer.restart();
                }
            }
        }
    }
}
