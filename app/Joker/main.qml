/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

import QtQuick 2.0
import Joker 1.0
import QtQml 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Item {
    id: item1

    width: 800
    height: 600

    PhQmlView {
        objectName: "PhQmlView"
    }

    Rectangle {
        id: titleRect
        objectName: "titleRect"
        color: "#000080"
        height: childrenRect.height
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

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

    SplitView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleRect.visible ? titleRect.bottom : parent.top
        anchors.bottom: parent.bottom

        orientation: Qt.Vertical

        Video {
            Layout.fillHeight: true
            Layout.minimumHeight: 50
        }

        Strip {
            id: strip
            objectName: "strip"
            // TODO save and restore height on startup
            height: 200
            Layout.minimumHeight: 50
        }
    }
}

