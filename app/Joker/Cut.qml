import QtQuick 2.0
import QtQuick.Controls 1.1

Item {
    id: cutDelegate

    Rectangle {
        color: settings.invertColor? "AAFFFFFF":"#AA000000"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: settings.cutWidth
        x: -width/2

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
        MenuItem {
            text: "Delete cut"
            onTriggered: {
                console.log("Delete cut");
                remove();
            }
        }
    }

    function remove() {
        doc.cutModel.remove(model.index);
    }

    function showContextMenu() {
        cutContextMenu.popup();
    }
}
