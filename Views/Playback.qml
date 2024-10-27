import QtQuick 2.15
import Qt5Compat.GraphicalEffects

import "qrc:/Controls"

import com.backgammon.models 1.0

Rectangle {
    property var game: syncManager.gamesDatabase.getGame(viewContext.id)
    color: consts.boardBackground
    anchors.fill: parent

    Rectangle {
        width: parent.width
        height: 44
        color: consts.boardBackground

        Text {
            id: player1Score
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 22
            text: qsTr("Score: " ) + game.score1
            color: "white"
        }

        DoubleIndicator {
            anchors.left: player1Score.right
            anchors.leftMargin: 11
            anchors.verticalCenter: parent.verticalCenter
            doubleText.text: game.doubleVal
            width: 33
            height: 33
            visible: board.model.whoseDouble == 2 || board.model.whoseDouble == 1
//            onDoubleClicked: {
//                pushOverlay("qrc:/Overlays/SendDouble.qml", {gameId: viewContext.id, model: board.model, game: game})
//            }
        }

        Text {
            anchors.centerIn: parent
            text: game.name
            font.bold: true
            font.pixelSize: 24
            color: "white"
        }

        Text {
            id: player2Score
            anchors.right: leaveButton.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 22
            text: qsTr("Score: " ) + game.score2
            color: "white"
        }

        DoubleIndicator {
            anchors.right: player2Score.left
            anchors.rightMargin: 11
            anchors.verticalCenter: parent.verticalCenter
            doubleText.text: game.doubleVal
            width: 33
            height: 33
            visible: board.model.whoseDouble == 2 || board.model.whoseDouble == 0
//            onDoubleClicked: {
//                pushOverlay("qrc:/Overlays/SendDouble.qml", {gameId: viewContext.id, model: board.model})
//            }
        }

        Image {
            id: leaveButton
            anchors.right: parent.right
            anchors.rightMargin: 11
            anchors.top: parent.top
            anchors.topMargin: 11
            height: parent.height - 11
            width: height
            source: "qrc:/images/cancel.png"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pushView("qrc:/Views/Home.qml")
                }
            }
        }
    }

    AnalysisBoard {
        id: board
        anchors.fill: parent
        anchors.topMargin: 66
        anchors.rightMargin: 66
        model: model
    }

    PieceModel {
        id: model
        actionDatabase: actionsDatabase
        Component.onCompleted:
        {
            syncManager.movesDatabase.gameId = viewContext.id
            model.positionsChanged()
        }
    }


    PlaybackModel {
        id: playbackModel
        pieceModel: model
        moves: movesDatabase.getMoves()
    }

//    function reloadMoves()
//    {
//        var moves =
//        pieceModel.endTurn();
//        console.log("Loading moves; " << moves);
//        for (var i =0 ; i < moves.length; i++)
//        {
//            pieceModel.loadMove(pieceModel.getPiece(moves[i].fromPosition), moves[i].move)
//        }
//    }

    Item {
        id: playbackButtons
        width: 66
        height: parent.height
        anchors.right: parent.right

        Image {
            id: previousButton
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 55
            width: parent.width
            height: width
            source: "qrc:/images/undo.png"
            visible: playbackModel.currentMove > 0

            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    playbackModel.previousMove()
                }
            }
        }


        Image {
            id: nextButton
            anchors.top: previousButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 11
            width: parent.width
            height: width
            source: "qrc:/images/undo.png"
            rotation: 180

            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    playbackModel.nextMove()
                }
            }
        }

        Image {
            id: analysisButton
            anchors.top: nextButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 11
            width: parent.width
            height: width
            source: "qrc:/images/analysis.png"

            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    pushOverlay("qrc:/Overlays/Analysis.qml", {pieces: model.pieces, color: gamesDatabase.isWhite(viewContext.id), diceRoll: pieceModel.getLastRoll() })
                }
            }
        }
    }
}
