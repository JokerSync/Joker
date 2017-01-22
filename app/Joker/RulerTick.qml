import QtQuick 2.0

Item {
    property int rulerIndex: index

    Rectangle {
        color: settings.invertColor? "white":"#808080"
        anchors.top: parent.top
        width: 1000/settings.horizontalTimePerPixel
        x: -width/2
        height: parent.height/2
    }

    Rectangle {
        color: settings.invertColor? "white":"#808080"
        anchors.top: parent.top
        width: 1000/settings.horizontalTimePerPixel
        x: -width/2 + parent.width/2
        height: parent.height/2
    }

    // ruler disc
    Rectangle {
        width: 4000/settings.horizontalTimePerPixel
        height: width
        color: settings.invertColor? "white":"#808080"
        radius: width*0.5
        x: parent.width/2 - width/2
        y: parent.height/2 + width/4
    }

    Text {
        color: settings.invertColor? "white":"#808080"
        x: -width/2
        y: parent.height/2
        text: parent.rulerIndex
        font.pixelSize: parent.height/3
    }
}
