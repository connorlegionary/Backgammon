import QtQuick 2.15
import QtQuick.Layouts 1.12

import com.backgammon.models 1.0
import com.backgammon.controls 1.0

import "qrc:/Controls"

Rectangle {
    property alias model: pieceModel
    property bool preview: false
    color: consts.backgroundColor

    function hideTurnFinished()
    {
         model.isTurnFinished = false;
    }

    Component.onCompleted: pieceModel.positionsChanged()
    function previewMove(from, to)
    {
        pieceModel.analysisMove(from, to, false)
    }

    function clearMoves()
    {
        pieceModel.endTurn()
    }

    Connections {
        target: model
        onBoardMove: {
            if (syncManager.turnIsWhite == isWhite)
            {
                model.tempMove(fromPos, myTurn, fromServer, isWhite)
            }
        }
    }

    PieceModel {
        id: pieceModel
        isWhite: gamesDatabase.isWhite(viewContext.id)
        actionDatabase: actionsDatabase
        onNoMoves: {
            noMoveTimer.triggers = 0;
            noMoveTimer.start()
            //            pushOverlay("qrc:/Overlays/NoMove.qml", {model: board.model } )
        }
        onTurnFinished: {
            if (isMyTurn)
            {
                syncManager.downloadMoves(viewContext.id)
            }
        }

        onWonGame: {
            model.isTurnFinished = false
            model.whoseDouble = 2

        }
        onUndoMove: {
            syncManager.undoMove(viewContext.id, currentPos);
        }
        onSendMove: {
            console.log("Add move");
            syncManager.addMove(0, viewContext.id, fromPos, toPos, 0, isWhite, took)
        }

        onSyncTurnFinished:
        {
            syncManager.turnFinished(viewContext.id);
        }
    }

    Timer {
        id: noMoveTimer
        interval: 2000
        running: false
        repeat: false
        property var triggers: 0
        onTriggered: {
            console.log("No move triggered");
             model.isTurnFinished = true
        }
    }

    Timer {
        id: refreshTimer
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            syncManager.downloadMoves(viewContext.id)
        }
    }

    Connections {
        target: syncManager
        function onMovesChanged(firstId) {
            console.log("moves changed: " + game.matchId);
            var moves = syncManager.movesDatabase.getMoves(firstId, game.matchId)
            pieceModel.endTurn();
//            console.log("Loading moves; " << moves);
            for (var i =0 ; i < moves.length; i++)
            {
                pieceModel.loadMove(pieceModel.getPiece(moves[i].fromPosition), moves[i].move)
            }
        }
        function onNewMove(fromPos, move, isMyTurn)
        {
            pieceModel.tempMove(fromPos, isMyTurn, true)
        }
        function confirmMoves()
        {
            pieceModel.confirmMoves();
        }

        function onDiceChanged(isWhite, dice1, dice2, currentAction)
        {
            pieceModel.setDice(isWhite, dice1, dice2, currentAction)
        }

        function onStartRoll(type)
        {
            if (type)
                pieceModel.setDice(-1,-1, true, 0)
            rollButton.visible = true;
        }

        function onNeedRoll(reroll)
        {
            showRoll()
        }

        function onShowDice(dice1, dice2, mixed)
        {
            pieceModel.setDisplayDice(dice1, dice2)
        }

        function onClearTemp()
        {
            pieceModel.endTurn();
            pieceModel.clearMoves();
        }

        function onFlashUndo()
        {
            rollButton.visible = false;
            doubleButton.visible = false;
            board.model.undoMoveAction()
            flashTimer.start()
        }

        function onOpponentDoubled() {
            doubleText.visible = true;
            acceptDoubleButton.visible = true;
            declineDoubleButton.visible = true;
            rollButton.visible = false;
            doubleButton.visible = false;
        }
    }

    function showRoll() {
        rollButton.visible = true;
        if ((board.model.whoseDouble == (syncManager.gamesDatabase.isWhite(viewContext.id) ? 1 : 0) || board.model.whoseDouble === 2) && syncManager.gamesDatabase.toScore(viewContext.id) > 1)
            doubleButton.visible = true;
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
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 2 : 23
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 3 : 22
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 4 : 21
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 5 : 20
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 6 : 19
                            model: pieceModel
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.preferredWidth: 44

                            Connections {
                                target: pieceModel
                                function onPositionsChanged()
                                {
                                    offPieceList.model = pieceModel.getOffPieces();
//                                    console.log("Off pieces changed: " + offPieceList.model)
                                }
                            }

                            ListView {
                                id: offPieceList
                                anchors.fill: parent
                                model: pieceModel.getOffPieces()
                                orientation: Qt.Vertical
                                function movePiece(piece)
                                {
//                                    console.log("Piece: " + piece)
                                    if (syncManager.turnIsWhite == piece.isWhite)
                                    {
//                                        console.log("Move off piece: " + piece.color);
                                        if (pieceModel.tempMove(-1, true, false, syncManager.gamesDatabase.isWhite(viewContext.id)))
                                        {
    //                                        syncManager.addMove(0, viewContext.id, -1, fromPos + moveFinal, 0, piece.isWhite, pieceModel.checkTook())
                                        }
                                    }
                                }
                                delegate: Component {
                                    Piece {
                                        height: Math.min(offPieceList.height/5, offPieceList.width)
                                        width: height
                                        color: modelData.isWhite ? "#FFFFFF" : "#353535"
                                        border.color: modelData.isWhite ? "#353535" : "#FFFFFF"
                                        onPieceMoved: {
                                            if (syncManager.turnIsWhite == piece.isWhite  && (piece.isWhite == syncManager.gamesDatabase.isWhite(viewContext.id)))
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
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 8 : 17
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 9 : 16
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 10 : 15
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 11 : 14
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 12 : 13
                            model: pieceModel
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
                        width: pieceModel.moves.length > 0 ? pieceModel.moves.length * 44 : pieceModel.displayDice.length * 44
                        height: parent.height
                        anchors.centerIn: parent
                        model: pieceModel.moves.length > 0 ? pieceModel.moves : pieceModel.displayDice
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
                                        if (syncManager.turnIsWhite)
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
                            visible: board.model.isTurnFinished && syncManager.turnIsWhite == syncManager.gamesDatabase.isWhite(viewContext.id)
                            onClicked:  {
                                board.model.userFinishedTurn();
//                                syncManager.turnFinished(viewContext.id);
//                                model.clearMoves();
//                                model.isTurnFinished = false
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
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 23 : 2
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 22 : 3
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 21 : 4
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 20 : 5
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 19 : 6
                            model: pieceModel
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
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 17 : 8
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 16 : 9
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 15 : 10
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 14 : 11
                            model: pieceModel
                        }

                        BoardColumn
                        {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            rotation: 180
                            index: syncManager.gamesDatabase.isWhite(viewContext.id) ? 13 : 12
                            model: pieceModel
                        }
                    }
                }
            }
        }
    }
}
