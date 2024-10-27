import QtQuick 2.15
import QtQuick.Layouts 1.1

Item {
    id: createLoginRoot
    anchors.fill: parent

    Rectangle
    {
        anchors.fill: parent
        color: "black"
        opacity: 0.2
    }

    Rectangle
    {
        anchors.centerIn: parent
        width: parent.width*0.7
        height: parent.height * 0.8
        radius: 30
        border.width: 3
        border.color: consts.lightPieceColor
        color: consts.boardBackground

        ColumnLayout
        {
            anchors.fill: parent
            spacing: 0

            Text
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                text: qsTr("Create New User")
                color: consts.lightPieceColor
                font.pixelSize: 33
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Text
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                text: qsTr("Username")
                color: consts.lightPieceColor
                font.pixelSize: 22
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle
            {
                Layout.preferredWidth: parent.width - 66
                Layout.preferredHeight: 44
                Layout.alignment: Qt.AlignHCenter
                radius: 11
                color: consts.darkPieceColor

                TextInput {
                    id: usernameText
                    anchors.centerIn: parent
                    color: consts.lightPieceColor
                    font.pixelSize: 22
                    EnterKey.type: Qt.EnterKeyDone
                    focus: true

                    Keys.onEnterPressed: {
                        createLoginRoot.forceActiveFocus()
                    }
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked: usernameText.forceActiveFocus()
                }
            }

            Text
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Password")
                color: consts.lightPieceColor
                font.pixelSize: 22
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle
            {
                Layout.preferredWidth: parent.width - 66
                Layout.preferredHeight: 44
                Layout.alignment: Qt.AlignHCenter
                radius: 11
                color: consts.darkPieceColor

                TextInput {
                    id: passwordText
                    anchors.centerIn: parent
                    color: consts.lightPieceColor
                    font.pixelSize: 22
                    EnterKey.type: Qt.EnterKeyDone

                    Keys.onEnterPressed: {
                        createLoginRoot.forceActiveFocus()
                    }
                    echoMode: TextInput.Password
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked: passwordText.forceActiveFocus()
                }
            }

            Rectangle
            {
                Layout.preferredWidth: parent.width - 66
                Layout.preferredHeight: 44
                Layout.alignment: Qt.AlignHCenter
                radius: 11
                color: consts.activeRed

                Text {
                    anchors.fill: parent
                    color: consts.lightPieceColor
                    text: qsTr("Create")
                    font.pixelSize: 33
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        syncManager.createUser(usernameText.text, passwordText.text)
                        closeOverlay();
                    }
                }
            }
        }
    }
}
