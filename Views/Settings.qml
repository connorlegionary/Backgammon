import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import "qrc:/Controls"

Item {

    Image {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 11
        anchors.topMargin: 11
        width: 44
        height: 44
        source: "qrc:/images/cancel.png"

        MouseArea
        {
            anchors.fill: parent
            onClicked: pushView("qrc:/Views/Login.qml")
        }
    }

    ColumnLayout
    {
        anchors.fill: parent

        Text {
            id: titleText
            Layout.fillWidth: true
            Layout.preferredHeight: 66
            text: qsTr("Settings")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
            font.bold: true
        }

        Text
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            text: qsTr("Ip Address")
            color: "black"
            font.pixelSize: 44
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Item {
            Layout.preferredWidth: parent.width - 66
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignHCenter

            Rectangle
            {
                id: userArea
                width: parent.width - 66
                height: 55
                anchors.left: parent.left
                anchors.leftMargin: 11
                anchors.verticalCenter: parent.verticalCenter
                color: "black"
                radius: 11

                TextInput {
                    id: ipText
                    anchors.centerIn: parent
                    color: "white"
                    font.pixelSize: 22
                    Component.onCompleted:  text =  settings.ip
//                    ronTextChanged: settings.ip = ipText.text
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked: ipText.forceActiveFocus()
                }
            }

            Rectangle {
                anchors.right: parent.right
                anchors.verticalCenter: userArea.verticalCenter
                width: 44
                height: 44
                color: "green"

                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {
                        settings.addIp(ipText.text)
                        ipText.text = ""
                        ipList.model = settings.ips
                    }
                }
            }
        }


        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: ipList
                anchors.fill: parent
                model: settings.ips
                delegate: Component {
                    Item {
                        height: 55
                        width: ipList.width

                        Component.onCompleted: console.log("Created ip")

                        Text {
                            anchors.centerIn: parent
                            text: modelData
                            color: consts.black
                        }

                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: {
                                settings.setIp(index+1)
                            }
                        }
                    }
                }
            }
        }
    }
}
