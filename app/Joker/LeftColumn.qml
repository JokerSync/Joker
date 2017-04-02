import QtQuick 2.0
import "qrc:/fonts/fontawesome.js" as FontAwesome

Item {
    id: leftColumn
    width: 300
    height: 500

    Text {
        id: tcLabel
        objectName: "tcLabel"
        color: "#00ff00"
        text: jokerWindow.tcLabelText
        font.pointSize: 29
        anchors.top: parent.top
        anchors.left: parent.left
        visible: settings.displayNextText
    }

    Column {
        objectName: "infos"
        y: 50
        anchors.left: parent.left
        anchors.leftMargin: 10
        visible: settings.displayInfo

        Text {
            text: jokerWindow.updateInfo
            color: "red"
            font.pointSize: tcLabel.font.pointSize
            lineHeight: 0.75
        }

        Text {
            text: jokerWindow.refreshInfo
            color: "red"
            font.pointSize: tcLabel.font.pointSize
            lineHeight: 0.75
        }

        Text {
            text: jokerWindow.dropInfo
            color: "red"
            font.pointSize: tcLabel.font.pointSize
            lineHeight: 0.75
        }

        Text {
            text: jokerWindow.stripInfo
            color: "red"
            font.pointSize: tcLabel.font.pointSize
            lineHeight: 0.75
        }
    }

    ListView {
        objectName: "selectedPeopleList"
        anchors.left: parent.left
        anchors.leftMargin: 10
        y: 50
        height: childrenRect.height
        visible: settings.displayNextText

        model: selectedPeopleModel
        delegate: Text {
            text: display
            color: "grey"
            font.pointSize: 15
            lineHeight: 0.75
        }
    }

    Text {
        id: loopIcon
        color: "blue"
        font.family: "FontAwesome"
        text: FontAwesome.Icon.repeat
        font.pointSize: 15
        anchors.bottom: currentLoopLabel.top
        anchors.left: currentLoopLabel.left
        visible: settings.displayNextText && jokerWindow.currentLoopLabel
    }

    Text {
        id: currentLoopLabel
        objectName: "currentLoopLabel"
        color: "blue"
        text: jokerWindow.currentLoopLabel
        font.pointSize: 50
        anchors.bottom: leftColumn.bottom
        anchors.left: leftColumn.left
        anchors.leftMargin: 10
        anchors.bottomMargin: 0
        visible: settings.displayNextText
    }
}
