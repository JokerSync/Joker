/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

import QtQuick 2.5
import QtQml 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

//ApplicationWindow {
Item {
    id: window
//    title: "Test Window with color " + color
    width: 800
    height: 600

//    visible: true

//    menuBar: MenuBar {
//        Menu {
//            title: "File"
//            MenuItem {
//                text: "New"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuSeparator { }
//            MenuItem {
//                text: "Open"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Open Recent"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuSeparator { }
//            MenuItem {
//                text: "Open Video"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Close Video"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuSeparator { }
//            MenuItem {
//                text: "Save"
//                shortcut: "Ctrl+S"
//                onTriggered: jokerWindow.on_actionSave_triggered()
//            }
//            MenuItem {
//                text: "Save As"
//                shortcut: "Ctrl+Shift+S"
//                onTriggered: jokerWindow.on_actionSave_as_triggered()
//            }
//            MenuSeparator { }
//            MenuItem {
//                text: "Preferences"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Properties"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//        }
//        Menu {
//            title: "Control"
//            MenuItem {
//                text: "Play / Pause"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Play backward"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Step forward"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Step backward"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Time forward"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Time backward"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Speed"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }

//            //              <widget class="QMenu" name="menuVarial_Speed">
//            //               <addaction name="action_3"/>
//            //               <addaction name="action_1"/>
//            //               <addaction name="action_0_5"/>
//            //               <addaction name="action0"/>
//            //               <addaction name="action0_5"/>
//            //               <addaction name="action1"/>
//            //               <addaction name="action3"/>
//            //              </widget>

//            MenuItem {
//                text: "Change timestamp"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Go to"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }

//            //              <widget class="QMenu" name="menuGo_to">
//            //               <addaction name="actionTimecode"/>
//            //               <addaction name="actionNext_element"/>
//            //               <addaction name="actionPrevious_element"/>
//            //               <addaction name="actionNext_loop"/>
//            //               <addaction name="actionPrevious_loop"/>
//            //              </widget>

//            MenuSeparator { }
//            MenuItem {
//                text: "Set timecode In"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Set timecode Out"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//            MenuItem {
//                text: "Loop"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }
//        }
//        Menu {
//            title: "Rythmo"
//            MenuItem {
//                text: "Cut"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }

//            //              <addaction name="actionSelect_character"/>
//            //              <addaction name="actionHide_selected_peoples"/>
//            //              <addaction name="separator"/>
//            //              <addaction name="actionHide_the_rythmo"/>
//            //              <addaction name="actionInvert_colors"/>
//            //              <addaction name="actionDisplay_the_cuts"/>
//            //              <addaction name="actionDisplay_the_vertical_scale"/>
//            //              <addaction name="separator"/>
//            //              <addaction name="actionDisplay_feet"/>
//            //              <addaction name="actionSet_first_foot_timecode"/>
//            //              <addaction name="actionSet_distance_between_two_feet"/>
//        }
//        Menu {
//            title: "Video"
//            MenuItem {
//                text: "Cut"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }

//            //              <addaction name="actionForce_16_9_ratio"/>
//            //              <addaction name="actionDeinterlace_video"/>
//            //              <addaction name="actionUse_native_video_size"/>

//        }
//        Menu {
//            title: "View"
//            MenuItem {
//                text: "Cut"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }

//            //              <addaction name="actionFullscreen"/>
//            //              <addaction name="actionDisplay_the_control_panel"/>
//            //              <addaction name="actionDisplay_the_information_panel"/>
//            //              <addaction name="separator"/>
//        }
//        Menu {
//            title: "Help"
//            MenuItem {
//                text: "Cut"
//                //shortcut: "Ctrl+X"
//                //onTriggered: ...
//            }

//            //              <addaction name="actionSend_feedback"/>
//            //              <addaction name="actionAbout"/>
//        }
//    }

    // must be defined *before* the MediaPanel so that it ends up *below* in the z-stack
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: {
            if (containsMouse) {
                videoOverlay.showPanel()
            }
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Vertical

        Video {
            id: videoOverlay
            Layout.fillHeight: true
            Layout.minimumHeight: 50

            MediaPanel {
                id: mediaPanel
                anchors.left: videoOverlay.left
                anchors.bottom: videoOverlay.bottom
                anchors.right: videoOverlay.right
            }

            EditionSwitch {
                id: editionSwitch
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                visible: mediaPanel.visible
                opacity: mediaPanel.opacity
            }

            Timer {
                id: panelTimer
                interval: 3000
                onTriggered: {
                    mediaPanel.opacity = 0
                }
            }

            function showPanel() {
                panelTimer.restart()
                mediaPanel.opacity = 1
            }

            function showPanelPermanent() {
                panelTimer.stop()
                mediaPanel.opacity = 1
            }
        }

        Strip {
            id: strip
            objectName: "strip"
            // TODO save and restore height on startup
            height: 200
            Layout.minimumHeight: 50
        }

        // define a cutom delegate of height 2
        // otherwise Qt defines a large mousearea by default
        // that will block the mediapanel input
        handleDelegate: Rectangle {
            width: parent.width
            height: 2
            color: Qt.darker(pal.window, 1.5)
        }
    }

    SystemPalette { id: pal }

    PeopleSelection {
        id: peopleSelection
        anchors.centerIn: parent
        opacity: 0 // hide by default
        visible: false
    }

    property bool edition: editionSwitch.edition

    Shortcut {
        sequence: "Alt+Left"
        context: Qt.ApplicationShortcut
        onActivated: {
            pause()
            playbackController.onPreviousFrame()
        }
    }

    Shortcut {
        sequence: "Alt+Right"
        context: Qt.ApplicationShortcut
        onActivated: {
            pause()
            playbackController.onNextFrame()
        }
    }

    Shortcut {
        sequence: "Left"
        context: Qt.ApplicationShortcut
        onActivated: {
            pause()
            jokerWindow.on_actionPrevious_element_triggered()
        }
    }

    Shortcut {
        sequence: "Right"
        context: Qt.ApplicationShortcut
        onActivated: {
            pause()
            jokerWindow.on_actionNext_element_triggered()
        }
    }

    Shortcut {
        sequence: "Up"
        context: Qt.ApplicationShortcut
        onActivated: playbackController.onPlayPause()
    }

    Shortcut {
        sequence: "Down"
        context: Qt.ApplicationShortcut
        onActivated: playbackController.onPlayPauseBackward()
    }

    Shortcut {
        sequence: "Space"
        context: Qt.ApplicationShortcut
        onActivated: playbackController.onPlayPause()
    }

    Shortcut {
        sequence: "Return"
        enabled: window.edition && !strip.editing && !peopleSelection.editing // do not steal the TextInputs event processing
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel

            console.log("Return shortcut " + time + " " + textX + " " + textY);
            pause()

            // if there is a line below, this should start editing it
            var success = strip.editTextAt(textX, textY);

            if (!success) {
                // if there is none, this adds a line
                console.log("Add line " + time + " " + (strip.currentTrackNumber / 4));
                doc.addLine(time, strip.currentTrackNumber / 4);
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+Left"
        enabled: window.edition
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
        enabled: window.edition
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
        enabled: window.edition && !strip.editing && !peopleSelection.editing // do not steal the TextInputs event processing
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
        enabled: window.edition && !strip.editing && !peopleSelection.editing // do not steal the TextInputs event processing
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
        enabled: window.edition && !strip.editing && !peopleSelection.editing // do not steal the TextInputs event processing
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
        enabled: window.edition
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
        enabled: window.edition
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
        enabled: window.edition
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
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("2 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY);
        }
    }

    // start/end of a line with closed mouth
    Shortcut {
        sequence: "3"
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("3 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY);
        }
    }

    // labial
    Shortcut {
        sequence: "4"
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("4 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY);
        }
    }

    // dental
    Shortcut {
        sequence: "5"
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("5 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY);
        }
    }

    // neutral
    Shortcut {
        sequence: "6"
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("6 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY);
        }
    }

    // aperture
    Shortcut {
        sequence: "7"
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("7 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY);
        }
    }

    // bowl
    Shortcut {
        sequence: "8"
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("8 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY);
        }
    }

    // advance / "weque"
    Shortcut {
        sequence: "9"
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = strip.currentTextY
            var textX = time / settings.horizontalTimePerPixel
            console.log("9 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY);
        }
    }

    Shortcut {
        sequence: "Ctrl+Up"
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            console.log("Ctrl+Up shortcut " + strip.currentTrackNumber);
            strip.currentTrackNumber = mod(strip.currentTrackNumber - 1, 4)
        }
    }

    Shortcut {
        sequence: "Ctrl+Down"
        enabled: window.edition
        context: Qt.ApplicationShortcut
        onActivated: {
            console.log("Ctrl+Down shortcut " + strip.currentTrackNumber);
            strip.currentTrackNumber = mod(strip.currentTrackNumber + 1, 4)
        }
    }

    function mod(n, m) {
            return ((n % m) + m) % m;
    }

    function pause() {
        if (playbackController.rate !== 0) {
            playbackController.onPlayPause()
        }
    }

    function showPeopleSelection() {
        peopleSelection.show()
    }

    function focusOnStrip() {
        strip.forceActiveFocus()
    }

    function showVideoTimeInSelection() {
        videoTimeInSelection.show()
    }
}

