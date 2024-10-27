import QtQuick 2.15

Item {
    id: piece
    property alias color: visual.color
    property alias border: visual.border
    property var pieceVar: modelData

    signal pieceMoved(var piece)
    z: 10

    Drag.active: mouseArea.drag.active
    Drag.hotSpot.x: piece.width/2
    Drag.hotSpot.y: piece.height/2
    anchors.horizontalCenter: mouseArea.drag.active ? undefined : parent.horizontalCenter

    Rectangle {
        id: visual
        anchors.fill: parent
        radius: height/2
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        preventStealing: true
//        onPressed:
//        {
//            drag.target = parent
//            drag.target.parent = root
//        }

//        onReleased:
//        {
//            drag.target = null
//            piece.Drag.drop()
//        }

        // test clicking piece that has already move to see if it tries to move again with no moves
        onClicked: {
            piece.pieceMoved(modelData)
        }
    }
}
