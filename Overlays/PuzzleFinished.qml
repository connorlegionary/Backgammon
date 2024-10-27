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
                text: qsTr("Puzzle Finished")
                color: consts.lightPieceColor
                font.pixelSize: 33
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    anchors.centerIn: parent
                    text: qsTr("Score: " + viewContext.score)
                    color: consts.white
                    font.pixelSize: 30
                    font.bold: true
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
                    text: qsTr("Finished")
                    font.pixelSize: 33
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        popView();
                        closeOverlay();
                    }
                }
            }
        }
    }
}
