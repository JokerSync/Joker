import QtQuick 2.0

Item {
    id: lineDetect
    Binding { target: model; property: "time"; value: x*horizontalTimePerPixel }

    Rectangle {
        id: detectRectangle
        color: "#8000FFFF"
        width: 18
        height: 18
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        Drag.keys: "Ctrl"

        MouseArea {
            id: detectMouseArea
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
                    var movement = mouseX - startX;
                    if (mouse.modifiers & Qt.ControlModifier) {
                        // snap x to whole frame
                        console.log("detect dragged with ctrl " + movement)
                        lineDetect.x = lineDetect.x + snapToFrame(movement)
                    } else {
                        console.log("detect dragged without ctrl")
                        var shift = Math.round(movement/10);
                        console.log("Shift is " + shift + ", I'm item " + lineDetect.lineIndex);
                        var previousText = textRepeater.itemAt(lineDetect.lineIndex)
                        var nextText = textRepeater.itemAt(lineDetect.lineIndex+1)
                        var previousLength = previousText.text.length
                        var nextLength = nextText.text.length

                        if (shift > nextLength - 1) {
                            shift = nextLength - 1
                        }
                        if (shift > -previousLength + 1) {
                            shift = -previousLength +1
                        }

                        if (shift > 0) {
                            var previousTextMoved = previousText.text.substr(previousLength-shift-1,shift)
                            previousText.text = previousText.text.substr(0,previousLength-shift)
                            nextText.text = previousTextMoved + nextText.text
                        } else {
                            var nextTextMoved = nextText.text.substr(0, shift)
                            previousText.text = previousText.text + nextTextMoved
                            nextText.text = nextText.text.substr(shift,nextLength-shift)
                        }

                        // reset position
                        lineDetect.x = nextText.x
                    }
                }
            }
        }
    }
}
