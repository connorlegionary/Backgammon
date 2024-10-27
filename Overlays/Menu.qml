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

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                CustomButton {
                    text.text: qsTr("Resign Match")
                    width: 200
                    height: 66
                    anchors.centerIn: parent
                    onClicked: {
                        syncManager.resignMatch(viewContext.id)
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                CustomButton {
                    text.text: qsTr("Resign game")
                    width: 200
                    height: 66
                    anchors.centerIn: parent
                    onClicked: {
                        syncManager.resignGame(viewContext.id)
                        pushView("qrc:/Views/Home.qml")
                        closeOverlay();
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                CustomButton {
                    text.text: qsTr("Close Game")
                    width: 200
                    height: 66
                    anchors.centerIn: parent
                    onClicked: {
                        pushView("qrc:/Views/Home.qml")
                        closeOverlay();
                    }
                }
            }
        }
    }
}
