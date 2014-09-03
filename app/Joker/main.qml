/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

import QtQuick 2.0
import Joker 1.0

Item {
    id: item1

    width: 320
    height: 480

    PhQmlView {
        objectName: "PhQmlView"
    }

    Column {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 0

        spacing: 0

        move: Transition {
            NumberAnimation { properties: "x,y"; duration: 100 }
        }

        Rectangle {
            id: titleRect
            objectName: "titleRect"
            color: "#000080"
            height: childrenRect.height
            width: parent.width

            Text {
                id: titleLabel
                color: "white"
                wrapMode: Text.WordWrap
                text: doc.fullTitle
                font.pointSize: 8
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
            }
        }

        Item {
            width: parent.width
            height: childrenRect.height

            Text {
                id: tcLabel
                objectName: "tcLabel"
                color: "#00ff00"
                text: "#"
                wrapMode: Text.WordWrap
                font.pointSize: 29
            }

            Text {
                objectName: "nextTcLabel"
                color: "red"
                text: "#"
                wrapMode: Text.WordWrap
                font.pointSize: tcLabel.font.pointSize
                width: parent.width
                horizontalAlignment: Text.AlignRight
            }
        }

        ListView {
            objectName: "selectedPeopleList"
            width: parent.width
            height: childrenRect.height
            anchors.left: parent.left
            anchors.leftMargin: 10

            model: selectedPeopleModel
            delegate: Text {
                text: name
                color: "grey"
                font.pointSize: 15
                lineHeight: 0.75
                wrapMode: Text.WordWrap
            }
        }

        ListView {
            objectName: "infoList"
            width: parent.width
            height: childrenRect.height
            anchors.left: parent.left
            anchors.leftMargin: 10

            model: infoModel
            delegate: Text {
                text: modelData
                color: "red"
                font.pointSize: tcLabel.font.pointSize
                lineHeight: 0.75
                wrapMode: Text.WordWrap
            }
        }
    }

    Image {
        objectName: "videoLogo"
        //source property is set from C++
        fillMode: Image.PreserveAspectFit
        anchors.top: titleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Rectangle {
        id: noSyncRect
        color: "#c0500000"
        anchors.leftMargin: -10
        anchors.rightMargin: -10
        anchors.bottomMargin: -2
        anchors.topMargin: -2
        anchors.fill: noSyncLabel
        visible: noSyncLabel.visible
        opacity: noSyncLabel.opacity
    }

    Text {
        id: noSyncLabel
        objectName: "noSyncLabel"
        color: "red"
        wrapMode: Text.WordWrap
        text: qsTr("No video sync")
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 30
        horizontalAlignment: Text.AlignHCenter
        anchors.margins: 20
    }

    Text {
        objectName: "currentLoopLabel"
        color: "blue"
        text: jokerWindow.currentLoopLabel
        wrapMode: Text.WordWrap
        anchors.left: parent.left
        font.pointSize: 50
        anchors.leftMargin: 10
    }
}

