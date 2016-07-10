import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

// FIXME color, font, inverted color are not implemented
Item {
    id: stripLineContainer
    objectName: "Line" // used to find children of type Line in line repeater
    width: 1 // width is arbitrary
    height: parent.height/4
    x: timeIn/settings.horizontalTimePerPixel
    y: parent.height*trackNumber

    Binding { target: model; property: "timeIn"; value: x*settings.horizontalTimePerPixel }
    Binding { target: model; property: "trackNumber"; value: y/stripContainer.height }

    property var lineModel: model
    property bool editing: false

    onEditingChanged: {
        stripContainer.editing = editing
    }

    // appears when we have just added the opening sign
    Rectangle {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: Math.max(videoEngine.timeOut, doc.timeOut) /settings.horizontalTimePerPixel - stripLineContainer.x
        visible: textRepeater.count === 0 && window.edition
        color: "#80ff0000"
    }

    Row {
        id: textRow
        Repeater {
            id: textRepeater
            model: lineModel.texts
            delegate: StripText {
                id: stripTextDelegate
                // reversed stack order so that out detect are on top
                z: -index

                property int textIndex: index

                Binding { target: model; property: "content"; value: text }
                Binding { target: model; property: "duration"; value: width*settings.horizontalTimePerPixel }
            }
        }
    }

    // people name
    // FIXME selection, color, font, inverted color are not implemented
    Text {
        anchors.right: parent.left
        anchors.rightMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 2
        text: peopleName
        font.pixelSize: stripLineContainer.height/3
        font.family: "Arial"
        color: "blue"
        smooth: true // smooth scaling

        MouseArea {
            enabled: window.edition
            anchors.fill: parent
            acceptedButtons: "LeftButton"
            onDoubleClicked: {
                peopleConnection.isEnabled = true
                peopleSelection.show(peopleName)
            }
        }

        Connections {
            id: peopleConnection
            property bool isEnabled: false
            target: peopleSelection
            onSelected: {
                if (isEnabled) {
                    console.log("onSelected " + name + " " + index)
                    doc.assignLineToPeople(index, name)
                    isEnabled = false
                }
            }
        }
    }

    Repeater {
        id: detectRepeater
        model: lineModel.unlinkedDetects
        delegate: Detect {
            x: time/settings.horizontalTimePerPixel
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            property int lineIndex: index
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
        visible: window.edition

        MouseArea {
            anchors.fill: parent
            drag{ target: parent; axis: Drag.XAxis }

            onMouseXChanged: {
                if(drag.active){
                    console.log("timePerFrame: ", jokerWindow.timePerFrame);
                    var pixelChange = snapToFrame(mouseX)
                    var pixelPerFrame = jokerWindow.timePerFrame / settings.horizontalTimePerPixel
                    if (pixelChange > textRepeater.itemAt(0).width - pixelPerFrame) {
                        pixelChange = textRepeater.itemAt(0).width - pixelPerFrame
                    }
                    stripLineContainer.x = stripLineContainer.x + pixelChange
                    textRepeater.itemAt(0).width = textRepeater.itemAt(0).width - pixelChange
                }
            }
        }
    }

    Menu {
        id: lineContextMenu
        title: "Edit"
        property int index: 0
        property int mouseX: 0
        MenuItem {
            text: "Delete phrase"
            onTriggered: {
                console.log("Line " + lineContextMenu.index + " " + textRow.width);
                doc.lineModel.remove(lineContextMenu.index);
            }
        }
        MenuItem {
            text: "Add detect"
            onTriggered: {
                var time = lineContextMenu.mouseX * settings.horizontalTimePerPixel;
                console.log("add detect " + time);
                lineModel.unlinkedDetects.add(time)
            }
        }
    }

    function editTextAt(x, y) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y

        if (lineY !== 0) {
            return false
        }

        // is it a text timeOut?
        for (var i = 0; i < textRow.children.length; ++i) {
            var text = textRow.children[i];
            console.log(text.x + " " + text.width + " " + lineX)
            if (lineX >= text.x - 0.1 && lineX <= text.x + text.width + 0.1) {
                console.log("line.stripTextAt found text")
                console.log(text)
                var textX = lineX - text.x
                var textY = lineY - text.y
                text.editTextAt(textX, textY)
                return true;
            }
        }

        return false;
    }

    function addDetectAt(x, y) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y
        var lineWidth = duration/settings.horizontalTimePerPixel

        if (lineX > 0 && lineX < lineWidth && lineY >= 0 && lineY < stripLineContainer.height) {
            var time = lineX * settings.horizontalTimePerPixel
            console.log("line.addDetectAt adding detect " + time + " " + lineX + " " + lineY + " " + lineWidth)
            stripLineContainer.lineModel.unlinkedDetects.add(time)
            return true;
        }
        return false;
    }

    function moveDetectAt(x, y, frameChange) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y
        var pixelPerFrame = jokerWindow.timePerFrame / settings.horizontalTimePerPixel
        var pixelChange = frameChange * pixelPerFrame
        var timeChange = frameChange * jokerWindow.timePerFrame

        if (lineY !== 0) {
            return false;
        }

        // is it the line timeIn?
        if (lineX === 0) {
            console.log("moving timeIn " + stripLineContainer.x + " " + frameChange + " " + pixelPerFrame + " " + timeChange)
            timeIn += timeChange;
            if (textRepeater.count > 0) {
                textRepeater.itemAt(0).width = textRepeater.itemAt(0).width - pixelChange
            }
            return true;
        }

        // is it a text timeOut?
        for (var i = 0; i < textRow.children.length; ++i) {
            var text = textRow.children[i];
            console.log(text.x + " " + text.width + " " + lineX)
            if (Math.abs(text.x + text.width - lineX) < 1) {
                console.log("moving text timeOut " + stripLineContainer.x + " " + frameChange + " " + pixelPerFrame + " " + timeChange)
                text.width += pixelChange

                if (textRepeater.count > i) {
                    var nextText = textRow.children[i+1]
                    nextText.width -= pixelChange
                }

                return true;
            }
        }

        // is it an unlinked detect time?
        var u = detectRepeater.count;
        for (var j = 0; j < detectRepeater.count; ++j) {
            var detect = detectRepeater.itemAt(j);
            console.log(detect.x + " " + lineX)
            if (Math.abs(detect.x - lineX) < 1) {
                console.log("moving detect time " + stripLineContainer.x + " " + frameChange + " " + pixelPerFrame + " " + timeChange)
                detect.x += pixelChange
                return true;
            }
        }

        return false;
    }

    function shiftDetectAt(x, y, shift) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y

        console.log(lineY)

        if (lineY !== 0) {
            return false;
        }

        // is it an unlinked detect time?
        var u = detectRepeater.count;
        for (var j = 0; j < detectRepeater.count; ++j) {
            var detect = detectRepeater.itemAt(j);
            console.log(detect.x + " " + lineX)
            if (Math.abs(detect.x - lineX) < 1) {
                console.log("attaching detect time " + stripLineContainer.x + " " + shift)
                // this will attach the detect and it will be found as a time out in the next loop
                attachDetect(j)
                break;
            }
        }

        // is it a text timeOut?
        for (var i = 0; i < textRow.children.length; ++i) {
            var text = textRow.children[i];
            console.log(text.x + " " + text.width + " " + lineX)
            if (Math.abs(text.x + text.width - lineX) < 1) {
                console.log("shifting text timeOut " + stripLineContainer.x + " " + shift)
                text.shiftText(shift)
                return true;
            }
        }

        return false;
    }

    function attachDetect(detectIndex, detectX) {
        // find the text below
        var text = textRow.childAt(detectX, 0);
        if (text) {
            console.log("split found text")

            // find the position and split in 2
            var pos = text.positionAt(detectX - text.x)
            // add the new text
            var splitTime = (detectX - text.x) * settings.horizontalTimePerPixel

            console.log("split " + detectIndex + " " + detectX + " " + text.x + " " + text.textIndex + " " + pos + " " + splitTime)

            stripLineContainer.lineModel.texts.split(text.textIndex, pos, splitTime)
        }

        // remove the detect
        stripLineContainer.lineModel.unlinkedDetects.remove(detectIndex)
    }

    function detachDetect(textIndex) {
        var text = textRepeater.itemAt(textIndex)
        var nextText = textRepeater.itemAt(textIndex+1)

        var time = nextText.x * settings.horizontalTimePerPixel

        console.log("merge " + textIndex + " " + time)

        stripLineContainer.lineModel.texts.merge(textIndex)

        // add a new unlinked detect where the previous text ended
        stripLineContainer.lineModel.unlinkedDetects.add(time)
    }

    function showContextMenu(mouseX) {
        console.log("index: " + model.index);
        lineContextMenu.index = model.index;
        lineContextMenu.mouseX = mouseX;
        lineContextMenu.popup();
    }
}

