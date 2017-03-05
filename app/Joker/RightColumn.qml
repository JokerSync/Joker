import QtQuick 2.0
import "qrc:/fonts/fontawesome.js" as FontAwesome

Item {
    width: 200
    height: 500

    Text {
        id: nextTcLabel
        objectName: "nextTcLabel"
        color: "red"
        text: jokerWindow.nextTcLabelText
        wrapMode: Text.WordWrap
        font.pointSize: 29
        anchors.top: parent.top
        anchors.right: parent.right
        horizontalAlignment: Text.AlignRight
        visible: settings.displayNextText
    }

    Item {
        anchors.top: nextTcLabel.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        clip: true

        Item {
            id: nextPeopleList
            objectName: "nextPeopleList"
            y: -jokerWindow.stripTime / settings.verticalTimePerPixel
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 2

            Repeater {
                model: jokerWindow.filteredLineModel

                // FIXME selected colors and inverted colors are not implemented
                delegate: Item {
                    width: parent.width
                    y: timeIn / settings.verticalTimePerPixel
                    x:0

                    Rectangle {
                        color: "#b0b0b0b0" // selected ? "#b0b0b0b0" : "#b0707070"
                        height: childText.height + 2
                        width: childText.width + 2
                        anchors.top: parent.top
                        anchors.right: parent.right

                        antialiasing: true // without antialiasing the jump from one pixel line to the next is visible
                        visible: peopleName.length > 0 // do not draw when the name is empty

                        Text {
                            id: childText
                            text: peopleName
                            color: color
                            font.pointSize: 15
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter // FIXME does not work
                        }
                    }
                }
            }
        }

        // loops
        Item {
            y: -jokerWindow.stripTime / settings.verticalTimePerPixel
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                model: doc.loopModel
                delegate: Item {
                    id: loopContainer
                    y: time/settings.verticalTimePerPixel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    property color loopColor: "gray"

                    Rectangle {
                        color: parent.loopColor
                        anchors.left: parent.left
                        anchors.right: parent.right
                        y: -height/2
                        height: 3
                    }

                    Text {
                        color: parent.loopColor
                        font.pixelSize: 30
                        anchors.centerIn: parent
                        font.family: "FontAwesome"
                        text: FontAwesome.Icon.repeat
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}
