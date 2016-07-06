import QtQuick 2.6
import QtQuick.Controls 1.4

Rectangle {
    visible: mediaPanel.visible

    property bool edition: editionSwitch.checked

    anchors.horizontalCenter: videoOverlay.horizontalCenter
    anchors.top: videoOverlay.top
    anchors.topMargin: 2

    width: childrenRect.width
    height: childrenRect.height

    color: "black"
    border.color: "white"

    radius: 3

    Row {
        leftPadding: 10
        rightPadding: 10
        topPadding: 4
        bottomPadding: 4
        spacing: 5

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "Projection"
            color: editionSwitch.checked ? "white" : "light blue"
            font.pixelSize: 20
        }
        Switch {
            anchors.verticalCenter: parent.verticalCenter
            id: editionSwitch
            checked: false
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "Edition"
            color: editionSwitch.checked ? "light blue" : "white"
            font.pixelSize: 20
        }
    }
}
