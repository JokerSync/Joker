import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Rectangle {
    height: 1.5*childrenRect.height

    property bool edition: editionSwitch.checked

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#aa0d0d0d" }
        GradientStop { position: 1.0; color: "#000d0d0d" }
    }

    RowLayout  {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 5

        Text {
            anchors.verticalCenter: parent.verticalCenter
            color: "white"
            text: doc.fullTitle
            font.pointSize: 20
        }

        Item {
            Layout.fillWidth: true
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "Projection"
            color: "white"
            font.pixelSize: 20
            font.underline: !editionSwitch.checked
        }
        Switch {
            anchors.verticalCenter: parent.verticalCenter
            id: editionSwitch
            checked: false
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "Edition"
            color: "white"
            font.pixelSize: 20
            font.underline: editionSwitch.checked
        }
    }
}
