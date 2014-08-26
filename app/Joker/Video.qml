import QtQuick 2.6
import QtQml 2.2
import QtMultimedia 5.0
import QtQuick.Controls 1.4

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
        visible: settings.displayLogo && (videoEngine.timeIn >= videoEngine.timeOut)
    }

    VideoOutput {
        id: videoOutput
        source: videoSource
        anchors.fill: parent
    }

    VideoOutput {
        id: pipVideoOutput
        source: pipVideoSource
        visible: settings.videoPictureInPicture

        width: videoOutput.width * settings.videoPictureInPictureRatio
        height: videoOutput.height * settings.videoPictureInPictureRatio

        anchors.top: parent.top
        anchors.topMargin: height
        anchors.rightMargin: width
        anchors.leftMargin: width

        state: settings.videoPictureInPicturePositionRight ? "right" : "left"

        states: [
                State {
                    name: "left"
                    AnchorChanges {
                        target: pipVideoOutput
                        anchors.left: parent.left
                        anchors.right: undefined
                    }
                },

                State {
                    name: "right"
                    AnchorChanges {
                        target: pipVideoOutput
                        anchors.left: undefined
                        anchors.right: parent.right
                    }
                }
            ]
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
        visible: false
        opacity: 0

        state: "SYNCED"

        states: [
                State {
                    name: "SYNCED"
                    PropertyChanges { target: noSyncLabel; visible: false }
                    PropertyChanges { target: noSyncLabel; opacity: 0 }
                },
                State {
                    name: "UNSYNCED"
                    PropertyChanges { target: noSyncLabel; visible: true }
                    PropertyChanges { target: noSyncLabel; opacity: 1 }
                }
            ]

        Connections {
            target: jokerWindow
            onVideoSync: {
                console.log("sync!")
                noSyncLabel.state = "SYNCED"
                noSyncTimer.restart()
            }
        }

        Timer {
            id: noSyncTimer
            interval: 1000
            onTriggered: {
                console.log("unsynced!")
                noSyncLabel.state = "UNSYNCED"
            }
        }

        transitions: [
            Transition {
                from: "SYNCED"
                to: "UNSYNCED"
                NumberAnimation { property: "opacity"; target: noSyncLabel; duration: 1000 }
            },
            Transition {
                from: "UNSYNCED"
                to: "SYNCED"
                NumberAnimation { property: "opacity"; target: noSyncLabel; duration: 100 }
            }
        ]
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
