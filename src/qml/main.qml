import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
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
    }


    Timer {
        // Refresh the observations every 5 minutes
        // 300000 = 5mins
        // 900000 = 15mins
        interval: 900000
        repeat: true
        triggeredOnStart: true
        running: true
        onTriggered: {
            weather_item.requestObservation();
            weather_item.requestForecast();
            weather_item.requestRadar();
            weather_item.requestDetailedForecast();
        }
    }

    ScrollView {
        // main page layout
        id: scroll_view
        anchors.fill: parent
        width: parent.width
        height: parent.height
        Layout.fillHeight: true
        Layout.fillWidth: true

        ColumnLayout {
            spacing: 0
            anchors.fill: parent
            width: parent.width //Math.max(implicitWidth, scroll_view.availableWidth)
            Layout.fillWidth: true
            Layout.fillHeight: true

            Today {
                id: today
            }

            Spacer {}

            Forecast {
                id:forecast
                property variant weather_station: weather_item
            }

            // GraphicForecast {}

            Spacer {}

            Radar {
                id: radar
                property variant weather_station: weather_item
            }

        }
    }
}
