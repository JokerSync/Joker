import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1
import PhImport 1.0 as Ph
import "qrc:/qml/colors.js" as Colors
import "qrc:/qml/symbols.js" as Symbols
import "qrc:/fonts/fontawesome.js" as FontAwesome

// FIXME color, font, inverted color are not implemented
Item {
    id: stripLineContainer
    objectName: "Line" // used to find children of type Line in line repeater
    width: duration/settings.horizontalTimePerPixel
    height: parent.height/4
    x: timeIn/settings.horizontalTimePerPixel
    y: parent.height*trackNumber

    Binding { target: model; property: "timeIn"; value: x*settings.horizontalTimePerPixel }
    Binding { target: model; property: "trackNumber"; value: y/stripContainer.height }

    property var lineModel: model
    property bool editing: false

    // Load the "FontAwesome" font for the detection icons.
    FontLoader {
        source: "qrc:/fonts/fontawesome-webfont.ttf"
    }

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
                property bool last: index === textRepeater.count - 1

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
        color: Colors.colorFromDetectType(typeIn)
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.bottom
        anchors.verticalCenterOffset: -4
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
            text: Symbols.symbolFromDetectType(typeIn)
        }

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

        MouseArea {
            enabled: window.edition
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: {
                leftHandleContextMenu.popup();
            }
        }
    }

    Menu {
        id: leftHandleContextMenu
        title: "Edit"
        MenuItem {
            text: FontAwesome.Icon.trash + " Delete phrase"
            onTriggered: {
                console.log("Line " + model.index);
                doc.lineModel.remove(model.index);
            }
        }
        Menu {
            title: "Change type"

            MenuItem {
                text: "Mouth opened"
                enabled: model.typeIn !== Ph.PhStripDetect.MouthOpen
                onTriggered: {
                    model.typeIn = Ph.PhStripDetect.MouthOpen
                }
            }
            MenuItem {
                text: "Mouth closed"
                enabled: model.typeIn !== Ph.PhStripDetect.MouthClosed
                onTriggered: {
                    model.typeIn = Ph.PhStripDetect.MouthClosed
                }
            }
        }
    }

    Menu {
        id: lineContextMenu
        title: "Edit"
        property int index: 0
        property double mouseX: 0

        MenuItem {
            text: FontAwesome.Icon.trash + " Delete phrase"
            onTriggered: {
                console.log("Line " + lineContextMenu.index + " " + textRow.width);
                doc.lineModel.remove(lineContextMenu.index);
            }
        }

        Menu {
            title: "Add detect"

            MenuItem {
                text: "Labial"
                shortcut: "4"
                onTriggered: {
                    var time = lineContextMenu.mouseX * settings.horizontalTimePerPixel;
                    console.log("add detect " + time);
                    lineModel.unlinkedDetects.add(time, Ph.PhStripDetect.Labial)
                }
            }
            MenuItem {
                text: "Dental"
                shortcut: "5"
                onTriggered: {
                    var time = lineContextMenu.mouseX * settings.horizontalTimePerPixel;
                    console.log("add detect " + time);
                    lineModel.unlinkedDetects.add(time, Ph.PhStripDetect.Dental)
                }
            }
            MenuItem {
                text: "Neutral"
                shortcut: "6"
                onTriggered: {
                    var time = lineContextMenu.mouseX * settings.horizontalTimePerPixel;
                    console.log("add detect " + time);
                    lineModel.unlinkedDetects.add(time, Ph.PhStripDetect.Unknown)
                }
            }
            MenuItem {
                text: "Aperture"
                shortcut: "7"
                onTriggered: {
                    var time = lineContextMenu.mouseX * settings.horizontalTimePerPixel;
                    console.log("add detect " + time);
                    lineModel.unlinkedDetects.add(time, Ph.PhStripDetect.Aperture)
                }
            }
            MenuItem {
                text: "Bowl"
                shortcut: "8"
                onTriggered: {
                    var time = lineContextMenu.mouseX * settings.horizontalTimePerPixel;
                    console.log("add detect " + time);
                    lineModel.unlinkedDetects.add(time, Ph.PhStripDetect.Bowl)
                }
            }
            MenuItem {
                text: "Advance"
                shortcut: "9"
                onTriggered: {
                    var time = lineContextMenu.mouseX * settings.horizontalTimePerPixel;
                    console.log("add detect " + time);
                    lineModel.unlinkedDetects.add(time, Ph.PhStripDetect.Advance)
                }
            }
        }
    }

    function editTextAt(x, y) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y

        if (lineY !== 0 || lineX < -0.1 || lineX > stripLineContainer.width + 0.1) {
            return false;
        }

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

    function addDetectAt(x, y, type) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y
        var lineWidth = duration/settings.horizontalTimePerPixel

        if (lineX > 0 && lineX < lineWidth && lineY >= 0 && lineY < stripLineContainer.height) {
            var time = lineX * settings.horizontalTimePerPixel
            console.log("line.addDetectAt adding detect " + time + " " + lineX + " " + lineY + " " + lineWidth + " " + type)
            stripLineContainer.lineModel.unlinkedDetects.add(time, type)
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

        if (lineY !== 0 || lineX < -0.5 || lineX > stripLineContainer.width + 0.5) {
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

                if (textRepeater.count > i+1) {
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

        if (lineY !== 0 || lineX <= 0 || lineX >= stripLineContainer.width) {
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
                attachDetect(j, detect.x)
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

    function toggleAttachDetectAt(x, y) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y

        if (lineY !== 0 || lineX <= 0 || lineX >= stripLineContainer.width) {
            return false;
        }

        // is it an unlinked detect time?
        var u = detectRepeater.count;
        for (var j = 0; j < detectRepeater.count; ++j) {
            var detect = detectRepeater.itemAt(j);
            console.log(detect.x + " " + lineX)
            if (Math.abs(detect.x - lineX) < 1) {
                console.log("attaching detect time " + stripLineContainer.x)
                attachDetect(j, detect.x)
                return true;
            }
        }

        // is it a text timeOut?
        for (var i = 0; i < textRow.children.length; ++i) {
            var text = textRow.children[i];
            console.log(text.x + " " + text.width + " " + lineX)
            if (Math.abs(text.x + text.width - lineX) < 1) {
                console.log("detaching text timeOut " + stripLineContainer.x)
                detachDetect(i)
                return true;
            }
        }

        return false;
    }

    function isAt(x, y) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y

        if (lineY !== 0 || lineX < 0 || lineX > stripLineContainer.width) {
            return false;
        }

        return true;
    }

    function deleteDetectAt(x, y) {
        var lineX = x - stripLineContainer.x
        var lineY = y - stripLineContainer.y

        if (lineY !== 0 || lineX <= 0 || lineX >= stripLineContainer.width) {
            return false;
        }

        // is it a text timeOut?
        // exclude the last text
        for (var i = 0; i < textRepeater.count - 1; ++i) {
            var text = textRepeater.itemAt(i);
            console.log("textOut " + text.x + " " + text.width + " " + lineX)
            if (Math.abs(text.x + text.width - lineX) < 1) {
                // detach this timeout
                // it will appear as an unlinked detect in the next loop
                console.log("detaching text timeOut " + stripLineContainer.x)
                stripLineContainer.detachDetect(i)
                break;
            }
        }

        // is it an unlinked detect time?
        for (var j = 0; j < detectRepeater.count; ++j) {
            var detect = detectRepeater.itemAt(j);
            console.log("detect " + detect.x + " " + lineX)
            if (Math.abs(detect.x - lineX) < 1) {
                console.log("deleting detect time " + stripLineContainer.x)
                stripLineContainer.lineModel.unlinkedDetects.remove(j)
                return true;
            }
        }

        return false;
    }

    function attachDetect(detectIndex, detectX) {
        var detect = detectRepeater.itemAt(detectIndex);

        // find the text below
        var text = textRow.childAt(detectX, 0);
        if (text) {
            console.log("split found text")

            // find the position and split in 2
            var pos = text.positionAt(detectX - text.x)
            // add the new text
            var splitTime = (detectX - text.x) * settings.horizontalTimePerPixel

            console.log("split " + detectIndex + " " + detectX + " " + text.x + " " + text.textIndex + " " + pos + " " + splitTime + " " + detect.modelType)

            stripLineContainer.lineModel.texts.split(text.textIndex, pos, splitTime, detect.modelType)
        }

        // remove the detect
        stripLineContainer.lineModel.unlinkedDetects.remove(detectIndex)
    }

    function detachDetect(textIndex) {
        var text = textRepeater.itemAt(textIndex)
        var nextText = textRepeater.itemAt(textIndex+1)
        var firstTypeOut = text.modelTypeOut;

        var time = nextText.x * settings.horizontalTimePerPixel

        console.log("merge " + textIndex + " " + time + " " + firstTypeOut)

        stripLineContainer.lineModel.texts.merge(textIndex)

        // add a new unlinked detect where the previous text ended
        stripLineContainer.lineModel.unlinkedDetects.add(time, firstTypeOut)
    }

    function showContextMenu(mouseX, textIndex) {
        var text = textRow.children[textIndex];

        console.log("line index: " + model.index + ", text index: " + textIndex + " text.x: " + text.x + " mouseX:" + mouseX);

        lineContextMenu.index = model.index;
        lineContextMenu.mouseX = mouseX + text.x;
        lineContextMenu.popup();
    }
}

