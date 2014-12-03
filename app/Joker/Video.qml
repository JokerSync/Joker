import QtQuick 2.0
import Joker 1.0
import QtQml 2.2
import QtMultimedia 5.0

Item {
    id: videoOverlay
    width: 500
    height: 700

    Image {
        objectName: "videoLogo"
        source: videoLogoUrl
        fillMode: Image.PreserveAspectFit
        anchors.top: videoOverlay.top
        anchors.left: videoOverlay.left
        //width: 500
        anchors.right: videoOverlay.right
        anchors.bottom: videoOverlay.bottom
        //anchors.horizontalCenter: parent.Center
        //anchors.centerIn: videoOverlay
        //horizontalAlignment: Image.AlignHCenter
        //verticalAlignment: Image.AlignVCenter
    }

    VideoOutput {
        id: videoOutput
        source: videoSource
        anchors.fill: parent
        //anchors.top: parent.top
        //anchors.left: parent.left
        //width: 300
        //height: 200
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
        font.pointSize: 30
        wrapMode: Text.WordWrap
        text: qsTr("No video sync")
        anchors.verticalCenter: videoOverlay.verticalCenter
        anchors.horizontalCenter: videoOverlay.horizontalCenter
        anchors.margins: 20
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    LeftColumn {
        anchors.left: videoOverlay.left
        anchors.top: videoOverlay.top
        anchors.bottom: videoOverlay.bottom
    }

    RightColumn {
        anchors.right: videoOverlay.right
        anchors.top: videoOverlay.top
        anchors.bottom: videoOverlay.bottom
    }
}
