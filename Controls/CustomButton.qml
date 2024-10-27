import QtQuick 2.15

Rectangle {
    radius: height
//    border.width: 3
    property alias text: buttonText
    property alias image: buttonImage
    property alias pressed: mouse.pressed
    signal clicked()
    signal hovered()
    color: consts.lightPieceColor

    Text {
        id: buttonText
        anchors.left: parent.left
        anchors.leftMargin: 16
//        width: parent.width - buttonImage.source === "" ? 16 : 49
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Qt.AlignVCenter
        font.bold: true
//        font.pixelSize: consts.buttonTextSize
        color: consts.darkPieceColor
    }

    Image {
        id: buttonImage
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.verticalCenter: parent.verticalCenter
        width: 33
        height: 33
        source: ""
    }

    MouseArea
    {
        id: mouse
        anchors.fill: parent
        onClicked:
        {
            parent.clicked()
        }
        onHoveredChanged: parent.hovered()
    }
}
