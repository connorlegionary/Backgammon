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
            anchors.centerIn: parent
            text: qsTr("Puzzle: " ) + viewContext.id
            font.bold: true
            font.pixelSize: 24
            color: "white"
//            visible: paintedWidth < (blackDoubleIndicator.x - (whiteDoubleIndicator.x + whiteDoubleIndicator.width))
        }

        Text {
            id: scoreText
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("Score: ")
            color: "white"
        }
        Text {
            id: scoreValueText
            anchors.left: scoreText.right
            anchors.verticalCenter: scoreText.verticalCenter
            text: pieceModel.score
            color: "white"

            Text {
                id: scoreValueAnimText
                anchors.left: scoreValueText.left
//                anchors.verticalCenter: scoreText.verticalCenter
                text: pieceModel.score
                color: "green"
                visible: false
                NumberAnimation {id: newScoreAnim; running: visible; from: scoreValueText.y - 100; to: scoreValueText.y; duration: 300; onFinished: { scoreValueAnimText.visible = false} }

                Connections {
                    target: pieceModel
                    onScoreChanged: {
//                        scoreValueAnimText.y = scoreValueText.y - 100;
                        scoreValueAnimText.visible = true
                        newScoreAnim.start()
                    }
                }
            }
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

    PuzzlePieceModel {
        id: pieceModel
        puzzle: puzzleDb.getPuzzleWithData(viewContext.id)
        Component.onCompleted:
        {
            pieceModel.loadPieces(puzzleDb.getLocations(viewContext.id))
        }

        onIncorrectPuzzleMove:
        {
            pushOverlay("qrc:/Overlays/IncorrectPuzzleMove.qml")
            closeOverlayTimer.start()
        }

        onPuzzleFinished:
        {
            syncManager.puzzlesDatabase.puzzleFinished(viewContext.id, pieceModel.score)
            pushOverlay("qrc:/Overlays/PuzzleFinished.qml")
        }


    }
    Timer {
        id: closeOverlayTimer
        interval: 500
        running: false
        repeat: false
        onTriggered: {
            closeOverlay();
        }
    }

    PuzzleBoard {
        id: board
        anchors.fill: parent
        anchors.topMargin: 66
        model: pieceModel
    }
}
