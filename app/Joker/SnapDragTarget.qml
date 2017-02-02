import QtQuick 2.0

Item {
    property int lineHeight: 10
    property int maximumY: lineHeight*3
    property int snappedX: snapToFrame(x)
    property int snappedY: Math.round(y / lineHeight) * lineHeight

    signal dragged(int dragX, int dragY)

    onSnappedXChanged: {
        dragged(snappedX, snappedY)
    }

    onSnappedYChanged: {
        dragged(snappedX, snappedY)
    }

    function snapToFrame(pixelChange) {
       var pixelPerFrame = jokerWindow.timePerFrame / settings.horizontalTimePerPixel;
       return Math.round(pixelChange / pixelPerFrame) * pixelPerFrame;
    }
}
