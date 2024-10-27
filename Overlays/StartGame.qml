import QtQuick 2.15
import QtQuick.Layouts 1.1

import "../Controls"

OverlayBase {
    body.height: 350
    text.text: qsTr("Start Game")

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

                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/images/play.png"
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            viewContext.model.startGame()
                            closeOverlay()
                        }
                    }
                }
            }
        }
    }
}
