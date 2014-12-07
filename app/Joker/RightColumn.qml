import QtQuick 2.0

Item {
    width: 300
    height: 500

    Text {
        id: nextTcLabel
        objectName: "nextTcLabel"
        color: "red"
        text: nextTcLabelText
        wrapMode: Text.WordWrap
        font.pointSize: 29
        anchors.top: parent.top
        anchors.right: parent.right
        horizontalAlignment: Text.AlignRight
        visible: nextTcLabelVisible
    }

    ListView {
        id: nextPeopleList
        objectName: "nextPeopleList"
        anchors.top: nextTcLabel.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 2
        orientation: ListView.Vertical
        contentY:  -height -jokerWindow.stripTime / verticalTimePerPixel
        verticalLayoutDirection: "BottomToTop"
        interactive: false
        clip: true

        model: nextPeopleModel

        // FIXME inverted colors are not implemented
        delegate: Item {
            width: parent.width
            height: duration / verticalTimePerPixel
            x:0

            Rectangle {
                color: selected ? "#b0b0b0b0" : "#b0707070"
                height: childrenRect.height + 2
                width: childrenRect.width + 2
                anchors.bottom: parent.bottom
                anchors.right: parent.right

                antialiasing: true // without antialiasing the jump from one pixel line to the next is visible
                visible: name.length > 0 // do not draw when the name is empty

                Text {
                    text: name
                    color: color
                    font.pointSize: 15
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter // FIXME does not work
                }
            }
        }
    }

    // loops
    ListView {
        anchors.top: nextTcLabel.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: parent.width/2
        orientation: ListView.Vertical
        contentY:  -height -jokerWindow.stripTime / verticalTimePerPixel
        verticalLayoutDirection: "BottomToTop"
        interactive: false
        model: loopModel
        delegate: Item {
            id: loopContainer
            height: duration/verticalTimePerPixel
            width: parent.width

            Rectangle {
                color: "white"
                anchors.left: parent.left
                anchors.right: parent.right
                y: -height/2
                height: 3
            }

            Rectangle {
                color: "white"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: parent.width*2/5
                anchors.rightMargin: parent.width*2/5
                y: -height/2
                height: 3
                rotation: 45
                transformOrigin: Center
            }

            Rectangle {
                color: "white"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: parent.width*2/5
                anchors.rightMargin: parent.width*2/5
                y: -height/2
                height: 3
                rotation: -45
                transformOrigin: Center
            }
        }
    }
}
