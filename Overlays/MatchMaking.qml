import QtQuick 2.15
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.15

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

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    anchors.centerIn: parent
                    text: qsTr("Searching for game")
                    color: consts.white
                    font.bold: true
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                BusyIndicator {
                    running: true
                    anchors.centerIn: parent
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle
                {
                    width: parent.width - 66
                    height: 44
                    anchors.centerIn: parent
                    radius: 11
                    color: consts.activeRed

                    Text {
                        anchors.fill: parent
                        color: consts.lightPieceColor
                        text: qsTr("Cancel")
                        font.pixelSize: 33
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            syncManager.leaveMatchmaking();
                            closeOverlay();
                        }
                    }
                }
            }
        }
    }
}
