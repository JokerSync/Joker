import QtQuick 2.0

Item {
    width: 600
    height: 400

//    Item {
//        id: stripTexts
//        objectName: "stripTexts"
//        anchors.top: parent.top
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left
//        anchors.right: parent.right
//        clip: true
//        //contentX: clockTime / horizontalTimePerPixel

//        Repeater {
//            model: stripTextModel

//            // FIXME inverted colors are not implemented
//            delegate: Item {
//                //x: (model.modelData.timeIn - clockTime) / horizontalTimePerPixel + stripTexts.width / 6
//                x: model.modelData.timeIn / horizontalTimePerPixel + stripTexts.width / 6
//                y: model.modelData.y * stripTexts.height
//                transform: Scale { xScale: (model.modelData.timeOut - model.modelData.timeIn) / horizontalTimePerPixel / childrenRect.width;
//                                   yScale: model.modelData.height*stripTexts.height / childrenRect.height}

//                Text {
//                    text: model.modelData.content
//                    // FIXME color and font not implemented
//                    //color: model.modelData.color
//                    font.pixelSize: model.modelData.height*stripTexts.height
//                    font.family: "Arial"
//                    wrapMode: Text.WordWrap
//                    font.weight: Font.Black
//                }
//            }
//        }
//    }

    ListView {
        id: stripTexts
        objectName: "stripTexts"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        //clip: true
        contentX: clockTime / horizontalTimePerPixel
        //cacheBuffer: 1000
        //horizontalVelocity: 100000
        //layer.enabled: true

        model: stripTextModel

        // FIXME inverted colors are not implemented
        delegate: Item {
            //x: (model.modelData.timeIn - clockTime) / horizontalTimePerPixel + stripTexts.width / 6
            x: model.modelData.timeIn / horizontalTimePerPixel + stripTexts.width / 6
            y: model.modelData.y * stripTexts.height
            transform: Scale { xScale: (model.modelData.timeOut - model.modelData.timeIn) / horizontalTimePerPixel / childrenRect.width;
                               yScale: model.modelData.height*stripTexts.height / childrenRect.height}
            //layer.enabled: true

//            Rectangle {
//                color: "red"
//                width: 200
//                height: 20
//            }

            Text {
                text: model.modelData.content
                // FIXME color and font not implemented
                //color: model.modelData.color
                font.pixelSize: model.modelData.height*stripTexts.height
                font.family: "Arial"
                wrapMode: Text.WordWrap
                font.weight: Font.Black
                //layer.enabled: true
            }
        }
    }
}
