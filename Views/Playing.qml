import QtQuick 2.15
import Qt5Compat.GraphicalEffects

import "qrc:/Controls"

Rectangle {
    property var game: syncManager.gamesDatabase.getGame(viewContext.id)
    color: consts.boardBackground
    anchors.fill: parent

    Connections {
        target: syncManager
        function onCurrentMatch(matchId)
        {
            game.matchId = matchId
            syncManager.gamesDatabase.setCurrentMatchId(viewContext.id, matchId)
        }

        function onCurrentStats(whiteScore, blackScore, whiteTime, blackTime, doubleVal, whoseDouble)
        {
            game.score1 = whiteScore
            game.score2 = blackScore
            game.doubleVal = doubleVal


            whiteTimeText.text = consts.msToTime(whiteTime)
            blackTimeText.text = consts.msToTime(blackTime)
            board.model.whoseDouble = whoseDouble
        }

        function onWonGame(isWhite, current) {
            board.model.whoseDouble = 2
            board.model.isTurnFinished = false
            if (game.score1 >= game.toScore || game.score2 >= game.toScore)
            {
                syncManager.setFinished(viewContext.id);
                pushOverlay("qrc:/Overlays/WonMatch.qml", { gameId: viewContext.id, white: isWhite })
            } else {
                pushOverlay("qrc:/Overlays/WonGame.qml", { gameId: viewContext.id, white: isWhite })
                board.model.resetPositions()
            }
        }

        function onWonMatch(isWhite)
        {
            pushOverlay("qrc:/Overlays/WonMatch.qml", { white: isWhite })
        }
    }

    Connections {
        target: board.model
        onUndid: {
            turnFinishedButton.visible = false
        }
    }

    Rectangle {
        width: parent.width
        height: 44
        color: consts.boardBackground

        Text {
            id: player1Score
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 22
            font.bold: true
            text: qsTr("Score: " ) + game.score1
            color: "white"
        }

        Text {
            id: player1Pips
            anchors.left: parent.left
            anchors.top: player1Score.bottom
            anchors.leftMargin: 22
            font.bold: true
            text: qsTr("Pips: " ) + board.model.pips(0)
            color: "white"

            Connections {
                target: board.model
                onPositionsChanged: {
                    player1Pips.text = qsTr("Pips: " ) + board.model.pips(0)
                    player2Pips.text = qsTr("Pips: " ) + board.model.pips(1)
                }
            }
        }

        DoubleIndicator {
            id: whiteDoubleIndicator
            anchors.left: player1Score.right
            anchors.leftMargin: 11
            anchors.verticalCenter: player1Pips.top
            doubleText.text: game.doubleVal
            width: 33
            height: 33
            visible: board.model.whoseDouble == 2 || board.model.whoseDouble == 1
        }

        Text {
            anchors.centerIn: parent
            text: game.name
            font.bold: true
            font.pixelSize: 24
            color: "white"
            visible: paintedWidth < (blackDoubleIndicator.x - (whiteDoubleIndicator.x + whiteDoubleIndicator.width))
        }

        Text {
            id: player2Score
            anchors.right: leaveButton.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 22
            text: qsTr("Score: " ) + game.score2
            font.bold: true
            color: "white"
        }

        Text {
            id: player2Pips
            anchors.left: player2Score.left
            anchors.top: player2Score.bottom
//            anchors.leftMargin: 22
            font.bold: true
            text: qsTr("Pips: " ) + board.model.pips(1)
            color: "white"
        }

        DoubleIndicator {
            id: blackDoubleIndicator
            anchors.right: player2Score.left
            anchors.rightMargin: 11
            anchors.verticalCenter: player2Pips.top
            doubleText.text: game.doubleVal
            width: 33
            height: 33
            visible: board.model.whoseDouble == 2 || board.model.whoseDouble == 0
        }

        Image {
            id: leaveButton
            anchors.right: parent.right
            anchors.rightMargin: 11
            anchors.top: parent.top
            anchors.topMargin: 11
            height: parent.height - 11
            width: height
            source: "qrc:/images/menu.png"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pushOverlay("qrc:/Overlays/Menu.qml")
                }
            }
        }
    }

    Board {
        id: board
        anchors.fill: parent
        anchors.topMargin: 66
        anchors.rightMargin: 66
    }

    Item {
        width: 66
        height: parent.height
        anchors.right: parent.right

        Rectangle {
            id: topIndicator
            color: consts.lightPieceColor
            border.color: syncManager.turnIsWhite ? consts.darkPieceColor : consts.activeRed
            width: parent.width - 6
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 55

            Text {
                id: whiteTimeText
                anchors.centerIn: parent
                font.pixelSize: 24
                color: consts.darkPieceColor
            }
        }

        Image {
            id: undoButton
            anchors.top: topIndicator.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 11
            width: parent.width
            height: width
            source: "qrc:/images/undo.png"

            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    syncManager.undoMove(movesDatabase.gameId);
                    board.model.undoMoveAction();
                    board.hideTurnFinished()
                }
            }
        }

        Image {
            id: turnFinishedButton
            anchors.top: undoButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 11
            width: parent.width
            height: width
            source: "qrc:/images/finish.png"
            visible: false
        }

        ColorOverlay {
            id: finishColor
            visible: board.model.isTurnFinished && syncManager.turnIsWhite == syncManager.gamesDatabase.isWhite(viewContext.id)
            anchors.fill: turnFinishedButton
            source: turnFinishedButton
            color: "red"  // make image like it lays under red glass
            PropertyAnimation {id: animateColorRed; target: finishColor; properties: "color"; to: "red"; duration: 1000; onFinished: animateColorBlack.start()}
            PropertyAnimation {id: animateColorBlack; target: finishColor; properties: "color"; to: "black"; duration: 1000; onFinished:  animateColorRed.start(); running: true}
        }

        Image {
            id: analysisButton
            anchors.top: finishColor.visible ? finishColor.bottom : undoButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 11
            width: parent.width
            height: width
            source: "qrc:/images/analysis.png"

            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    pushOverlay("qrc:/Overlays/Analysis.qml", {pieces: board.model.pieces, color: gamesDatabase.isWhite(viewContext.id), diceRoll: board.model.getLastRoll() })
                }
            }
        }

        Rectangle {
            id: bottomIndicator
            color: consts.darkPieceColor
            border.color: syncManager.turnIsWhite ? "#FFFFFF" : consts.activeRed
            width: parent.width - 6
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 11

            Text {
                id: blackTimeText
                anchors.centerIn: parent
                font.pixelSize: 24
                color: consts.lightPieceColor
            }
        }
    }
}
