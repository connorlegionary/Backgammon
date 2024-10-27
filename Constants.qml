import QtQuick 2.15

Item {
    id: consts

    property var getGamesResponse: 0
    property var addGameResponse: 1
    property var loginResponse: 2
    property var createUserResponse: 3
    property var deleteGameResponse: 4
    property var joinGameResponse: 5

    property color backgroundColor: Qt.rgba(76/255,124/255,220/255)
    property color lightColumnColor: Qt.rgba(92/255,125/255,180/255)
    property color darkColumnColor: Qt.rgba(52/255,86/255,172/255)
    property color darkPieceColor: Qt.rgba(43/255, 49/255, 166/255)
    property color lightPieceColor: Qt.rgba(218/255, 221/255, 243/255)
    property color boardBackground: Qt.rgba(5/255,1/255,34/255)

    property color activeRed: Qt.rgba(197/255,48/255,65/255)

    property color black: "black"
    property color white: "white"

    function msToTime(ms)
    {
//        console.log("MsToTime: " + ms)
        if (ms >= 86400000) // 1 day
        {
            return Math.floor(ms/86400000) + " days"
        }

        if (ms < 60*1000) {return msToSeconds(ms);}
        else if (ms < 60*60*1000) {return msToMinutes(ms);}
        else {return msToHours(ms);}
    }

    function msToHours(duration) {

      var seconds = Math.floor((duration / 1000) % 60),
        minutes = Math.floor((duration / (1000 * 60)) % 60),
        hours = Math.floor((duration / (1000 * 60 * 60)) % 24);

      minutes = (minutes < 10) ? "0" + minutes : minutes;
      seconds = (seconds < 10) ? "0" + seconds : seconds;

      return hours + ":" + minutes + ":" + seconds;
    }

    function msToMinutes(ms) {
      var seconds = Math.floor((ms / 1000) % 60),
        minutes = Math.floor((ms / (1000 * 60)) % 60);
      seconds = (seconds < 10) ? "0" + seconds : seconds;

      return minutes + ":" + seconds;
    }

    function msToSeconds(ms) {
      var milliseconds = settings.accuracy == 1 ? Math.floor(ms % 1000) : Math.floor((ms % 1000) / 100),
        centiseconds = Math.floor(ms % 100),
        seconds = Math.floor((ms / 1000) % 60);

        milliseconds = ( settings.accuracy != 1 && centiseconds >= 50)  ? milliseconds + 1 : milliseconds;
        seconds = ( settings.accuracy != 1 && milliseconds >= 10  ) ? seconds + 1 : seconds;
        milliseconds = (milliseconds >= 10 && settings.accuracy != 1) ? (milliseconds % 10) : milliseconds;
        milliseconds = (milliseconds < 10 && settings.accuracy == 1) ? "00" + milliseconds : milliseconds;
        milliseconds = (milliseconds < 100 && settings.accuracy == 1 && milliseconds >= 10  ) ? "0" + milliseconds : milliseconds;


      return seconds + "." + milliseconds;
    }
}
