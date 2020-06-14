import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import "Style"

Item {
    height: 200
    width: parent.width
    Layout.fillWidth: true

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
                text: "12C"
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: true
            }

            Label {
                id: feel
                color: Style.today.font_color
                text: "Feels like 23C"
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: true
            }
        }

        ColumnLayout {

            Label {
                id: place
                text: "Adelaide"
                color: Style.today.font_color
                font.pointSize: Style.today.font_size_place
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }


            Label {
                id: wind
                text: "12km 8kt ssw"
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }

            Label {
                id: rain
                text: "1mm"
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }

            Label {
                id: humid
                text: "18%"
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }

            Label {
                id: temp_range
                text: "12C 24C"
                color: Style.today.font_color
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }

            Label {
                id: summary
                text: "Warm and sunny"
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
        image.source = 'qrc:/' + xData['cloud'] + '.svg';
    }

}

