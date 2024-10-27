import QtQuick 2.15
import QtQuick.Layouts 1.15

import com.backgammon.analysis 1.0
import com.backgammon.models 1.0
import com.backgammon.controls 1.0

import "qrc:/Controls"

Item {
    Rectangle {
        anchors.right: parent.right
        height: parent.height
        width: 300

        PositionModel {
            id: analModel
        }

        PieceModel {
            id: pieceModel
            actionDatabase: actionsDatabase
            Component.onCompleted:
            {
                console.log("Id: " + viewContext.id + " analModel: " + analModel)
                pieceModel.loadPreviousMoves(viewContext.id, analModel)
            }
            Component.onDestruction: {
                pieceModel.undoMoveAction();
            }
        }

        AnalysisBoard {
            id: previewBoard
            height: parent.height/2
            width: parent.width
            enabled: false
            preview: true
            model: pieceModel
        }

        Text {
            anchors.top: previewBoard.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Moves")
        }

        ListView {
            id: analList
            height: parent.height/2
            width: parent.width
            anchors.top: previewBoard.bottom
            anchors.topMargin: 22
            clip: true
            model: analModel
            delegate: Component {
                Rectangle {
                    width: analList.width
                    height: (moves.length *22) + 22
                    border.color: consts.black
                    border.width: 2

                    Text {
                        anchors.top: previewBoard.bottom
                        anchors.topMargin: 5
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Moves")
                    }

                    ListView {
                        id: moveList
                        anchors.fill: parent
                        anchors.topMargin: 22
//                        Component.onCompleted: console.log("Moves: " + moves)
                        model: moves
                        interactive: false
                        spacing: 0
                        delegate: Component {
                            Item {
                                width: moveList.width
                                height: 22

                                Text {
                                    anchors.left: parent.left
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: qsTr("From: ") + modelData.from
                                }

                                Text {
                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: qsTr("to: ") + modelData.to
                                }
                            }
                        }
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            pieceModel.loadPieces(positions.pieces)
//                            previewBoard.clearMoves()
//                            for (var i = 0; i < positions.pieces.length; i++)
//                            {
//                                console.log("Position at: " + i + " =  " + positions.pieces[i].position)
//                                previewBoard.previewMove(moves[i].from, moves[i].to)
//                            }
//                            analysis.analiseMove(previewBoard.model.pieces, 0)
                        }
                        onPressAndHold: {
                            viewContext.model.reloadPosition(viewContext.id ,index)
                        }
                    }
                }
            }
        }
    }
}
