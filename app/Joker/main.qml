/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

import QtQuick 2.0
import Joker 1.0

Item {

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
}

