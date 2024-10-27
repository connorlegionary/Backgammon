import QtQuick 2.15
import QtQuick.Layouts 1.1

import "../Controls"

OverlayBase {
    body.height: 250
    body.width: parent.width/3
    text.text: viewContext.title
    body.x: (width - body.width)/2
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
                        text: viewContext.text
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        font.bold: true
                    }
                }
            }
        }
    }
}
