import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

// FIXME color, font, inverted color are not implemented
Item {
    id: stripLineContainer
    objectName: "Line" // used to find children of type Line in line repeater
    width: 1 // width is arbitrary
    height: parent.height/4
    x: timeIn/horizontalTimePerPixel
    y: parent.height*trackNumber

    Binding { target: model; property: "timeIn"; value: x*horizontalTimePerPixel }
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
        width: (doc.timeOut - timeIn)/horizontalTimePerPixel
        visible: textRepeater.count === 0
        color: "#80ff0000"
    }

    Row {
        id: textRow
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
    }

    Repeater {
        id: detectRepeater
        model: lineModel.unlinkedDetects
        delegate: Detect {
            x: time/horizontalTimePerPixel
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
                var time = lineContextMenu.mouseX * horizontalTimePerPixel;
                console.log("add detect " + time);
                lineModel.unlinkedDetects.add(time)
            }
        }
    }

    function xOut() {
        var xOut = stripLineContainer.x
        for (var i = 0; i < textRepeater.children.length; ++i) {
            var item = textRepeater.children[i];
            if (item.x + item.width > xOut) {
                xOut = item.x + item.width
            }
        }

        return xOut;
    }

    function editTextAt(x, y) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y
        var text = textRow.childAt(lineX, lineY);
        if (text) {
            console.log("line.stripTextAt found text")
            console.log(text)
            var textX = lineX - text.x
            var textY = lineY - text.y
            text.editTextAt(textX, textY)
            return true;
        }
        return false;
    }

    function moveDetectAt(x, y, frameChange) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y
        var pixelPerFrame = jokerWindow.timePerFrame / horizontalTimePerPixel
        var pixelChange = frameChange * pixelPerFrame
        var timeChange = frameChange * jokerWindow.timePerFrame

        if (lineY !== y) {
            return false;
        }

        // is it the line timeIn?
        if (lineX === 0) {
            console.log("moving timeIn " + stripLineContainer.x + " " + frameChange + " " + pixelPerFrame + " " + timeChange)
            timeIn += timeChange;
            textRepeater.itemAt(0).width = textRepeater.itemAt(0).width - pixelChange
            return true;
        }

        // is it a text timeOut?
        for (var i = 0; i < textRow.children.length; ++i) {
            var text = textRow.children[i];
            console.log(text.x + " " + text.width + " " + lineX)
            if (Math.abs(text.x + text.width - lineX) < 1) {
                console.log("moving text timeOut " + stripLineContainer.x + " " + frameChange + " " + pixelPerFrame + " " + timeChange)
                text.width += pixelChange
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

    function showContextMenu(mouseX) {
        console.log("index: " + model.index);
        lineContextMenu.index = model.index;
        lineContextMenu.mouseX = mouseX;
        lineContextMenu.popup();
    }
}

