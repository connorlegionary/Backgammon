import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

import com.backgammon.network 1.0
import com.backgammon.database 1.0

//import com.dreamdev.QtAdMobBanner 1.0
//import com.dreamdev.QtAdMobInterstitial 1.0

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Backgammon")

    property var syncManager: sync
    property var gamesDatabase: games
    property var movesDatabase: movesDb
    property var messages: messageTypes

    MessageTypes {
        id: messageTypes
    }

    Constants {
        id: consts
    }

    SyncManager {
        id: sync
        settingsDatabase: settings
        onRequestFailed:
        {
//            if (messageType == -1)
//                pushOverlay("qrc:/Overlays/WarningMessage.qml", {title: "Server Failed to respond", image: "qrc:/Images/warning.png", text: "You have no internet connection or the server is down!"})
        }
    }

    MovesDatabase {
        id: movesDb

        Component.onCompleted: syncManager.movesDatabase = movesDb
    }

    GamesDatabase {
        id: games

        Component.onCompleted: syncManager.gamesDatabase = games
    }


    PuzzleDatabase {
        id: puzzleDb

        Component.onCompleted: syncManager.puzzlesDatabase = puzzleDb
    }

    function pushView(source, context, close)
    {
        mainViewLoader.views.push({source: source, context: context})
        refreshView()
        if (close)
            closeOverlay()
    }

    // Used in games to automatically revert to previous screen, every screen past the screen you want to go back to must use replace instead of push.
    function replaceView(source, context)
    {
        mainViewLoader.views.pop()
        mainViewLoader.views.push({source: source, context: context})
        refreshView()
    }

    Component.onCompleted:
    {
        pushView("qrc:/Views/Login.qml")
    }

    function refreshView()
    {
        mainViewLoader.viewContext = mainViewLoader.views[mainViewLoader.views.length - 1].context
        mainViewLoader.source = mainViewLoader.views[mainViewLoader.views.length - 1].source
    }

    function popView()
    {
        mainViewLoader.views.pop();
        refreshView()
    }

    function pushOverlay(source, context, closeOnClick)
    {
        overlayLoader.viewContext = context
        overlayLoader.source = source
        overlay_background.visible = true
        if (closeOnClick !== undefined)
            closeMouseArea.close = closeOnClick
        else
            closeMouseArea.close = true
    }

    function closeOverlay()
    {
        overlayLoader.source = ""
        overlay_background.visible =false
    }

    Item
    {
        anchors.left: parent.left
        anchors.top: parent.top //messageArea.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Loader
        {
            id: mainViewLoader
            anchors.fill: parent

            property var views: []

            z: 1

            property var viewContext: views[views.length - 1].context

            source: views[views.length - 1].source
        }

        Item
        {
            anchors.fill: parent
            z: 2

            Rectangle
            {
                id: overlay_background
                anchors.fill: parent
                color: "black"
                opacity: 0.5
                visible: false

                MouseArea
                {
                    id: closeMouseArea
                    anchors.fill: parent
                    property bool close: true
                    onClicked:
                    {
                        if (close)
                            closeOverlay()
                    }
                }
            }

            Loader
            {
                id: overlayLoader
                anchors.fill: parent

                property var viewContext
            }
        }
    }

//    AdMobInterstitial
//    {
//        id: intertitial

//        Component.onCompleted: {
//            intertitial.unitId = "ca-app-pub-8331428861021262/7366952539"
//        }

//        onClosed: {
//            intertitial.unitId = "ca-app-pub-8331428861021262/7366952539"
//        }

//        onLoaded: {
//            interstitialButton.enabled = true
//            interstitialButton.text = qsTr("Show")
//        }

//        onLoading: {
//            interstitialButton.enabled = false
//            interstitialButton.text = qsTr("Loading...")
//        }
//    }
}
