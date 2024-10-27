import QtQuick 2.15
import QtQuick.Controls 2.15

import com.backgammon.models 1.0
import com.backgammon.database 1.0


Rectangle {
    color: consts.boardBackground

    PuzzleModel {
        id: puzzleModel
        puzzlesDatabase: syncManager.puzzlesDatabase
    }

    Text {
        width: parent.width
        height: 66
        text: qsTr("Computer")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 22
        font.bold: true
        color: consts.lightPieceColor

//        Image {
//            anchors.right: cancelButton.left
//            anchors.rightMargin: 11
//            anchors.verticalCenter: parent.verticalCenter
//            source: "qrc:/images/search.png"
//            fillMode: Image.PreserveAspectFit
//            width: 33
//            height: 33

//            MouseArea
//            {
//                anchors.fill: parent
//                onClicked: {
//                    syncManager.joinMatchmaking(1);
//                    pushOverlay("qrc:/Overlays/MatchMaking.qml")
//                }
//            }
//        }

        Image {
            id: cancelButton
            anchors.right: parent.right
            anchors.rightMargin: 11
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/images/cancel.png"
            width: 55
            height: 55

            MouseArea
            {
                anchors.fill: parent
                onClicked: {
//                    syncManager.logout();
                    pushView("qrc:/Views/Home.qml")
                }
            }
        }
    }

    Item {
        anchors.fill: parent
        anchors.topMargin: 66

        ComputerGamesDatabase {
            id: computerGamesDatabase
        }

        ListView {
            id: puzzleList
            anchors.fill: parent
            anchors.margins: 16

            spacing: 11
            model: computerGamesDatabase.games
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            delegate: Component {
                Rectangle {
                    width: puzzleList.width
                    height: 55
                    radius: 10
                    color: index%2 ? consts.lightColumnColor : consts.darkColumnColor

                    Text {
                        anchors.centerIn: parent
                        width: parent.width
                        text: "Computer: " + modelData.computer + " started at " + modelData.date
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                        font.pixelSize: 22
                        color: index%2 ? consts.darkPieceColor : consts.lightPieceColor
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            pushView("qrc:/Views/Computer.qml", { id: modelData.id } )
                        }
                        onPressAndHold: {
                            pushOverlay("qrc:/Overlays/DeleteGame.qml", {id: modelData.id, database: computerGamesDatabase})
                        }
                    }
                }
            }
            footer: Component {
                Rectangle {
                    width: puzzleList.width
                    height: 55
                    radius: 10
                    color: consts.black
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        anchors.centerIn: parent
                        text: "New Game"
                        font.bold: true
                        font.pixelSize: 22
                        color: consts.white
//                        color: modelData.id%2 ? consts.darkPieceColor : consts.lightPieceColor
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            pushOverlay("qrc:/Overlays/AddGame.qml", {database: computerGamesDatabase})
                        }
                    }
                }
            }
        }
    }
}
