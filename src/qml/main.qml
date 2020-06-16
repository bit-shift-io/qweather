import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import "util.js" as Util
import Weather 1.0
import Database 1.0
import 'Style'

ApplicationWindow {
    id: root
    title: Qt.application.name
    width: 350
    height: 500
    visible: true
    color: Style.app.color

    // todo: store this in settings
    property var places: []

    Component.onCompleted: {
        //console.log("app load complete");
    }


    Weather {
        // weather object
        id: weather_item
        station: '94672'
        onResultObservationFinished: {
            today.updateObservation(xResult);
        }
        onResultForecastFinished: {
            today.updateForecast(xResult);
        }
        onResultRadarFinished: {
        }
    }


    Timer {
        // Refresh the observations every 5 minutes
        interval: 300000
        repeat: true
        triggeredOnStart: true
        running: true
        onTriggered: {
            weather_item.requestObservation();
            weather_item.requestForecast();
            weather_item.requestRadar();
        }
    }

    ScrollView {
        // main page layout
        id: scroll_view
        anchors.fill: parent


        ColumnLayout {
            spacing: 0
            anchors.fill: parent
            width: Math.max(implicitWidth, scroll_view.availableWidth)
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true

            Today {
                id: today
            }

            Forecast {
                id:forecast
                property variant weather_station: weather_item
            }

            //GraphicForecast {}

            Radar {
                id: radar
                property variant weather_station: weather_item
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
