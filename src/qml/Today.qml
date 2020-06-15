import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import "Style"

Item {
    //height: 200
    Layout.preferredHeight: 200
    Layout.preferredWidth: parent.width
    //width: parent.width
    Layout.fillWidth: true
    Layout.alignment: Qt.AlignCenter


    Rectangle {
        id: background
        color: Style.today.color_day
        anchors.fill: parent
    }

    DropShadow {
        anchors.fill: background
        horizontalOffset: 0
        verticalOffset: 0
        radius: Style.shadow.radius
        samples: Style.shadow.samples
        color: Style.shadow.color
        source: background
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: Style.today.margin

        ColumnLayout {
            Image {
                id: image
                source: "qrc:/sunny.svg"
                sourceSize.width: Style.today.image_size
                sourceSize.height: Style.today.image_size
            }

            Label {
                id: temp
                font.pointSize: Style.today.font_size_temp
                color: Style.today.font_color
                text: ""
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: true
            }

            Label {
                id: feel
                color: Style.today.font_color
                text: ""
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: true
            }
        }

        ColumnLayout {

            Label {
                id: place
                text: ""
                color: Style.today.font_color
                font.pointSize: Style.today.font_size_place
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }


            Label {
                id: wind
                text: ""
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }

            Label {
                id: rain
                text: ""
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }

            Label {
                id: humid
                text: ""
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }

            RowLayout {
                //SizeBox{}

                Label {

                    id: temp_min
                    text: ""
                    color: Style.today.font_color
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                }

                Label {
                    id: space
                    text: " "
                    color: Style.today.font_color
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                }

                Label {
                    id: temp_max
                    text: ""
                    color: Style.today.font_color
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                }
            }

            Label {
                id: summary
                text: ""
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }

        }

    }
    
    function updateObservation(xData) {
        place.text = xData['name'];
        temp.text = xData['air_temp'] + '°C';
        feel.text = 'Feels like ' + xData['apparent_t'] + '°C';
        wind.text = xData['wind_spd_kmh'] + 'kmh ' + xData['wind_spd_kmh'] + 'kt ' + xData['wind_dir'];
        humid.text = xData['rel_hum'] + '%';
        rain.text = xData['rain_trace'] + 'mm';
        //image.source = 'qrc:/' + xData['cloud'] + '.svg';
    }

    function updateForecast(xData) {
        var today = xData['forecast'][0];
        //image.source = today['forecast_icon_code'];
        temp_max.text = today['air_temperature_maximum'] + '°C';
        summary.text = today['precis'];
    }

}

