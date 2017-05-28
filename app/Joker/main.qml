/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

import QtQuick 2.5
import QtQml 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import PhImport 1.0
import "qrc:/fonts/fontawesome.js" as FontAwesome

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
            visible: !settings.hideStrip
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
        anchors.fill: parent
        opacity: 0 // hide by default
        visible: false
    }

    Rectangle {
        id: savingItem
        anchors.fill: parent
        color: "black"
        visible: false
        opacity: 0
        property bool saving: jokerWindow.saving

        Text {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            styleColor: "white"
            font.bold: true
            font.pixelSize: 150
            font.family: "FontAwesome"
            text: FontAwesome.Icon.save
        }

        onSavingChanged: {
            console.log(saving)
            if(jokerWindow.saving) {
                visible = true
                opacity = 0.6
            } else {
                animateOpacity.start()
            }
        }

        NumberAnimation {
            id: animateOpacity
            target: savingItem
            properties: "opacity"
            from: 0.6
            to: 0
            duration: 1000
            easing.type: Easing.InCubic
       }
    }

    property bool edition: editionSwitch.edition

    Shortcuts {}

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

