import QtQuick 2.5

TextInput {
    id: stripTextItem2
    x: timeIn/horizontalTimePerPixel - stripLineContainer.x
    text: content
    font.pixelSize: stripLineContainer.height
    font.family: stripFont.name
    font.weight: textBoldness * 99/5
    transform: Scale {  xScale: (timeOut - timeIn)/horizontalTimePerPixel/stripTextItem2.width;
                        yScale: 1;}

    cursorDelegate: Rectangle {
        // cancel the TextInput scale
        width: stripTextItem2.width*horizontalTimePerPixel/(timeOut - timeIn)
        height: stripTextItem2.height
        color: stripTextItem2.color
        visible: stripTextItem2.focus
    }

    smooth: true // smooth scaling

    color: stripLineContainer.editing ? "slateblue" : "black"

    onEditingFinished: {
        console.log("Editing finished")
        focus = false
        stripLineContainer.editing = false
    }

//                    Binding { target: model; property: "content"; value: stripTextItem2.text }

    Keys.onRightPressed: {
        console.log('Right Key was pressed ' + stripTextItem2.cursorPosition + ' ' + stripTextItem2.text.length);
        if (stripTextItem2.cursorPosition === stripTextItem2.text.length) {
            // give focus to next item!
            console.log("I'm item " + index);
            var nextItem = textRepeater.itemAt(index+1)
            nextItem.focus = true
            nextItem.cursorPosition = 1
            stripLineContainer.editing = true
        } else {
            stripTextItem2.cursorPosition = stripTextItem2.cursorPosition + 1
        }
        event.accepted = true;
    }

    Keys.onLeftPressed: {
        console.log('Left Key was pressed ' + stripTextItem2.cursorPosition);
        if (stripTextItem2.cursorPosition == 0) {
            // give focus to previous item!
            console.log("I'm item " + index);
            var previousItem = textRepeater.itemAt(index-1)
            previousItem.focus = true
            previousItem.cursorPosition = previousItem.text.length - 1
            stripLineContainer.editing = true
        } else {
            stripTextItem2.cursorPosition = stripTextItem2.cursorPosition - 1
        }
        event.accepted = true;
    }

    // drag mouse area
    MouseArea {
        id: dragArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        drag{
            target: parent
            minimumY: 0
            maximumY: stripContainer.height - stripLineContainer.height
            smoothed: true
        }

        enabled: !stripTextItem2.focus

        onDoubleClicked: {
            // give focus to the textinput
            stripTextItem2.focus = true
            stripLineContainer.editing = true
            var newPos = stripTextItem2.positionAt(mouseX, mouseY)
            stripTextItem2.cursorPosition = newPos
        }

        onPositionChanged: {
            if(drag.active){
                // snap x to whole frame
                var timePosition = stripLineContainer.x * horizontalTimePerPixel
                var framePosition = Math.round(timePosition / jokerWindow.timePerFrame)
                timePosition = framePosition * jokerWindow.timePerFrame
                stripLineContainer.x = timePosition / horizontalTimePerPixel

                // snap y to track
                var trackNumber = Math.round(stripLineContainer.y / stripContainer.height * 4) / 4
                stripLineContainer.y = stripContainer.height*trackNumber
            }
        }

//                onClicked: {
//                    stripTextItem2.color = Qt.rgba(Math.random(), Math.random(), Math.random(), 1);
//                }
    }
}
