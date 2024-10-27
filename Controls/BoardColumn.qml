import QtQuick 2.15

Item {
    property var index
    property var model
//    color: "pink"
    z: 0

    Connections {
        target: model
        function onPositionsChanged()
        {
            pieceList.model = model.getPieces(index);
        }
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: {
            console.log("temp move not from server " + model)
            model.positionsChanged()
        }
    }

    function movePiece(piece)
    {
        var fromPos = piece.position;
        if (model.started)
            model.boardMove(fromPos, true, false, piece.isWhite)
    }

    ListView {
        id: pieceList
        anchors.fill: parent
        model: model.getPieces(index)
        interactive: false
        delegate: Component {
            Piece {
                height: Math.min(pieceList.height/5, pieceList.width)
                width: height
                color: modelData.isWhite ? consts.lightPieceColor : consts.darkPieceColor
                border.color: modelData.isWhite ? consts.darkPieceColor : consts.lightPieceColor
                onPieceMoved: {
                    movePiece(piece)
                }
            }
        }
    }
}
