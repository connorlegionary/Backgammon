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
                text: qsTr("Delete Game? ")
                color: consts.lightPieceColor
                font.pixelSize: 33
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Image {
                    anchors.fill: parent
                    source: "qrc:/images/cancel.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle
            {
                Layout.preferredWidth: parent.width - 66
                Layout.preferredHeight: 44
                Layout.alignment: Qt.AlignHCenter
                radius: 11
                color: consts.activeRed

                Text {
                    anchors.fill: parent
                    color: consts.lightPieceColor
                    text: qsTr("Confirm Delete")
                    font.pixelSize: 33
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        viewContext.database.deleteGame(viewContext.id)
//                        pushView("qrc:/Views/Computer.qml", {id: gameId})
                        closeOverlay();
                    }
                }
            }
        }
    }
}
