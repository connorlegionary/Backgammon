#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "src/column.h"
#include "src/database/settingsdatabase.h"
#include "src/piecemodel.h"
#include "src/gamemodel.h"
#include "analysis/playbackmodel.h"

#include "src/database/puzzledatabase.h"
#include "src/database/puzzlemovedatabase.h"
#include "src/database/movesdatabase.h"
#include "src/database/gamesdatabase.h"
#include "src/database/computergamedatabase.h"

#include "puzzles/puzzlemodel.h"

#include "src/networking/syncmanager.h"


#include "analysis/analysisthread.h"
#include "analysis/positionmodel.h"

//#include <QtAdMob/QtAdMobBanner.h>
//#include <QtAdMob/QtAdMobInterstitial.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<Column>("com.backgammon.controls", 1, 0, "BackgammonColumn");

    qmlRegisterType<AnalysisThread>("com.backgammon.analysis", 1, 0, "GameAnalysis");
    qmlRegisterType<PositionModel>("com.backgammon.analysis", 1, 0, "PositionModel");
    qmlRegisterType<PositionSort>("com.backgammon.analysis", 1, 0, "PositionSort");

    qmlRegisterType<GamesDatabase>("com.backgammon.database", 1, 0, "GamesDatabase");
    qmlRegisterType<ComputerGamesDatabase>("com.backgammon.database", 1, 0, "ComputerGamesDatabase");
    qmlRegisterType<MovesDatabase>("com.backgammon.database", 1, 0, "MovesDatabase");
    qmlRegisterType<PuzzleDatabase>("com.backgammon.database", 1, 0, "PuzzleDatabase");
    qmlRegisterType<PuzzleMoveDatabase>("com.backgammon.database", 1, 0, "PuzzleMoveDatabase");

    qmlRegisterType<PieceModel>("com.backgammon.models", 1, 0, "PieceModel");
    qmlRegisterType<GameModel>("com.backgammon.models", 1, 0, "GameModel");
    qmlRegisterType<ComputerPieceModel>("com.backgammon.models", 1, 0, "ComputerPieceModel");
    qmlRegisterType<PlaybackModel>("com.backgammon.models", 1, 0, "PlaybackModel");
    qmlRegisterType<PuzzleModel>("com.backgammon.models", 1, 0, "PuzzleModel");
    qmlRegisterType<PuzzlePieceModel>("com.backgammon.models", 1, 0, "PuzzlePieceModel");
    qmlRegisterType<SyncManager>("com.backgammon.network", 1, 0, "SyncManager");

//    QmlAdMobBanner::DeclareQML();			// <== Call it before loading qml
//    QmlAdMobInterstitial::DeclareQML();		// <== Call it before loading qml

    ActionDatabase* actionDb = new ActionDatabase();
    SettingsDatabase* db = new SettingsDatabase();
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("settings", db);
    engine.rootContext()->setContextProperty("actionsDatabase", actionDb);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
