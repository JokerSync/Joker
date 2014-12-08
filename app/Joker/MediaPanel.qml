import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    width: 300
    height: 120
    color: "#aa0d0d0d"
    radius: 0
    border.width: 1
    border.color: "#aa6d7dff"

    Column {
        anchors.fill: parent

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

        Row {

            Button {
                objectName: "BackButton"
                text: "Back"
            }

            Button {
                objectName: "FastRewindButton"
                text: "FR"
            }

            Button {
                objectName: "PreviousFrameButton"
                text: "<<"
            }

            Button {
                objectName: "PlayButton"
                text: "Play/Pause"
            }

            Button {
                objectName: "NextFrameButton"
                text: ">>"
            }

            Button {
                objectName: "FastForwardButton"
                text: "FF"
            }
        }
    }
}
