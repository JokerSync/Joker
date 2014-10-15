import QtQuick 2.0

Item {
    width: 600
    height: 400

    Item {
        id: stripTexts
        objectName: "stripTexts"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        clip: true

        Repeater {
            model: stripTextModel

            // FIXME inverted colors are not implemented
            delegate: Item {
                x: (model.modelData.timeIn - jokerWindow.stripTime) / horizontalTimePerPixel + stripTexts.width / 6
                y: model.modelData.y * stripTexts.height
                transform: Scale { xScale: (model.modelData.timeOut - model.modelData.timeIn) / horizontalTimePerPixel / childrenRect.width;
                                   yScale: model.modelData.height*stripTexts.height / childrenRect.height}

                Text {
                    text: model.modelData.content
                    // FIXME color and font not implemented
                    //color: model.modelData.color
                    font.pixelSize: model.modelData.height*stripTexts.height
                    font.family: "Arial"
                    wrapMode: Text.WordWrap
                    font.weight: Font.Black
                }
            }
        }
    }
}
