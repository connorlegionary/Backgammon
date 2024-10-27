import QtQuick 2.0

Item {
    id: indicator
    property alias doubleText:doubleVal

    signal doubleClicked()

    Rectangle {
        width: Math.min(parent.width, parent.height) - 11
        height: width
        anchors.centerIn: parent
        radius: 10
        color: "brown"
        border.width: 2
        border.color: "green"

        Text {
            id: doubleVal
            anchors.centerIn: parent
//            text: doubleVal
            font.pixelSize: 20
            font.bold: true
            color: "white"
        }
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: {
            indicator.doubleClicked();
        }
    }
}
