import QtQuick 2.0
import QtQml 2.2
import QtMultimedia 5.0

Item {
    id: videoContainer
    width: 500
    height: 700

    // black background
    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Image {
        objectName: "videoLogo"
        source: videoLogoUrl
        fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        visible: videoLogoVisible
    }

    VideoOutput {
        id: videoOutput
        source: videoSource
        anchors.fill: parent
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
        visible: noSyncLabelVisible
        opacity: noSyncLabelOpacity
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

    Timer {
        id: panelTimer
        interval: 3000
        onTriggered: {
            mediaPanel.visible = false
        }
    }

    MediaPanel {
        id: mediaPanel
        anchors.left: videoOverlay.left
        anchors.bottom: videoOverlay.bottom
        anchors.right: videoOverlay.right
    }

    function showPanel() {
        panelTimer.restart()
        mediaPanel.visible = true
    }

    function showPanelPermanent() {
        panelTimer.stop()
        mediaPanel.visible = true
    }
}
