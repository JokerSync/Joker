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

    Rectangle {
        id: titleRect
        objectName: "titleRect"
        color: "#000080"
        anchors.leftMargin: -20
        anchors.rightMargin: -20
        anchors.bottomMargin: -2
        anchors.topMargin: -2
        anchors.fill: titleLabel
        anchors.margins: -10
    }

    Text {
        id: titleLabel
        color: "#ffffff"
        wrapMode: Text.WordWrap
        text: doc.fullTitle
        font.pointSize: 8
        anchors.topMargin: 0
        horizontalAlignment: Text.AlignHCenter
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20
        visible: titleRect.visible
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
        color: "#ff0000"
        wrapMode: Text.WordWrap
        text: qsTr("No video sync")
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 30
        horizontalAlignment: Text.AlignHCenter
        anchors.margins: 20
    }
}

