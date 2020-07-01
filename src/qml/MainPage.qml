import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import Weather 1.0
import Database 1.0
import 'Style'

Page {
    id: root

    property string weather_station: ""

    Weather {
        // weather object
        id: weather_item
        station: weather_station
        onResultObservationFinished: {
            today.updateObservation(xResult);
        }
        onResultForecastFinished: {
            today.updateForecast(xResult);
        }
    }


    Timer {
        // Refresh the observations every x minutes
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

    Rectangle {
        id: background
        color: Style.app.color
        anchors.fill: parent

        ScrollView {
            // main page layout
            id: scroll_view
            anchors.fill: parent
            width: parent.width
            height: parent.height
            Layout.fillHeight: true
            Layout.fillWidth: true
            //Flickable.boundsBehavior: Flickable.DragOverBounds
            /*
            onContentY: {

            }*/
            contentItem.onYChanged: {
                console.log("scroll")
            }
            /*
            Component.onCompleted {

            }*/

            ColumnLayout {
                spacing: 0
                anchors.fill: parent
                width: parent.width //Math.max(implicitWidth, scroll_view.availableWidth)
                Layout.fillWidth: true
                Layout.fillHeight: true

                Today {
                    id: today

                    MouseArea {
                        anchors.fill: parent
                        onClicked: { drawer.open() }
                    }
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

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
