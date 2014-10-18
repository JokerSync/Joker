import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    width: 600
    height: 400

    // FIXME color, font, inverted color are not implemented
    Component {
        id: stripTextDelegate
        Item {
            id: stripTextContainer
            width: (timeOut - timeIn)/horizontalTimePerPixel
            height: parent.height

            Rectangle {
                border.width: 1
                border.color: "#30000000"
                color: "#00FFFFFF"
                anchors.fill: parent
                visible: content.length > 0
            }

            Text {
                id: stripText
                text: content
                font.pixelSize: parent.height
                font.family: "Arial"
                transform: Scale {  xScale: stripTextContainer.width/stripText.width;
                                    yScale: 1;}
                smooth: true // smooth scaling
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: ListView.Horizontal
            contentX: jokerWindow.stripTime / horizontalTimePerPixel - width / 6
            interactive: false
            clip: true
            model: stripTextModelTrack0
            delegate: stripTextDelegate
        }


        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: ListView.Horizontal
            contentX: jokerWindow.stripTime / horizontalTimePerPixel - width / 6
            interactive: false
            clip: true
            model: stripTextModelTrack1
            delegate: stripTextDelegate
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: ListView.Horizontal
            contentX: jokerWindow.stripTime / horizontalTimePerPixel - width / 6
            interactive: false
            clip: true
            model: stripTextModelTrack2
            delegate: stripTextDelegate
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: ListView.Horizontal
            contentX: jokerWindow.stripTime / horizontalTimePerPixel - width / 6
            interactive: false
            clip: true
            model: stripTextModelTrack3
            delegate: stripTextDelegate
        }
    }
}
