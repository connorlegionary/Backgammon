import QtQuick 2.15
import QtQuick.Layouts 1.0
import Qt5Compat.GraphicalEffects

import "qrc:/Controls"

Item {
    property alias text: titleText
    property alias content: source
    property alias body: bodyArea
    property alias close: closeButton
    property alias closeColor: closeButtonColor
    property var closeOnClick: true
    signal closeClicked();

    Rectangle {
        id: bodyArea
        width: parent.width - 16
        height: 500
        radius: 32
        color: consts.boardBackground
        border.width: 3
        border.color: consts.lightPieceColor
        x: 8

        ColumnLayout
        {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            spacing: 0

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 56

                Text {
                    id: titleText
                    anchors.left: parent.left
                    anchors.right: closeButton.left
                    anchors.rightMargin: 5
                    fontSizeMode: Text.Fit
                    anchors.verticalCenter: parent.verticalCenter
                    color: consts.lightPieceColor
                    font.bold: true
                    font.italic: true
                }

                Image {
                    id: closeButton
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: 40
                    height: 40
                    source: "qrc:/images/cancel.png"

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            if (closeOnClick)
                            {
                                closeClicked()
                                closeOverlay()
                            }
                        }
                    }
                    ColorOverlay {
                        id: closeButtonColor
                        visible: true
                        anchors.fill: closeButton
                        source: closeButton
                        color: "red"
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Loader{
                    id: source
                    anchors.fill: parent
                }
            }
        }
    }
}
