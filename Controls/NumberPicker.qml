import QtQuick 2.15

ListView
{
    id: picker
    model: 100
    spacing: 8
    clip: true
    highlightFollowsCurrentItem: true
    orientation: Qt.Horizontal
    preferredHighlightBegin: width/2 - 33
    preferredHighlightEnd: width/2 + 33
    property bool showOff: false
    highlightRangeMode: ListView.StrictlyEnforceRange

    footer: Component {
        Item {
            width: picker.width/2
            height: picker.height
        }
    }

    header: Component {
        Item {
            width: picker.width/2
            height: picker.height
        }
    }

    delegate: Component {
        Item {
            width: 66
            height: 66

            Text {
                anchors.centerIn: parent
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
                text: showOff && modelData === 0 ? "Off" : modelData
                color: picker.currentIndex === index ? consts.activeRed : "white"
                font.pixelSize: picker.currentIndex === index ? 32 : 16
                font.bold: true
            }
        }
    }
}
