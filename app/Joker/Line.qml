import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

// FIXME color, font, inverted color are not implemented
Item {
    id: stripLineContainer
    // width is arbitray...
    width: 1
    height: parent.height/4
    x: timeIn/horizontalTimePerPixel
    y: parent.height*trackNumber

    Binding { target: model; property: "timeIn"; value: x*horizontalTimePerPixel }
    //Binding { target: model; property: "timeOut"; value: (x + width)*horizontalTimePerPixel }
    Binding { target: model; property: "trackNumber"; value: y/stripContainer.height }

    property var lineModel: model
    property bool editing: false

    // appears when we have just added the opening sign
    Rectangle {
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "blue";
            }
            GradientStop {
                position: 1.00;
                color: "#000000ff";
            }
        }

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: 100
        width: parent.height
        rotation: 90
        visible: textRepeater.count === 0
    }

    Row {
        Repeater {
            id: textRepeater
            model: lineModel.texts
            delegate:
                StripText {
                id: stripTextDelegate
                // reversed stack order so that out detect are on top
                z: -index

                property int textIndex: index

                Binding { target: model; property: "content"; value: text }
                Binding { target: model; property: "duration"; value: width*horizontalTimePerPixel }
            }
        }
    }

    Repeater {
        id: detectRepeater
        model: lineModel.unlinkedDetects
        delegate: Detect {
            x: time/horizontalTimePerPixel
            id: lineDetect
            property int lineIndex: index
        }
    }

    MouseArea {
        id: rightPressArea
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
        color: "steelblue"
        anchors.horizontalCenter: parent.left
        anchors.bottom: parent.bottom

        MouseArea {
            anchors.fill: parent
            drag{ target: parent; axis: Drag.XAxis }

            onMouseXChanged: {
                if(drag.active){
                    console.log("timePerFrame: ", jokerWindow.timePerFrame);
                    var pixelChange = snapToFrame(mouseX)
                    var pixelPerFrame = jokerWindow.timePerFrame / horizontalTimePerPixel
                    if (pixelChange > textRepeater.itemAt(0).width - pixelPerFrame) {
                        pixelChange = textRepeater.itemAt(0).width - pixelPerFrame
                    }
                    stripLineContainer.x = stripLineContainer.x + pixelChange
                    textRepeater.itemAt(0).width = textRepeater.itemAt(0).width - pixelChange
                }
            }
        }
    }
}

