QT += quick sql

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

android: {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/platform/android
}
#include(QtAdMob/QtAdMob.pri)

#DEFINES += QTADMOB_QML

CONFIG -= bitcode

SOURCES += \
        ../../Projects/Backgammon/puzzles/puzzlemodel.cpp \
        analysis/analtypes.cpp \
        analysis/analysis.cpp \
        analysis/analysisthread.cpp \
        analysis/playbackmodel.cpp \
        analysis/positionmodel.cpp \
        diceroll.cpp \
        main.cpp \
        puzzles/puzzledata.cpp \
        puzzles/puzzlemove.cpp \
        src/column.cpp \
        src/database/action.cpp \
        src/database/actiondatabase.cpp \
        src/database/computergame.cpp \
        src/database/computergamedatabase.cpp \
        src/database/game.cpp \
        src/database/gamesdatabase.cpp \
        src/database/move.cpp \
        src/database/movesdatabase.cpp \
        src/database/puzzledatabase.cpp \
        src/database/puzzlemovedatabase.cpp \
        src/database/settingsdatabase.cpp \
        src/gamemodel.cpp \
        src/piece.cpp \
        src/piecelist.cpp \
        src/piecemodel.cpp \
        src/database/database.cpp \
        src/database/logindatabase.cpp \
        src/networking/syncmanager.cpp

RESOURCES += qml.qrc \
             images/images.qrc

VERSION=1.0.0

ios {
    TARGET = backgammon
    QMAKE_TARGET_BUNDLE_PREFIX = com.connortest
    QMAKE_INFO_PLIST = info.plist
}


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../../Projects/Backgammon/puzzles/puzzlemodel.h \
    analysis/analtypes.h \
    analysis/analysis.h \
    analysis/analysisthread.h \
    analysis/playbackmodel.h \
    analysis/positionmodel.h \
    diceroll.h \
    puzzles/puzzledata.h \
    puzzles/puzzlemove.h \
    src/column.h \
    src/database/action.h \
    src/database/actiondatabase.h \
    src/database/computergame.h \
    src/database/computergamedatabase.h \
    src/database/game.h \
    src/database/gamesdatabase.h \
    src/database/move.h \
    src/database/movesdatabase.h \
    src/database/puzzledatabase.h \
    src/database/puzzlemovedatabase.h \
    src/database/settingsdatabase.h \
    src/gamemodel.h \
    src/piece.h \
    src/piecelist.h \
    src/piecemodel.h \
    src/logindatabase.h \
    src/database/database.h \
    src/database/logindatabase.h \
    src/networking/syncmanager.h

android:
{
        DISTFILES += \
                $$ANDROID_PACKAGE_SOURCE_DIR/src/org/dreamdev/QtAdMob/QtAdMobActivity.java \
    ../../Projects/Backgammon/platform/android/AndroidManifest.xml \
    ../../Projects/Backgammon/platform/android/build.gradle \
    ../../Projects/Backgammon/platform/android/gradle.properties \
    ../../Projects/Backgammon/platform/android/gradlew \
    ../../Projects/Backgammon/platform/android/gradlew.bat \
    ../../Projects/Backgammon/platform/android/res/values/libs.xml \
    Info.plist \
}
