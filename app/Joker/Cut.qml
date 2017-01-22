import QtQuick 2.0
import QtQuick.Controls 1.1

Item {
    id: cutDelegate

    Rectangle {
        color: settings.invertColor? "AAFFFFFF":"#AA000000"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: settings.cutWidth

        MouseArea {
            id: rightPressArea
            enabled: window.edition
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: cutDelegate.showContextMenu()
        }
    }

    Menu {
        id: cutContextMenu
        title: "Edit"
        property int index: 0
        MenuItem {
            text: "Delete cut"
            onTriggered: {
                console.log("Cut " + cutContextMenu.index);
                doc.cutModel.remove(cutContextMenu.index);
            }
        }
    }

    function showContextMenu() {
        console.log("index: " + model.index);
        cutContextMenu.index = model.index;
        cutContextMenu.popup();
    }
}
