import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    width: 600
    height: 400

    // the font name is passed from here
    FontLoader { id: stripFont; source: textFontUrl }

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
                font.family: stripFont.name
                font.weight: textBoldness * 99/5
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
            model: stripTextModelTrack0
            delegate: stripTextDelegate
        }


        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: ListView.Horizontal
            contentX: jokerWindow.stripTime / horizontalTimePerPixel - width / 6
            interactive: false
            model: stripTextModelTrack1
            delegate: stripTextDelegate
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: ListView.Horizontal
            contentX: jokerWindow.stripTime / horizontalTimePerPixel - width / 6
            interactive: false
            model: stripTextModelTrack2
            delegate: stripTextDelegate
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: ListView.Horizontal
            contentX: jokerWindow.stripTime / horizontalTimePerPixel - width / 6
            interactive: false
            model: stripTextModelTrack3
            delegate: stripTextDelegate
        }
    }

    Rectangle {
        x: parent.width/6
        y: 0
        width: 4
        height: parent.height
        color: "#FFFF566C"
    }

    ListView {
        anchors.fill: parent
        orientation: ListView.Horizontal
        contentX: jokerWindow.stripTime / horizontalTimePerPixel - width / 6
        interactive: false
        model: cutModel
        visible: displayCuts
        delegate: Item {
            width: duration/horizontalTimePerPixel
            height: parent.height

            Rectangle {
                color: invertColor? "white":"black"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                x: parent.width
                width: cutWidth
            }
        }
    }
}
