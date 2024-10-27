import QtQuick 2.15
import QtQuick.Controls 2.15

import com.backgammon.models 1.0
import com.backgammon.database 1.0

import "qrc:/Controls"

Rectangle {
    color: consts.boardBackground
    Text {
        width: parent.width
        height: 66
        text: qsTr("Games: " + syncManager.username)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 22
        font.bold: true
        color: consts.lightPieceColor

        MouseArea
        {
            id: pressArea
            anchors.fill: parent
            onClicked: {
                syncManager.downloadGames(gamesDatabase.lastId)
            }
        }

        Image {
            anchors.right: cancelButton.left
            anchors.rightMargin: 11
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/images/search.png"
            fillMode: Image.PreserveAspectFit
            width: 33
            height: 33

            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    syncManager.joinMatchmaking(1);
                    pushOverlay("qrc:/Overlays/MatchMaking.qml")
                }
            }
        }

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
                    syncManager.logout();
                    pushView("qrc:/Views/Login.qml")
                }
            }
        }
    }

    CustomToggleButton {
        width: parent.width - 32
        height: 66
        options: [qsTr("Current"), qsTr("Historical"), qsTr("Puzzles"), qsTr("Computer")]
        currentIndex: 0
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 66
        buttonInactiveColor: consts.backgroundColor
        onCurrentIndexChanged: {
            gameList.currentIndex = currentIndex
            if (currentIndex == 2)
            {
                pushView("qrc:/Views/PuzzleHome.qml")
            } else if (currentIndex == 3)
            {
                pushView("qrc:/Views/ComputerHome.qml")
            }
        }
    }

    SwipeView
    {
        id: gameList
        anchors.fill: parent
        anchors.topMargin: 166
        anchors.leftMargin: 22
        anchors.rightMargin: 22
        spacing: 22

        Item {

            GameModel {
                id: gameModel
                Component.onCompleted:
                {

                    syncManager.setGameModel(gameModel)
                    gamesList.model = gameModel;
                }
            }

            Timer {
                interval: 5000
                running: true
                repeat: true
                onTriggered: {
                    syncManager.downloadGames(gamesDatabase.lastId())
                }
            }

            ListView {
                id: gamesList
                anchors.fill: parent

                spacing: 11

//                model:
                boundsBehavior: Flickable.StopAtBounds
                clip: true
                delegate: Component {
                    Rectangle {
//                        required property string name
                        width: gamesList.width
                        height: 55
                        radius: 10
                        color: index%2 ? consts.lightColumnColor : consts.darkColumnColor
                        Component.onCompleted: closeOverlay();

                        Text {
                            anchors.centerIn: parent
                            text: name
                            font.bold: true
                            font.pixelSize: 22
                            color: index%2 ? consts.darkPieceColor : consts.lightPieceColor
                        }

                        Text {
                            anchors.right: parent.right
                            anchors.rightMargin: 11
                            anchors.verticalCenter: parent.verticalCenter
                            text: qsTr("To Score: ") + toScore
                            font.pixelSize: 18
                            font.bold: true
                            color: index%2 ? consts.darkPieceColor : consts.lightPieceColor
                        }

                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: {
                                console.log("movesDb: " + syncManager.movesDatabase)
                                syncManager.movesDatabase.gameId = index
                                syncManager.movesDatabase.clearMoves()
                                syncManager.gamesDatabase.clearLastAction(index)
                                syncManager.downloadMoves(index)
                                syncManager.gamesDatabase.updateTurn(index, 0)
                                pushView("qrc:/Views/Playing.qml", { id: index } )
                            }
                        }
                    }
                }
                footer: Component {
                    Item {
                        width: gamesList.width
                        height: 60


                        Rectangle {
                            anchors.fill: parent
                            anchors.topMargin: 11
                            color: consts.darkPieceColor
                            radius: 8

                            Text {
                                anchors.centerIn: parent
                                text: "+"
                                font.pixelSize: 30
                                font.bold: true
                                color: consts.lightPieceColor
                            }
                        }

                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked:  {
                                pushOverlay("qrc:/Overlays/CreateGame.qml")
                            }
                        }
                    }
                }
            }
        }

        Item {
            ListView {
                id: historicalList
                anchors.fill: parent

                spacing: 11
                model: gamesDatabase.finishedGames
                boundsBehavior: Flickable.StopAtBounds
                clip: true
                delegate: Component {
                    Rectangle {
                        width: historicalList.width
                        height: 55
                        radius: 10
                        color: index%2 ? consts.lightColumnColor : consts.darkColumnColor

                        Text {
                            anchors.centerIn: parent
                            text: modelData.name
                            font.bold: true
                            font.pixelSize: 22
                            color: index%2 ? consts.darkPieceColor : consts.lightPieceColor
                        }

                        Text {
                            anchors.right: parent.right
                            anchors.rightMargin: 11
                            anchors.verticalCenter: parent.verticalCenter
                            text: qsTr("To Score: ") + modelData.toScore
                            font.pixelSize: 18
                            font.bold: true
                            color: index%2 ? consts.darkPieceColor : consts.lightPieceColor
                        }

                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: {
                                syncManager.movesDatabase.gameId = modelData.id
                                syncManager.gamesDatabase.clearLastAction(modelData.id)
                                syncManager.downloadMoves(modelData.id)
                                syncManager.gamesDatabase.updateTurn(modelData.id, 0)
                                pushView("qrc:/Views/Playback.qml", { id: modelData.id } )
                            }
                        }
                    }
                }
            }
        }
    }
}
