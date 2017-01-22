import QtQuick 2.0
import QtQuick.Controls 1.1

Item {
    id: loopDelegate

    Rectangle {
        color: settings.invertColor? "white":"black"
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

    Rectangle {
        color: settings.invertColor? "white":"black"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: parent.height/3
        anchors.bottomMargin: parent.height/3
        x: -width/2
        width: parent.height / 40
        rotation: 45

        MouseArea {
            enabled: window.edition
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: loopDelegate.showContextMenu()
        }
    }

    Rectangle {
        color: settings.invertColor? "white":"black"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: parent.height/3
        anchors.bottomMargin: parent.height/3
        x: -width/2
        width: parent.height / 40
        rotation: -45

        MouseArea {
            enabled: window.edition
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: loopDelegate.showContextMenu()
        }
    }

    Text {
        text: label
        color: "gray"
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
                doc.loopModel.remove(loopContextMenu.index);
            }
        }
    }

    function showContextMenu() {
        console.log("index: " + model.index);
        loopContextMenu.index = model.index;
        loopContextMenu.popup();
    }
}
