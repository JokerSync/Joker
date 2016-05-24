import QtQuick 2.5

Item {
    id: stripTextItem2
    width: duration/horizontalTimePerPixel
    height: stripLineContainer.height

    property string text: stripTextInput.text
    property bool textFocus: stripTextInput.focus
    property int cursorPosition: stripTextInput.cursorPosition

    TextInput {
        id: stripTextInput
        text: content
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: stripLineContainer.height
        font.family: stripFont.name
        font.weight: textBoldness * 99/5
        transform: Scale {  xScale: stripTextItem2.width/stripTextInput.width;
                            yScale: 1;}

        cursorDelegate: Rectangle {
            // cancel the TextInput scale
            width: stripTextInput.width/stripTextItem2.width
            height: stripTextInput.height
            color: stripTextInput.color
            visible: stripTextInput.focus
        }

        smooth: true // smooth scaling

        color: stripLineContainer.editing ? "slateblue" : "black"

        onEditingFinished: {
            console.log("Editing finished")
            focus = false
            stripLineContainer.editing = false
        }

        Keys.onRightPressed: {
            console.log('Right Key was pressed ' + stripTextInput.cursorPosition + ' ' + stripTextInput.text.length);
            if (stripTextInput.cursorPosition === stripTextItem2.text.length) {
                giveFocusToNextItem();
            } else {
                stripTextInput.cursorPosition = stripTextInput.cursorPosition + 1
            }
            event.accepted = true;
        }

        Keys.onLeftPressed: {
            console.log('Left Key was pressed ' + stripTextInput.cursorPosition);
            if (stripTextInput.cursorPosition == 0) {
                // give focus to previous item!
                console.log("I'm item " + stripTextDelegate.textIndex);
                var previousItem = textRepeater.itemAt(stripTextDelegate.textIndex-1)
                previousItem.textFocus = true
                previousItem.cursorPosition = previousItem.text.length - 1
                stripLineContainer.editing = true
            } else {
                stripTextInput.cursorPosition = stripTextInput.cursorPosition - 1
            }
            event.accepted = true;
        }
    }

    // drag mouse area
    MouseArea {
        id: dragArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        drag{
            target: stripLineContainer
            minimumY: -stripLineContainer.y
            maximumY: stripContainer.height - stripLineContainer.height - stripLineContainer.y
            smoothed: true
        }

        enabled: !stripTextInput.focus

        onDoubleClicked: {
            // give focus to the textinput
            stripTextInput.focus = true
            stripLineContainer.editing = true
            var newPos = stripTextInput.positionAt(mouseX/stripTextItem2.width*stripTextInput.width, mouseY)
            console.log(mouseX + " " + mouseY + " " + newPos);
            stripTextInput.cursorPosition = newPos
        }

        onPositionChanged: {
            if(drag.active){
                // snap x to whole frame
                stripLineContainer.x = snapToFrame(stripLineContainer.x);
                // snap y to track
                stripLineContainer.y = Math.round(stripLineContainer.y / height) * height;
            }
        }

//                onClicked: {
//                    stripTextItem2.color = Qt.rgba(Math.random(), Math.random(), Math.random(), 1);
//                }
    }

    // right handle
    Rectangle {
        width: 18
        height: 18
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.right
        color: "steelblue"

        MouseArea {
            anchors.fill: parent
            drag{ target: parent; axis: Drag.XAxis }
            onMouseXChanged: {
                if(drag.active){
                    var pixelPerFrame = jokerWindow.timePerFrame / horizontalTimePerPixel
                    var pixelChange = snapToFrame(mouseX - parent.width)
                    stripTextItem2.width = stripTextItem2.width + pixelChange
                    if(stripTextItem2.width < pixelPerFrame)
                        stripTextItem2.width = pixelPerFrame
                }
            }
        }
    }

    function giveFocusToNextItem() {
        // give focus to next item!
        console.log("I'm item " + index);
        var nextItem = textRepeater.itemAt(index + 1)
        nextItem.textFocus = true
        nextItem.cursorPosition = 1
        stripLineContainer.editing = true
    }
}
