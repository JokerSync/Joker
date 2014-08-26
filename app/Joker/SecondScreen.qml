/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

import QtQuick 2.6
import QtQml 2.2
import QtMultimedia 5.0
import QtQuick.Controls 1.4

Item {
    id: secondWindow
    width: 800
    height: 600

    // black background
    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Image {
        objectName: "videoLogo"
        source: videoLogoUrl
        fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        visible: settings.displayLogo && (videoEngine.timeIn >= videoEngine.timeOut)
    }

    VideoOutput {
        id: secondVideoOutput
        source: secondScreenVideoSource
        anchors.fill: parent
    }
}

