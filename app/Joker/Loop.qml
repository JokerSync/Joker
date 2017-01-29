import QtQuick 2.0
import QtQuick.Controls 1.1
import "qrc:/fonts/fontawesome.js" as FontAwesome

Item {
    id: loopDelegate
    property color loopColor: "gray"

    Rectangle {
        color: parent.loopColor
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        x: -width/2
        width: parent.height / 40

        MouseArea {
            enabled: window.edition
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: loopDelegate.showContextMenu()
        }
    }

    Text {
        color: parent.loopColor
        font.pixelSize: parent.height/4
        anchors.centerIn: parent
        font.family: "FontAwesome"
        text: FontAwesome.Icon.repeat
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        MouseArea {
            enabled: window.edition
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: loopDelegate.showContextMenu()
        }
    }

    Text {
        id: loopNumberLabel
        text: label
        color: parent.loopColor
        font.pixelSize: parent.height/4
        font.family: "Arial"
        x: 10
        y: parent.height * 2 / 3

        MouseArea {
            enabled: window.edition
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: loopDelegate.showContextMenu()
        }
    }

    Menu {
        id: loopContextMenu
        title: "Edit"
        property int index: 0
        MenuItem {
            text: "Delete loop"
            onTriggered: {
                console.log("Loop " + loopContextMenu.index);
                doc.loopModel.remove(loopDelegate.model.index);
            }
        }
    }

    function showContextMenu() {
        console.log("index: " + model.index);
        loopContextMenu.index = model.index;
        loopContextMenu.popup();
    }
}
