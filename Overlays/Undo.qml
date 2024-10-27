import QtQuick 2.15
import QtQuick.Layouts 1.1

import "../Controls"

OverlayBase {
    id: rollBase
    body.height: 150
    body.width: width - 22
    body.anchors.centerIn: rollBase
    text.text: qsTr("Undid!")

    content.sourceComponent: Component {
        Item {

            Image {
                anchors.centerIn: parent
                width: 44
                height: 44
                source: "qrc:/images/undo.png"
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    closeOverlay()
                }
            }
        }
    }
}
