import QtQuick 2.0

Rectangle {
    color: "#8000FFFF"
    width:10
    height: 10
    x: timeIn/horizontalTimePerPixel - stripLineContainer.x

    MouseArea {
        id: detectMouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        onPositionChanged: {
            if (pressed) {
                if (mouse.modifiers & Qt.ControlModifier) {
                    console.log("detect dragged with ctrl")
                } else {
                    console.log("detect dragged")
                }
            }
        }
    }
}
