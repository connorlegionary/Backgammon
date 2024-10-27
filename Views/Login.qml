import QtQuick 2.15
import QtQuick.Layouts 1.1

Rectangle {
    anchors.fill: parent
    color: consts.boardBackground

    Component.onCompleted: syncManager.checkLogin()

    ColumnLayout
    {
        anchors.fill: parent

        Item
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 44

            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/images/backgammon.png"

                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {
                        pushView("qrc:/Views/Settings.qml")
                    }
                }
            }
        }

        Text
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            text: qsTr("Username")
            color: consts.lightPieceColor
            font.pixelSize: 44
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle
        {
            id: userArea
            Layout.preferredWidth: parent.width - 66
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignHCenter
            radius: 11
            color: consts.darkPieceColor

            TextInput {
                id: usernameText
                anchors.centerIn: parent
                font.pixelSize: 22
                color: consts.lightPieceColor
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
            font.pixelSize: 44
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle
        {
            id: passwordArea
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
                text: qsTr("Login")
                font.pixelSize: 33
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    syncManager.login(usernameText.text, passwordText.text)
                }

                Connections {
                    target: syncManager
                    function onRequestSuccess(messageType)
                    {
                        if (messageType === messages.loginResponse)
                        {
                            pushView("qrc:/Views/Home.qml")
                        }
                    }
                    function onRequestFailed(messageType) {
                        if (messageType === messages.loginResponse)
                        {
                            passwordArea.border.width = 2
                            passwordArea.border.color = "red"
                            userArea.border.width = 2
                            userArea.border.color = "red"
                        }
                    }
                }
            }
        }

        Text
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 22
            text: qsTr("Create new")
            color: consts.lightPieceColor
            font.pixelSize: 22
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            MouseArea
            {
                anchors.fill: parent
                onClicked: pushOverlay("Overlays/CreateLogin.qml")
            }
        }
    }
}
