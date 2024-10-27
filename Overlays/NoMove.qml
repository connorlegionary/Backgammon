import QtQuick 2.15
import QtQuick.Layouts 1.1

import "../Controls"

OverlayBase {
    body.height: 250
    body.width: parent.width/3
    text.text: qsTr("No Move")
    closeOnClick: false

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
                        text: qsTr("No Moves")
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        font.bold: true
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
                            pushOverlay("qrc:/Overlays/TurnFinished.qml", {model: viewContext.model, id: viewContext.id})
                        }
                    }
                }
            }
        }
    }
}
