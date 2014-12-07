import QtQuick 2.0

Rectangle {
    width: 300
    height: 120
    color: "#0d0d0d"
    radius: 0
    border.width: 1
    border.color: "#6d7dff"

    Text {
        id: text1
        x: 72
        y: 8
        color: "#ffffff"
        text: qsTr("#####")
        textFormat: Text.PlainText
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        styleColor: "#ffffff"
        font.pixelSize: 38
    }
}
