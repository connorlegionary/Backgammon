import QtQuick 2.15
import QtQuick.Layouts 1.1

import "../Controls"

OverlayBase {
    id: rollBase
    body.height: 150
    body.width: width - 22
    body.anchors.centerIn: rollBase
    text.text: qsTr("Turn finished")

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
                        text: qsTr("Done with your turn?")
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Image {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/images/roll.png"
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Finished turn");
                            viewContext.model.userFinishedTurn();
                            closeOverlay();
                        }
                    }
                }
            }
        }
    }
}


//

//_usedMoves.clear();
