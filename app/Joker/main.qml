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

    Rectangle {
        id: titleRect
        objectName: "titleRect"
        color: "#000080"
        height: childrenRect.height
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        visible: titleRectVisible

        Text {
            id: titleLabel
            color: "white"
            wrapMode: Text.WordWrap
            text: doc.fullTitle
            font.pointSize: 8
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
        }
    }

    SplitView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleRect.visible ? titleRect.bottom : parent.top
        anchors.bottom: parent.bottom

        orientation: Qt.Vertical

        Video {
            id: videoOverlay
            Layout.fillHeight: true
            Layout.minimumHeight: 50
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

        SystemPalette { id: pal }
    }

    Shortcut {
        sequence: "Alt+Left"
        context: Qt.ApplicationShortcut
        onActivated: playbackController.onPreviousFrame()
    }

    Shortcut {
        sequence: "Alt+Right"
        context: Qt.ApplicationShortcut
        onActivated: playbackController.onNextFrame()
    }

    Shortcut {
        sequence: "Left"
        context: Qt.ApplicationShortcut
        onActivated: jokerWindow.on_actionPrevious_element_triggered()
    }

    Shortcut {
        sequence: "Right"
        context: Qt.ApplicationShortcut
        onActivated: jokerWindow.on_actionNext_element_triggered()
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
        enabled: !strip.editing // do not steal the TextInputs event processing
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = 0;
            var textX = time / horizontalTimePerPixel
            console.log("Return shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should start editing it

            var success = strip.editTextAt(textX, textY);

            if (!success) {
                // if there is none, this adds a line
                doc.lineModel.add(time, textY);
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+Left"
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = 0;
            var textX = time / horizontalTimePerPixel
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
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = 0;
            var textX = time / horizontalTimePerPixel
            console.log("Ctrl+Right shortcut " + time + " " + textX + " " + textY);

            // find the detect that is at the current time and move it one frame to the left
            var success = strip.moveDetectAt(textX, textY, 1);

            if (success) {
                playbackController.relativeTime += jokerWindow.timePerFrame
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+Alt+Left"
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = 0;
            var textX = time / horizontalTimePerPixel
            console.log("Ctrl+Alt+Left shortcut " + time + " " + textX + " " + textY);

            // find the detect that is at the current time, attach it if it is detached, and shift the text
            strip.shiftDetectAt(textX, textY, -1);
        }
    }

    Shortcut {
        sequence: "Ctrl+Alt+Right"
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = 0;
            var textX = time / horizontalTimePerPixel
            console.log("Ctrl+Alt+Right shortcut " + time + " " + textX + " " + textY);

            // find the detect that is at the current time, attach it if it is detached, and shift the text
            strip.shiftDetectAt(textX, textY, 1);
        }
    }

    Shortcut {
        sequence: "0"
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = 0;
            var textX = time / horizontalTimePerPixel
            console.log("0 shortcut " + time + " " + textX + " " + textY);

            // insert loop
            doc.loopModel.add(time)
        }
    }

    Shortcut {
        sequence: "1"
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = 0;
            var textX = time / horizontalTimePerPixel
            console.log("1 shortcut " + time + " " + textX + " " + textY);

            // insert cut
            doc.cutModel.add(time)
        }
    }

    Shortcut {
        sequence: "4"
        context: Qt.ApplicationShortcut
        onActivated: {
            var time = jokerWindow.stripTime;
            var textY = 0;
            var textX = time / horizontalTimePerPixel
            console.log("4 shortcut " + time + " " + textX + " " + textY);

            // if there is a line below, this should add a detect to it
            var success = strip.addDetectAt(textX, textY);
        }
    }
}

