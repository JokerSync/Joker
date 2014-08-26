import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2
import "qrc:/fonts/fontawesome.js" as FontAwesome

Item {
    id: root

    signal selected(string name)
    property bool editing: opacity > 0

    // Load the "FontAwesome" font for the monochrome icons.
    FontLoader {
        source: "qrc:/fonts/fontawesome-webfont.ttf"
    }

    Component {
        id: contactDelegate

        Rectangle {
            id: peopleRect
            color: "#aa000000"
            width: grid.cellWidth-2
            height: grid.cellHeight-2
            radius: 5

            Column {
                anchors.fill: parent
                Text {
                    font.family: "FontAwesome";
                    text: FontAwesome.Icon.user;
                    font.pixelSize: 80
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                TextInput {
                    id: nameInput
                    font.pixelSize: 20
                    text: name
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: true

                    onEditingFinished: {
                        console.log("Editing finished")
                        model.name = text
                        focusOnStrip()
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: "LeftButton"
                onDoubleClicked: {
                    root.selected(name)
                    hide()
                }
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: "RightButton"
                onClicked: {
                    console.log("index: " + model.index);
                    peopleContextMenu.index = model.index;
                    peopleContextMenu.popup();
                }
            }

            Menu {
                id: peopleContextMenu
                title: "Edit"
                property int index: 0
                MenuItem {
                    text: "Delete"
                    onTriggered: {
                        console.log("People " + peopleContextMenu.index);
                        var usages = doc.deletePeople(peopleContextMenu.index);
                        console.log("Usages " + usages)
                        if (usages > 0) {
                            messageDialog.open()
                        }
                    }
                }
                MenuItem {
                    text: "Rename"
                    onTriggered: {
                        nameInput.readOnly = false
                        nameInput.forceActiveFocus()
                    }
                }
            }

            MessageDialog {
                id: messageDialog
                visible: false
                title: "Invalid operation"
                text: "Cannot delete this role: it is being used."
                onAccepted: {
                    close()
                }
            }
        }
    }

    Row {
        anchors.centerIn: parent
        spacing: 2

        Rectangle {
            color: "#aa000000"
            width: grid.cellWidth-2
            height: grid.cellHeight-2
            radius: 5

            Column {
                anchors.fill: parent
                Text {
                    font.family: "FontAwesome";
                    text: FontAwesome.Icon.user_plus;
                    font.pixelSize: 80
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Text {
                    font.pixelSize: 20
                    text: "New role"
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: "LeftButton"
                onDoubleClicked: {
                    doc.peopleModel.add("New")
                }
            }
        }

        GridView {
            id: grid
            cellWidth: 120; cellHeight: 120
            width: 600
            height: 400

            model: doc.peopleModel
            delegate: contactDelegate
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            focus: true
        }
    }

    function hide() {
        opacity = 0
        visible = false
    }

    function show(peopleName) {
        grid.currentIndex = doc.peopleModel.indexOf(peopleName)
        opacity = 1
        visible = true
        forceActiveFocus()
    }
}
