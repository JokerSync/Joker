import QtQuick 2.5
import QtQml 2.2
import QtQuick.Controls 1.2
import PhImport 1.0

Item {
    // do not steal the TextInputs event processing
    property bool allShortcutEnabled: !strip.editing && !peopleSelection.editing
    property bool editionShortcutEnabled: window.edition && allShortcutEnabled

    Shortcut {
        sequence: "Alt+Left"
        enabled: allShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            pause()
            playbackController.onPreviousFrame()
        }
    }

    Shortcut {
        sequence: "Alt+Right"
        enabled: allShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            pause()
            playbackController.onNextFrame()
        }
    }

    Shortcut {
        sequence: "Left"
        enabled: allShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            pause()
            jokerWindow.on_actionPrevious_element_triggered()
        }
    }

    Shortcut {
        sequence: "Right"
        enabled: allShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            pause()
            jokerWindow.on_actionNext_element_triggered()
        }
    }

    Shortcut {
        sequence: "Up"
        enabled: allShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: playbackController.onPlayPause()
    }

    Shortcut {
        sequence: "Down"
        enabled: allShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: playbackController.onPlayPauseBackward()
    }

    Shortcut {
        sequence: "Space"
        enabled: allShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: playbackController.onPlayPause()
    }

    Shortcut {
        sequence: "Return"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel

            console.log("Return shortcut " + time + " " + textX + " " + textY);
            pause()

            strip.addOrEditOrClose(textX, textY, PhStripDetect.MouthOpen)
        }
    }

    Shortcut {
        sequence: "Ctrl+Left"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("Ctrl+Left shortcut " + time + " " + textX + " " + textY);

            // find the detect that is at the current time and move it one frame to the left
            var success = strip.moveDetectAt(textX, textY, -1);

            if (success) {
                playbackController.relativeTime -= jokerWindow.timePerFrame
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+Right"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("Ctrl+Right shortcut " + time + " " + textX + " " + textY);

            // find the detect that is at the current time and move it one frame to the left
            var success = strip.moveDetectAt(textX, textY, 1);

            if (success) {
                playbackController.relativeTime += jokerWindow.timePerFrame
            }
        }
    }

    Shortcut {
        sequence: "Shift+Left"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("Shift+Left shortcut " + time + " " + textX + " " + textY);

            // find the detect that is at the current time, attach it if it is detached, and shift the text
            strip.shiftDetectAt(textX, textY, -1);
        }
    }

    Shortcut {
        sequence: "Shift+Right"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("Shift+Right shortcut " + time + " " + textX + " " + textY);

            // find the detect that is at the current time, attach it if it is detached, and shift the text
            strip.shiftDetectAt(textX, textY, 1);
        }
    }

    Shortcut {
        sequence: "Del"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("Del " + time + " " + textX + " " + textY);

            // find the detect or the line that is at the current time, delete it
            strip.deleteAt(textX, textY);
        }
    }

    Shortcut {
        sequence: "Ctrl+D"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("Ctrl+D shortcut " + time + " " + textX + " " + textY);

            // find the detect that is at the current time, attach it if it is detached, or vice-versa
            strip.toggleAttachDetectAt(textX, textY);
        }
    }

    // add loop
    Shortcut {
        sequence: "0"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            console.log("0 shortcut " + time)

            // insert loop
            doc.loopModel.add(time)
        }
    }

    // add cut
    Shortcut {
        sequence: "1"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            console.log("1 shortcut " + time);

            // insert cut
            doc.cutModel.add(time)
        }
    }

    // start/end of a line with open mouth
    Shortcut {
        sequence: "2"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("2 shortcut " + time + " " + textX + " " + textY);

            strip.addOrEditOrClose(textX, textY, PhStripDetect.MouthOpen)
        }
    }

    // start/end of a line with closed mouth
    Shortcut {
        sequence: "3"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("3 shortcut " + time + " " + textX + " " + textY);

            strip.addOrEditOrClose(textX, textY, PhStripDetect.MouthClosed)
        }
    }

    // labial
    Shortcut {
        sequence: "4"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("4 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY, PhStripDetect.Labial);
        }
    }

    // dental
    Shortcut {
        sequence: "5"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("5 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY, PhStripDetect.Dental);
        }
    }

    // neutral
    Shortcut {
        sequence: "6"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("6 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY, PhStripDetect.Unknown);
        }
    }

    // aperture
    Shortcut {
        sequence: "7"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("7 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY, PhStripDetect.Aperture);
        }
    }

    // bowl
    Shortcut {
        sequence: "8"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("8 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY, PhStripDetect.Bowl);
        }
    }

    // advance / "weque"
    Shortcut {
        sequence: "9"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("9 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY, PhStripDetect.Advance);
        }
    }

    Shortcut {
        sequence: "Alt+Up"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            console.log("Alt+Up shortcut " + strip.currentTrackNumber);
            strip.currentTrackNumber = mod(strip.currentTrackNumber - 1, 4)
        }
    }

    Shortcut {
        sequence: "Alt+Down"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            console.log("Alt+Down shortcut " + strip.currentTrackNumber);
            strip.currentTrackNumber = mod(strip.currentTrackNumber + 1, 4)
        }
    }

    Shortcut {
        sequence: "Ctrl+Up"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            console.log("Ctrl+Up shortcut " + strip.currentTrackNumber);

            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            var line = strip.findLineAt(textX, textY)

            if (line) {
                var newTrackNumber = mod(strip.currentTrackNumber - 1, 4)
                line.moveToTrackNumber(newTrackNumber)
                strip.currentTrackNumber = newTrackNumber
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+Down"
        enabled: editionShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            console.log("Ctrl+Down shortcut " + strip.currentTrackNumber);

            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            var line = strip.findLineAt(textX, textY)

            if (line) {
                var newTrackNumber = mod(strip.currentTrackNumber + 1, 4)
                line.moveToTrackNumber(newTrackNumber)
                strip.currentTrackNumber = newTrackNumber
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+E"
        enabled: allShortcutEnabled
        context: Qt.ApplicationShortcut
        onActivated: {
            console.log("Ctrl+E shortcut " + window.edition);
            editionSwitch.toggle()
        }
    }
}
