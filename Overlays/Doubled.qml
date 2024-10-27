import QtQuick 2.15
import QtQuick.Layouts 1.1

import "../Controls"

OverlayBase {
    id: rollBase
    body.height: 150
    body.width: width - 22
    body.anchors.centerIn: rollBase
    text.text: qsTr("Doubled")

    content.sourceComponent: Component {
        Item {
            ColumnLayout
            {
                anchors.fill: parent
                anchors.bottomMargin: 16
                spacing: 16

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Text {
                        anchors.centerIn: parent
                        font.bold: true
                        font.pixelSize: 33
                        text: qsTr("Your opponent doubled, accept?")
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    CustomButton {
                        anchors.left: parent.left
                        anchors.leftMargin: 11
                        anchors.verticalCenter: parent.verticalCenter
                        width: 120
                        height: 56
                        text.text: qsTr("Decline")
                        onClicked:  {
                            syncManager.doubleResponse(viewContext.gameId, false);
                        }
                    }

                    CustomButton {
                        anchors.right: parent.right
                        anchors.rightMargin: 11
                        anchors.verticalCenter: parent.verticalCenter
                        width: 120
                        height: 56
                        text.text: qsTr("Accept")
                        onClicked:  {
                            syncManager.doubleResponse(viewContext.gameId, true);
                        }
                    }
                }
            }
        }
    }
}
