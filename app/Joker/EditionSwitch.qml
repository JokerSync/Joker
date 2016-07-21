import QtQuick 2.6
import QtQuick.Controls 1.4

Rectangle {
    height: 1.5*childrenRect.height

    property bool edition: editionSwitch.checked

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#aa0d0d0d" }
        GradientStop { position: 1.0; color: "#000d0d0d" }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
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
