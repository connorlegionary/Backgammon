import QtQuick 2.15
import QtQuick.Layouts 1.1

import "../Controls"

OverlayBase {
    id: rollBase
    body.height: 150
    body.width: width - 22
    body.anchors.centerIn: rollBase
    text.text: qsTr("Won Game!")
    closeOnClick: false
    body.color: viewContext.white ? consts.lightPieceColor : consts.boardBackground
    body.border.color: viewContext.white ? consts.boardBackground : consts.lightPieceColor
    closeColor.color: viewContext.white ? consts.boardBackground : consts.lightPieceColor
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
                        text: (viewContext.white ? qsTr("White") : qsTr("Black")) + " won!";
                        color: viewContext.white ? consts.boardBackground : consts.lightPieceColor
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
                            closeOverlay();
//                            pushOverlay("qrc:/Overlays/Roll.qml", { gameId: viewContext.gameId, text: "Next game, Roll to see who goes first!"});
                        }
                    }
                }
            }
        }
    }
}
