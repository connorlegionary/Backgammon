import QtQuick 2.15
import QtQuick.Layouts 1.1

Rectangle {
    id: toggleButton
    color: consts.white
    radius: height

    property var buttonInactiveColor: consts.white
    property int textSize: consts.bodyTextSize
    property var options: []
    property int currentIndex: 0
    property var delegate: Component {
        Rectangle {
            radius: height/2
            color: index == currentIndex ? consts.black : buttonInactiveColor
            Layout.fillWidth: true
            Layout.preferredWidth: Math.min(textArea.paintedWidth, (optionsList.width - options.length*8)/options.length)
            Layout.preferredHeight: optionsList.height - 16
            Layout.minimumWidth: textArea.paintedWidth + 24
            Layout.alignment: Qt.AlignVCenter

            Text {
                id: textArea
                text: modelData
                anchors.centerIn: parent
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                fontSizeMode: Text.Fit
                color: index == currentIndex ? consts.white : consts.black
                font.pixelSize: textSize
                font.italic: false
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked: currentIndex = index
            }
        }
    }

    RowLayout
    {
        id: optionsList
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        spacing: 8

        Repeater {
            model: options
            delegate: toggleButton.delegate
        }
    }
}
