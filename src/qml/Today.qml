import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import "Style"

Rectangle {
    id: root
    color: Style.today.color_day
    height: root_layout.height
    width: parent.width
    Layout.fillWidth: true

    RowLayout {
        id: root_layout
        Layout.fillWidth: true
        width: parent.width

        ColumnLayout {
            id: left_column
            Layout.alignment: Qt.AlignTop
            Layout.margins: Style.today.margin

            Item {
                Layout.alignment: Qt.AlignHCenter
                width: Style.today.image_size
                height: Style.today.image_size

                Image {
                    id: image
                    source: "qrc:/sunny.svg"
                    sourceSize.width: Style.today.image_size
                    sourceSize.height: Style.today.image_size
                }

                DropShadow {
                    anchors.fill: image
                    horizontalOffset: 4
                    verticalOffset: 4
                    radius: 8
                    samples: 10
                    color: Style.today.color_shadow
                    source: image
                }
            }

            Label {
                id: temp
                font.pixelSize: Style.today.font_size_temp
                font.weight: Style.today.font_weight_temp
                color: Style.today.font_color
                text: ""
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignBottom
                //Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Label {
                id: feel
                font.pixelSize: Style.today.font_size
                color: Style.today.font_color
                text: ""
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignBottom
                //Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        ColumnLayout {
            id: right_column
            Layout.alignment: Qt.AlignTop | Qt.AlignRight
            Layout.fillHeight: true
            Layout.margins: Style.today.margin

            Label {
                id: place
                text: ""
                color: Style.today.font_color
                font.weight: Style.today.font_weight_place
                font.pixelSize: Style.today.font_size_place
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignBottom
                Layout.fillWidth: true
                Layout.fillHeight: true
            }


            Label {
                id: wind
                font.pixelSize: Style.today.font_size
                text: ""
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignBottom
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Label {
                id: rain
                font.pixelSize: Style.today.font_size
                text: ""
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignBottom
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Label {
                id: humid
                font.pixelSize: Style.today.font_size
                text: ""
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignBottom
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Label {
                id: temperature
                font.pixelSize: Style.today.font_size
                text: ""
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignBottom
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Label {
                id: summary
                font.pixelSize: Style.today.font_size
                text: ""
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignBottom
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

        }
    }

    
    function updateObservation(xData) {
        place.text = xData['name'];
        feel.text = 'Feels like ' + xData['apparent_t'] + '°';
        wind.text = xData['wind_spd_kmh'] + 'kmh ' + xData['wind_spd_kt'] + 'kt ' + xData['wind_dir'];
        humid.text = xData['rel_hum'] + '% Humid';
        rain.text = xData['rain_trace'] + 'mm Rain';
        temp.text = xData['air_temp'] + '°';
    }

    function updateForecast(xData) {
        var forecast = xData['forecast'][0];
        image.source = 'qrc:/' + forecast['forecast_icon'];
        summary.text = forecast['precis'];

        var min_temp = forecast['air_temperature_minimum']
        var max_temp = forecast['air_temperature_maximum']
        if (min_temp && max_temp)
            temperature.text = min_temp + ' ' + max_temp + '° Max';
        else if (max_temp)
            temperature.text = max_temp + '° Max';
        else
            temperature.text = temp.text + ' Max';
    }

}

