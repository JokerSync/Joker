import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

// FIXME color, font, inverted color are not implemented
Item {
    id: stripLineContainer
    width: (timeOut - timeIn)/horizontalTimePerPixel
    height: parent.height/4
    x: timeIn/horizontalTimePerPixel
    y: parent.height*trackNumber

    Binding { target: model; property: "timeIn"; value: x*horizontalTimePerPixel }
    Binding { target: model; property: "timeOut"; value: (x + width)*horizontalTimePerPixel }
    Binding { target: model; property: "trackNumber"; value: y/stripContainer.height }

    Rectangle {
        border.width: 1
        border.color: "#30FF0000"
        color: "#00FFFFFF"
        anchors.fill: parent
        visible: content.length > 0
    }

    property var lineModel: model
    property bool editing: false

    Repeater {
        id: textRepeater
        model: lineModel.texts
        delegate: StripText {}
    }

    Repeater {
        model: lineModel.detects
        delegate: Detect {}
    }

    MouseArea {
        id: longPressArea
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            console.log("index: " + model.index);
            myContextMenu.index = model.index;
            myContextMenu.popup();
        }
    }

    property int rulersSize: 18

    // left handle
    Rectangle {
        width: rulersSize
        height: rulersSize
        radius: rulersSize
        color: "steelblue"
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.verticalCenter

        MouseArea {
            anchors.fill: parent
            drag{ target: parent; axis: Drag.XAxis }
            onMouseXChanged: {
                if(drag.active){
                    console.log("timePerFrame: ", jokerWindow.timePerFrame);
                    var pixelPerFrame = jokerWindow.timePerFrame / horizontalTimePerPixel
                    var pixelChange = mouseX
                    var timeChange = pixelChange * horizontalTimePerPixel
                    // round to frame
                    var frameChange = Math.round(timeChange / jokerWindow.timePerFrame)
                    timeChange = frameChange * jokerWindow.timePerFrame
                    pixelChange = timeChange / horizontalTimePerPixel

                    if (pixelChange > stripLineContainer.width - pixelPerFrame) {
                        pixelChange = stripLineContainer.width - pixelPerFrame
                    }
                    stripLineContainer.width = stripLineContainer.width - pixelChange
                    stripLineContainer.x = stripLineContainer.x + pixelChange
                }
            }
        }
    }

    // right handle
    Rectangle {
        width: rulersSize
        height: rulersSize
        radius: rulersSize
        color: "steelblue"
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.verticalCenter

        MouseArea {
            anchors.fill: parent
            drag{ target: parent; axis: Drag.XAxis }
            onMouseXChanged: {
                if(drag.active){
                    var pixelPerFrame = jokerWindow.timePerFrame / horizontalTimePerPixel
                    var pixelChange = mouseX
                    var timeChange = pixelChange * horizontalTimePerPixel
                    // round to frame
                    var frameChange = Math.round(timeChange / jokerWindow.timePerFrame)
                    timeChange = frameChange * jokerWindow.timePerFrame
                    pixelChange = timeChange / horizontalTimePerPixel

                    stripLineContainer.width = stripLineContainer.width + pixelChange
                    if(stripLineContainer.width < pixelPerFrame)
                        stripLineContainer.width = pixelPerFrame
                }
            }
        }
    }
}

