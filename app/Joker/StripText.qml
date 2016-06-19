import QtQuick 2.5
import QtQuick.Controls 1.1

FocusScope {
    id: stripTextItem2
    width: duration/horizontalTimePerPixel
    height: stripLineContainer.height

    property string text: stripTextInput.text
    property bool textFocus: stripTextInput.focus
    property int cursorPosition: stripTextInput.cursorPosition
    property color emptyEditing: "#A00000FF"
    property color emptyNonEditing: "#A0AAAAAA"

    // appears when we create a new line by adding the ending sign
    Rectangle {
        anchors.fill: parent
        color: stripLineContainer.editing ? emptyEditing : emptyNonEditing
        visible: content.length === 0
    }

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
            width: 2*stripTextInput.width/stripTextItem2.width
            height: stripTextInput.height
            color: stripTextInput.color
            visible: stripTextInput.activeFocus

            SequentialAnimation on visible {
                    loops: Animation.Infinite
                    running: stripTextInput.activeFocus
                    PropertyAnimation { to: true; duration: 500 }
                    PropertyAnimation { to: false; duration: 500 }
                    onStopped: visible = false
                }
        }

        smooth: true // smooth scaling

        color: stripLineContainer.editing ? "slateblue" : "black"

        onEditingFinished: {
            console.log("Editing finished")
            focus = false
            stripContainer.forceActiveFocus()
            stripLineContainer.editing = false
        }

        onActiveFocusChanged: console.log("active focus changed: ", activeFocus)
        onFocusChanged: console.log("focus changed: ", focus)

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

        MouseArea {
            id: rightPressArea
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: {
                var posX = stripTextDelegate.x + mouse.x
                console.log("click " + posX);
                stripLineContainer.showContextMenu(posX)
            }
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

        enabled: !stripTextInput.activeFocus

        onDoubleClicked: {
            editTextAt(mouseX, mouseY)
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

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            drag{
                target: parent
                axis: Drag.XAxis
                smoothed: true
            }

            property int startX: 0

            onPressed: {
                startX  = mouse.x
            }

            onPositionChanged: {
                if (drag.active) {
                    if (mouse.modifiers & Qt.ControlModifier) {
                        var pixelPerFrame = jokerWindow.timePerFrame / horizontalTimePerPixel
                        var pixelChange = snapToFrame(mouseX - parent.width)
                        stripTextItem2.width = stripTextItem2.width + pixelChange
                        if(stripTextItem2.width < pixelPerFrame)
                            stripTextItem2.width = pixelPerFrame
                    } else {
                        var movement = mouseX - startX;
                        console.log("detect dragged without ctrl")
                        var shift = Math.round(movement/40);
                        console.log("Shift is " + shift + ", I'm item " + index);
                        if (shift != 0) {
                            shiftText(shift)
                            startX = mouse.x
                        }
                    }
                }
            }
        }
    }

    function shiftText(shift) {
        // give focus to next item!
        console.log("I'm item " + index);

        if (shift === 0) {
            return
        }

        if (index === textRepeater.count - 1) {
            return
        }

        var nextText = textRepeater.itemAt(index+1)
        var currentLength = text.length
        var nextLength = nextText.text.length

        if (shift > 0) {
            if (shift > currentLength - 1) {
                shift = currentLength - 1
            }

            var textMoved = text.substr(currentLength-shift,shift)

            console.log(textMoved + " " + text + " " + nextText.text)

            text = text.substr(0,currentLength-shift)
            nextText.text = textMoved + nextText.text
        } else {
            var absoluteShift = -shift

            if (absoluteShift > nextLength - 1) {
                absoluteShift = nextLength - 1
            }

            var nextTextMoved = nextText.text.substr(0, absoluteShift)

            console.log(nextTextMoved + " - " + text + " - " + nextText.text)

            text = text + nextTextMoved
            nextText.text = nextText.text.substr(absoluteShift,nextLength-absoluteShift)
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

    function editTextAt(x, y) {
        var newPos = stripTextInput.positionAt(x/stripTextItem2.width*stripTextInput.width, y)
        console.log("text.editTextAt " + newPos)
        stripContainer.forceActiveFocus()
        stripTextInput.forceActiveFocus()
        stripLineContainer.editing = true
        stripTextInput.cursorPosition = newPos
    }
}
