import QtQuick 2.15
import QtQuick.Layouts 1.1

import "qrc:/Controls"

Item {
    anchors.fill: parent

    Rectangle
    {
        anchors.fill: parent
        color: "black"
        opacity: 0.2
    }

    Rectangle
    {
        anchors.centerIn: parent
        width: parent.width*0.7
        height: parent.height * 0.8
        radius: 30
        border.width: 3
        border.color: consts.lightPieceColor
        color: consts.boardBackground

        ColumnLayout
        {
            anchors.fill: parent
            spacing: 0

            Text
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                text: qsTr("Bad move")
                color: consts.lightPieceColor
                font.pixelSize: 33
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Rectangle
            {
                Layout.preferredWidth: parent.width - 22
                Layout.preferredHeight: 66
                Layout.alignment: Qt.AlignHCenter
                radius: 11
                color: consts.darkPieceColor

//                NumberPicker {
//                    id: timePicker
//                    anchors.fill: parent
//                    EnterKey.type: Qt.EnterKeyDone
//                    model: [0,60000,300000,600000,3600000,86400000,604800000]
//                    showOff: true

//                    delegate: Component {
//                        Item {
//                            width: 66
//                            height: 66

//                            Text {
//                                anchors.centerIn: parent
//                                verticalAlignment: Qt.AlignVCenter
//                                horizontalAlignment: Qt.AlignHCenter
//                                text: timePicker.showOff && modelData === 0 ? "Off" : consts.msToTime(modelData)
//                                color: timePicker.currentIndex === index ? consts.activeRed : "white"
//                                font.pixelSize: timePicker.currentIndex === index ? 32 : 16
//                                font.bold: true
//                            }
//                        }
//                    }
//                }
            }

            Rectangle
            {
                Layout.preferredWidth: parent.width - 66
                Layout.preferredHeight: 44
                Layout.alignment: Qt.AlignHCenter
                radius: 11
                color: consts.activeRed

//                Text {
//                    anchors.fill: parent
//                    color: consts.lightPieceColor
//                    text: qsTr("Create")
//                    font.pixelSize: 33
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                }

//                MouseArea
//                {
//                    anchors.fill: parent
//                    onClicked:
//                    {
//                        syncManager.createGame(usernameText.text, scorePicker.model[scorePicker.currentIndex], timePicker.model[timePicker.currentIndex]/1000)
//                        closeOverlay();
//                    }
//                }

//                Connections {
//                    target: syncManager
//                    function onRequestFailed(messageType) {
//                        pushOverlay("qrc:/Overlays/WarningMessage.qml", { title: qsTr("User doesn't exist"), text: qsTr("Couldn't find a user by that name") })
//                    }
//                    function onRequestSuccess(messageType) {
////                        closeOverlay();
//                    }
//                }
            }
        }
    }
}
