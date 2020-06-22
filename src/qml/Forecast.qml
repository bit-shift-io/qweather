import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import 'Style'
import Forecast 1.0 // forecast model

ColumnLayout {
    id: root
    Layout.fillWidth: true
    spacing: Style.panel.margin_internal

    Repeater {
        id: repeat
        width: parent.width
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter

        model: ForecastModel {
            id: forecast_model
            weather: weather_item
        } // from cpp
        delegate: ForecastDelegate {
            id: forecast_delegate
        }
    }
}
