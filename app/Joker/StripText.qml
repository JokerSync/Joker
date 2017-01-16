import QtQuick 2.5
import QtQuick.Controls 1.1
import PhImport 1.0 as Ph
import "qrc:/qml/colors.js" as Colors
import "qrc:/qml/symbols.js" as Symbols
import "qrc:/fonts/fontawesome.js" as FontAwesome

FocusScope {
    id: stripTextItem2
    height: stripLineContainer.height

    width: duration/settings.horizontalTimePerPixel
    // maintain the binding even after width is statistically reassigned during a drag
    property int modelDuration: duration
    onModelDurationChanged: width = duration/settings.horizontalTimePerPixel

    property string text: stripTextInput.text
    property bool textFocus: stripTextInput.focus
    property int cursorPosition: stripTextInput.cursorPosition
    property color emptyEditing: "#A00000FF"
    property color emptyNonEditing: "#A0AAAAAA"

    property int modelTypeOut: typeOut

    // Load the "FontAwesome" font for the detection icons.
    FontLoader {
        source: "qrc:/fonts/fontawesome-webfont.ttf"
    }

    function setText(text) {
        content = text;
    }

    // appears when we create a new line by adding the ending sign
    Rectangle {
        anchors.fill: parent
        color: stripLineContainer.editing ? emptyEditing : emptyNonEditing
        visible: content.length === 0 && window.edition

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: {
                console.log("click " + mouse.x + " " + model.index);
                stripLineContainer.showContextMenu(mouse.x, model.index)
            }
        }
    }

    TextInput {
        id: stripTextInput
        readOnly: !window.edition
        text: content
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: stripLineContainer.height * 0.8
        font.family: stripFont.name
        font.weight: Font.Bold //settings.textBoldness * 99/5
        transform: Scale {  xScale: stripTextItem2.width/stripTextInput.width;
                            yScale: 1;}

        cursorDelegate: Rectangle {
            // cancel the TextInput scale
            width: 2*stripTextInput.width/stripTextItem2.width
            height: stripTextInput.height
            color: stripTextInput.color
            visible: stripTextInput.activeFocus && window.edition

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
                event.accepted = true;
            } else {
                event.accepted = false;
            }
        }

        Keys.onLeftPressed: {
            console.log('Left Key was pressed ' + stripTextInput.cursorPosition);
            if (stripTextInput.cursorPosition == 0) {
                giveFocusToPreviousItem();
                event.accepted = true;
            } else {
                event.accepted = false;
            }
        }

        Keys.onEscapePressed: {
            console.log('Escape pressed');
            while (canUndo) {
                undo()
            }
            editingFinished();
        }
    }

    MouseArea {
        enabled: window.edition
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            console.log("click " + mouse.x + " " + model.index);
            stripLineContainer.showContextMenu(mouse.x, model.index)
        }
    }

    // drag mouse area
    MouseArea {
        id: dragArea
        enabled: window.edition && !stripTextInput.activeFocus
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        propagateComposedEvents: true

        drag{
            target: stripLineContainer
            minimumY: -stripLineContainer.y
            maximumY: stripContainer.height - stripLineContainer.height - stripLineContainer.y
            smoothed: true
        }

        onDoubleClicked: {
            console.log("double click on text")
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
    }

    // right handle
    Rectangle {
        width: 18
        height: 18
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.right
        color: Colors.colorFromDetectType(typeOut)
        visible: window.edition

        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "black"
            styleColor: "black"
            font.bold: true
            font.pixelSize: parent.width - 2
            font.family: "FontAwesome"
            text: Symbols.symbolFromDetectType(typeOut)
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

            onDoubleClicked: {
                detach()
            }

            onPositionChanged: {
                if (drag.active) {
                    if (mouse.modifiers & Qt.ControlModifier || stripTextDelegate.last) {
                        var pixelPerFrame = jokerWindow.timePerFrame / settings.horizontalTimePerPixel
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

        MouseArea {
            id: rightPressArea
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            enabled: window.edition
            onClicked: {
                console.log("index: " + model.index);
                detectContextMenu.index = model.index;
                detectContextMenu.popup();
            }
        }
    }

    Menu {
        id: detectContextMenu
        title: "Edit"
        property int index: 0
        MenuItem {
            text: FontAwesome.Icon.unlink + " Detach detect"
            visible: !stripTextDelegate.last
            onTriggered: {
                console.log("Detach " + detectContextMenu.index);
                detach()
            }
        }
//        MenuItem {
//            text: "Delete phrase"
//            onTriggered: {
//                console.log("Detect " + detectContextMenu.index);
//                stripLineContainer.lineModel.unlinkedDetects.remove(detectContextMenu.index);
//            }
//        }
        Menu {
            title: "Change type"

            MenuItem {
                text: "Mouth opened"
                enabled: model.typeOut !== Ph.PhStripDetect.MouthOpen
                visible: stripTextDelegate.last
                onTriggered: {
                    model.typeOut = Ph.PhStripDetect.MouthOpen
                }
            }
            MenuItem {
                text: "Mouth closed"
                enabled: model.typeOut !== Ph.PhStripDetect.MouthClosed
                visible: stripTextDelegate.last
                onTriggered: {
                    model.typeOut = Ph.PhStripDetect.MouthClosed
                }
            }
            MenuItem {
                text: "Labial"
                visible: !stripTextDelegate.last
                onTriggered: {
                    model.typeOut = Ph.PhStripDetect.Labial
                }
            }
            MenuItem {
                text: "Dental"
                visible: !stripTextDelegate.last
                onTriggered: {
                    model.typeOut = Ph.PhStripDetect.Dental
                }
            }
            MenuItem {
                text: "Neutral"
                visible: !stripTextDelegate.last
                onTriggered: {
                    model.typeOut = Ph.PhStripDetect.Unknown
                }
            }
            MenuItem {
                text: "Aperture"
                visible: !stripTextDelegate.last
                onTriggered: {
                    model.typeOut = Ph.PhStripDetect.Aperture
                }
            }
            MenuItem {
                text: "Bowl"
                visible: !stripTextDelegate.last
                onTriggered: {
                    model.typeOut = Ph.PhStripDetect.Bowl
                }
            }
            MenuItem {
                text: "Advance"
                visible: !stripTextDelegate.last
                onTriggered: {
                    model.typeOut = Ph.PhStripDetect.Advance
                }
            }
        }
    }

    // move 'shift' characters to the next item (or previous if 'shift' is negative)
    function shiftText(shift) {
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

            setText(text.substr(0,currentLength-shift))
            nextText.setText(textMoved + nextText.text)
        } else {
            var absoluteShift = -shift

            if (absoluteShift > nextLength - 1) {
                absoluteShift = nextLength - 1
            }

            var nextTextMoved = nextText.text.substr(0, absoluteShift)

            console.log(nextTextMoved + " - " + text + " - " + nextText.text)

            setText(text + nextTextMoved)
            nextText.setText(nextText.text.substr(absoluteShift,nextLength-absoluteShift))
        }
    }

    function giveFocusToNextItem() {
        console.log("I'm item " + index);
        var nextItem = textRepeater.itemAt(index + 1)
        nextItem.editTextAtPos(1)
    }

    function giveFocusToPreviousItem() {
        console.log("I'm item " + index);
        var previousItem = textRepeater.itemAt(index - 1)
        previousItem.editTextAtPos(previousItem.text.length - 1)
    }

    function editTextAt(x, y) {
        var newPos = stripTextInput.positionAt(x/stripTextItem2.width*stripTextInput.width, y)
        editTextAtPos(newPos)
    }

    function editTextAtPos(pos) {
        console.log("text.editTextAt " + pos)
        stripContainer.forceActiveFocus()
        stripTextInput.forceActiveFocus()
        stripLineContainer.editing = true
        stripTextInput.cursorPosition = pos
    }

    function positionAt(x) {
        return stripTextInput.positionAt(x/stripTextItem2.width*stripTextInput.width, 0)
    }

    function detach() {
        stripLineContainer.detachDetect(stripTextItem2.textIndex)
    }
}
