import QtQuick 2.15
import QtQuick.Layouts 1.12

import com.backgammon.models 1.0
import com.backgammon.controls 1.0

import "qrc:/Controls"

Rectangle {
    id: analBoardRoot
    property var model
    property bool preview: false
    property var showStartGame: false
    color: consts.backgroundColor

    function hideTurnFinished()
    {
         model.isTurnFinished = false;
    }

    function previewMove(from, to)
    {
//        console.log("Preview move: " + puzzleModel)
//        if (puzzleModel.isCorrectMove(from, to))
//        {
//            model.analysisMove(from, to, false)
//            // good move
//        } else {
//            // bad move
//        }


    }

    function clearMoves()
    {
        model.endTurn()
    }

    Connections {
        target: model
        onBoardMove: {
//            console.log("Board move")
            if (isWhite == analBoardRoot.model.isWhite)
            {
                model.tempMove(fromPos, myTurn, fromServer, isWhite)
            }
        }
    }

    Item {
        anchors.fill: parent

        Item {
            anchors.fill: parent

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                color: consts.boardBackground
                width: 44
                height: parent.height
            }

            ColumnLayout
            {
                anchors.fill: parent

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    z: 1

                    RowLayout
                    {
                        anchors.fill: parent
                        spacing: 0

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "1" : "24"
                                color: "white"
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "2" : "23"
                                color: "white"
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "3" : "22"
                                color: "white"
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "4" : "21"
                                color: "white"
                            }
                        }
                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "5" : "20"
                                color: "white"
                            }
                        }
                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "6" : "19"
                                color: "white"
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.preferredWidth: 44
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "7" : "18"
                                color: "white"
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "8" : "17"
                                color: "white"
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "9" : "16"
                                color: "white"
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "10" : "15"
                                color: "white"
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "11" : "14"
                                color: "white"
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "12" : "13"
                                color: "white"
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    z: 1

                    RowLayout
                    {
                        anchors.fill: parent
                        anchors.bottom: parent.bottom
                        spacing: 0

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "24" : "1"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "23" : "2"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "22" : "3"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "21" : "4"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "20" : "5"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "19" : "6"
                                color: "white"
                                rotation: 180
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.preferredWidth: 44
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "18" : "7"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "17" : "8"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "16" : "9"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "15" : "10"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: false

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "14" : "11"
                                color: "white"
                                rotation: 180
                            }
                        }

                        BackgammonColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            even: true

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 11
                                text: syncManager.gamesDatabase.isWhite(viewContext.id) ? "13" : "12"
                                color: "white"
                                rotation: 180
                            }
                        }
                    }
                }
            }
        }

        Item {
            anchors.fill: parent

            ColumnLayout
            {
                anchors.fill: parent

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    z: 1

                    RowLayout
                    {
                        anchors.fill: parent
                        spacing: 0

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 1 : 24
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 2 : 23
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 3 : 22
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 4 : 21
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 5 : 20
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 6 : 19
                            model: analBoardRoot.model
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.preferredWidth: 44

                            Connections {
                                target: model
                                function onPositionsChanged()
                                {
                                    offPieceList.model = model.getOffPieces();
//                                    console.log("Get off piece list again: " + model.getOffPieces())
                                }
                            }

                            ListView {
                                id: offPieceList
                                anchors.fill: parent
                                model: model.getOffPieces()
                                orientation: Qt.Vertical
                                function movePiece(piece)
                                {

                                    if (analBoardRoot.model.boardMove(-1, true, false, piece.isWhite))
                                    {
//                                        console.log("temp move")
//                                        syncManager.addMove(0, viewContext.id, -1, fromPos + moveFinal, 0, piece.isWhite, pieceModel.checkTook())
                                    } else {
//                                        console.log("can't temp move")
                                    }
                                }
                                delegate: Component {
                                    Piece {
                                        height: Math.min(offPieceList.height/5, offPieceList.width)
                                        width: height
                                        color: modelData.isWhite ? "#FFFFFF" : "#353535"
                                        border.color: modelData.isWhite ? "#353535" : "#FFFFFF"
                                        onPieceMoved: {
                                            console.log("piece moved, modelData.isWhite: " + modelData.isWhite + " analBoardRoot.model.isWhite: " + analBoardRoot.model.isWhite);
                                            if (modelData.isWhite == analBoardRoot.model.isWhite)
                                                offPieceList.movePiece(piece)
                                        }
                                    }
                                }
                            }
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 7 : 18
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 8 : 17
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 9 : 16
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 10 : 15
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 11 : 14
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 12 : 13
                            model: analBoardRoot.model
                        }
                    }

                    Text {
                        id: doubleText
                        anchors.centerIn: parent
                        width: 200
                        wrapMode: Text.WordWrap
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        color: "white"
                        font.pixelSize: 24
                        visible: false
                        text: qsTr("Your opponent doubled, accept?")
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    z: 10

                    ListView {
                        id: moveList
                        width: board.model.moves.length > 0 ? board.model.moves.length * 44 : board.model.displayDice.length * 44
                        height: parent.height
                        anchors.centerIn: parent
                        model: board.model.moves.length > 0 ? board.model.moves : board.model.displayDice
                        interactive: false
                        orientation: ListView.Horizontal
                        delegate: playingDice

                        Component {
                            id: playingDice

                            Item {
                                width: 44
                                height: moveList.height

                                MouseArea
                                {
                                    anchors.fill: parent
                                    onClicked: board.model.reverseDice()
                                }


                                Image {
                                    anchors.fill: parent
                                    fillMode: Image.PreserveAspectFit
                                    source: {
                                        if (board.model.isWhite)
                                        {
                                            switch (modelData)
                                            {
                                            case 1:
                                                "qrc:/images/dice_white_1.png"
                                                break;
                                            case 2:
                                                "qrc:/images/dice_white_2.png"
                                                break;
                                            case 3:
                                                "qrc:/images/dice_white_3.png"
                                                break;
                                            case 4:
                                                "qrc:/images/dice_white_4.png"
                                                break;
                                            case 5:
                                                "qrc:/images/dice_white_5.png"
                                                break;
                                            case 6:
                                                "qrc:/images/dice_white_6.png"
                                                break;
                                            }
                                        } else {
                                            switch (modelData)
                                            {
                                            case 1:
                                                "qrc:/images/dice_black_1.png"
                                                break;
                                            case 2:
                                                "qrc:/images/dice_black_2.png"
                                                break;
                                            case 3:
                                                "qrc:/images/dice_black_3.png"
                                                break;
                                            case 4:
                                                "qrc:/images/dice_black_4.png"
                                                break;
                                            case 5:
                                                "qrc:/images/dice_black_5.png"
                                                break;
                                            case 6:
                                                "qrc:/images/dice_black_6.png"
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        Component {
                            id: startDice

                            Item {
                                width: 44
                                height: moveList.height

                                Image {
                                    anchors.fill: parent
                                    fillMode: Image.PreserveAspectFit
                                    source: {
                                        switch (modelData)
                                        {
                                        case 1:
                                            index == 0 ? "qrc:/images/dice_white_1.png" : "qrc:/images/dice_black_1.png"
                                            break;
                                        case 2:
                                            index == 0 ? "qrc:/images/dice_white_2.png" : "qrc:/images/dice_black_2.png"
                                            break;
                                        case 3:
                                            index == 0 ? "qrc:/images/dice_white_3.png" : "qrc:/images/dice_black_3.png"
                                            break;
                                        case 4:
                                            index == 0 ? "qrc:/images/dice_white_4.png" : "qrc:/images/dice_black_4.png"
                                            break;
                                        case 5:
                                            index == 0 ? "qrc:/images/dice_white_5.png" : "qrc:/images/dice_black_5.png"
                                            break;
                                        case 6:
                                            index == 0 ? "qrc:/images/dice_white_6.png" : "qrc:/images/dice_black_6.png"
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    RowLayout
                    {
                        anchors.fill: parent
                        spacing: 22
                        visible: !preview

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }

                        CustomButton {
                            id: rollButton
                            Layout.fillHeight: true
                            Layout.preferredWidth: 120
                            text.text: qsTr("Roll")
                            visible: false
                            image.source: "qrc:/images/roll.png"
                            onClicked: {
                                syncManager.rollDice(viewContext.id)
                                rollButton.visible = false
                                doubleButton.visible = false;
                            }
                        }
                        CustomButton {
                            id: endTurnButton
                            Layout.fillHeight: true
                            Layout.preferredWidth: 120
                            text.text: qsTr("End Turn")
                            image.source: "qrc:/images/finish.png"
                            visible: model.isTurnFinished && model.myTurn

//                            Connections {
//                                target: model
//                            }

                            onClicked:  {

                                model.userFinishedTurn()
                            }
                        }


                        CustomButton {
                            id: doubleButton
                            Layout.fillHeight: true
                            Layout.preferredWidth: 120
                            text.text: qsTr("Double")
                            image.source: "qrc:/images/double.png"
                            visible: false
                            onClicked:  {
                                syncManager.sendDouble(viewContext.id)
                                rollButton.visible = false
                                doubleButton.visible = false;

                            }
                        }

                        CustomButton {
                            id: declineDoubleButton
                            Layout.fillHeight: true
                            Layout.preferredWidth: 120
                            text.text: qsTr("Decline")
                            image.source: "qrc:/images/double.png"
                            visible: false
                            onClicked:  {
                                syncManager.doubleResponse(viewContext.id, false);
                                declineDoubleButton.visible = false;
                                acceptDoubleButton.visible = false;
                                rollButton.visible = false
                                doubleButton.visible = false;
                                doubleText.visible = false;
                            }
                        }

                        CustomButton {
                            id: acceptDoubleButton
                            Layout.fillHeight: true
                            Layout.preferredWidth: 120
                            text.text: qsTr("Accept")
                            image.source: "qrc:/images/double.png"
                            visible: false
                            onClicked:  {
                                syncManager.doubleResponse(viewContext.id, true);
                                declineDoubleButton.visible = false;
                                acceptDoubleButton.visible = false;
                                rollButton.visible = false
                                doubleButton.visible = false;
                                doubleText.visible = false;
                            }
                        }

                        CustomButton {
                            id: startGameButton
                            Layout.fillHeight: true
                            Layout.preferredWidth: 120
                            text.text: qsTr("Start Game")
//                            image.source: "qrc:/images/double.png"
                            visible: showStartGame
                            onClicked:  {
                                model.startGame()
                                startGameButton.visible = false
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    z: 1

                    RowLayout
                    {
                        anchors.fill: parent
                        anchors.bottom: parent.bottom
                        spacing: 0

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 24 : 1
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 23 : 2
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 22 : 3
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 21 : 4
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 20 : 5
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 19 : 6
                            model: analBoardRoot.model
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.preferredWidth: 44
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 18 : 7
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 17 : 8
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 16 : 9
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 15 : 10
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 14 : 11
                            model: analBoardRoot.model
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 13 : 12
                            model: analBoardRoot.model
                        }
                    }
                }
            }
        }
    }
}
