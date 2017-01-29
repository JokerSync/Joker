import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

Item {
    property int currentTrackNumber: 0
    property color maskColor: "#77000000"
    opacity: window.edition

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: parent.maskColor
        height: parent.height * parent.currentTrackNumber  / 4

        Behavior on opacity {
            NumberAnimation {
                easing.type: Easing.Linear
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: 100
                easing.type: Easing.Linear
            }
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height * (3 - parent.currentTrackNumber) / 4
        color: parent.maskColor

        Behavior on opacity {
            NumberAnimation {
                easing.type: Easing.Linear
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: 100
                easing.type: Easing.Linear
            }
        }
    }
}
