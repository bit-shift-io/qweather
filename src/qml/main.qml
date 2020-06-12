import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import "util.js" as Util
import Weather 1.0
import Stations 1.0
import 'Style'

ApplicationWindow {
    id: window
    title: Qt.application.name
    width: 350
    height: 500
    visible: true
    color: Style.app.color

    // todo: store this in settings
    property var places: []

    Component.onCompleted: {
        console.log("app load complete");
    }


    Weather {
        // weather object
        id: weather
        url: '94672'
        onResultFinished: {
            today.update(xResult);
        }
        Component.onCompleted: {
            weather.requestWeather();
        }
    }


    Timer {
        // Refresh the forecast every 5 minutes
        interval: 300000
        repeat: true
        triggeredOnStart: true
        running: true
        onTriggered: {
            weather.requestWeather();
        }
    }


    ScrollView {
        // main page layout
        id: scroll_view
        anchors.fill: parent

        ColumnLayout {
            spacing: 10
            anchors.fill: parent

            Today {
                id: today
            }

            Forecast {
                id:forecast
            }

            GraphicForecast {}

            Radar {
                id: radar
            }
        }
    }

}

/*
Kirigami.ApplicationWindow {
    id: window
    visible: true
    title: application.name
    width: 350
    height: 500

    pageStack.initialPage: Qt.resolvedUrl("WeatherPage.qml")

    globalDrawer: Kirigami.GlobalDrawer {
        title: "Hello App"
        titleIcon: "applications-graphics"
        actions: [
            Kirigami.Action {
                text: "Add location"
            },
            Kirigami.Action {
                text: "Remove location"
            }
            ]
    }

}
*/
