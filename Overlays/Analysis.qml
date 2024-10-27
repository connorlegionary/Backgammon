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

        GameAnalysis {
            id: analysis
            model: analModel
            Component.onCompleted: getBestMoves(viewContext.pieces, viewContext.color, viewContext.diceRoll)
        }

        PositionModel {
            id: analModel
        }

        PositionSort {
            id: sort
            sourceModel: analModel
        }

        PieceModel {
            id: pieceModel
            actionDatabase: actionsDatabase
            Component.onCompleted:
            {
                pieceModel.loadPieces(viewContext.pieces)
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
            model: sort
            delegate: Component {
                Item {
                    width: analList.width
                    height: 66

//                    required property string from
//                    required property string score
//                    required property string to

                    Rectangle {
                        anchors.fill: parent
                        color: consts.darkPieceColor
                        border.width: 2
                        border.color: consts.black

                        RowLayout
                        {
                            anchors.fill: parent
                            spacing: 0

                            Item {
                                Layout.fillHeight: true
                                Layout.fillWidth: true

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.top: parent.top
                                    anchors.topMargin: 11
                                    text: qsTr("Score")
                                    color: consts.lightPieceColor
                                }

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 11
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    text: score
                                    color: consts.lightPieceColor
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.top: parent.top
                                    anchors.topMargin: 11
                                    text: qsTr("whitePointsLeft: " + whitePointsLeft)
                                    color: consts.lightPieceColor
                                }

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 11
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    text: qsTr("blackPointsLeft: " + blackPointsLeft)
                                    color: consts.lightPieceColor
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                                Layout.fillWidth: true

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.top: parent.top
                                    anchors.topMargin: 11
                                    text: qsTr("Moves")
                                    color: consts.lightPieceColor
                                }

                                ListView {
                                    id: moveList
                                    anchors.fill: parent
                                    anchors.topMargin: 22
//                                    Component.onCompleted: console.log("Moves: " + moves)
                                    model: moves
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
                            }
                        }
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            previewBoard.clearMoves()
                            for (var i = 0; i < moves.length; i++)
                            {
                                console.log("loading move: " + moves[i].from + " to: " + moves[i].to)
                                previewBoard.previewMove(moves[i].from, moves[i].to)
                            }
                            analysis.analiseMove(previewBoard.model.pieces, 0)
                        }
                    }
                }
            }
        }
    }
}
