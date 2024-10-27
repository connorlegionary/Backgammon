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
        text: qsTr("Puzzles: " + syncManager.username)
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
                console.log("Download puzzle")
                syncManager.downloadPuzzles()
            }
        }

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

        ListView {
            id: puzzleList
            anchors.fill: parent

            spacing: 11
            model: puzzleModel
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
                        text: name
                        font.bold: true
                        font.pixelSize: 22
                        color: index%2 ? consts.darkPieceColor : consts.lightPieceColor
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            pushView("qrc:/Views/Puzzle.qml", { id: index } )
                        }
                    }
                }
            }
        }
    }
}
