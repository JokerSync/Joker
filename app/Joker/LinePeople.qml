import QtQuick 2.0
import QtQuick.Controls 1.1

// FIXME selection, color, font, inverted color are not implemented
Item {
    property string name
    width: childrenRect.width
    height: childrenRect.height

    Text {
        text: parent.name
        font.pixelSize: stripLineContainer.height/3
        font.family: "Arial"
        color: "blue"
        smooth: true

        MouseArea {
            enabled: window.edition
            anchors.fill: parent
            acceptedButtons: "LeftButton"
            onDoubleClicked: {
                peopleConnection.isEnabled = true
                peopleSelection.show(name)
            }
        }

        MouseArea {
            id: rightPressArea
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            enabled: window.edition
            onClicked: {
                contextMenu.popup();
            }
        }
    }

    Connections {
        id: peopleConnection
        property bool isEnabled: false
        target: peopleSelection
        onSelected: {
            if (isEnabled) {
                console.log("onSelected " + name + " " + index)
                stripLineContainer.assignToPeople(name)
                isEnabled = false
            }
        }
    }

    Menu {
        id: contextMenu
        title: "Edit"
        MenuItem {
            text: "Change people"
            onTriggered: {
                peopleConnection.isEnabled = true
                peopleSelection.show(name)
            }
        }
    }
}
