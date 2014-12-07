import QtQuick 2.0

Item {
    id: leftColumn
    width: 300
    height: 500

    Text {
        id: tcLabel
        objectName: "tcLabel"
        color: "#00ff00"
        text: tcLabelText
        wrapMode: Text.WordWrap
        font.pointSize: 29
        anchors.top: parent.top
        anchors.left: parent.left
        visible: tcLabelVisible
    }

    Column {
        objectName: "infos"
        anchors.top: tcLabel.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        visible: infosVisible

        Text {
            text: jokerWindow.updateInfo
            color: "red"
            font.pointSize: tcLabel.font.pointSize
            lineHeight: 0.75
            wrapMode: Text.WordWrap
        }

        Text {
            text: jokerWindow.refreshInfo
            color: "red"
            font.pointSize: tcLabel.font.pointSize
            lineHeight: 0.75
            wrapMode: Text.WordWrap
        }

        Text {
            text: jokerWindow.dropInfo
            color: "red"
            font.pointSize: tcLabel.font.pointSize
            lineHeight: 0.75
            wrapMode: Text.WordWrap
        }

        Text {
            text: jokerWindow.stripInfo
            color: "red"
            font.pointSize: tcLabel.font.pointSize
            lineHeight: 0.75
            wrapMode: Text.WordWrap
        }
    }

    ListView {
        objectName: "selectedPeopleList"
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: tcLabel.bottom
        height: childrenRect.height
        visible: selectedPeopleListVisible

        model: selectedPeopleModel
        delegate: Text {
            text: display
            color: "grey"
            font.pointSize: 15
            lineHeight: 0.75
            wrapMode: Text.WordWrap
        }
    }

    Text {
        id: currentLoopLabel
        objectName: "currentLoopLabel"
        color: "blue"
        text: jokerWindow.currentLoopLabel
        wrapMode: Text.WordWrap
        font.pointSize: 50
        anchors.bottom: leftColumn.bottom
        anchors.left: leftColumn.left
        anchors.leftMargin: 10
        anchors.bottomMargin: 0
    }
}
