import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import 'Style'
import Forecast 1.0 // forecast model

ColumnLayout {
    id: root
    Layout.fillWidth: true

    Repeater {
        id: repeat
        height: 300
        width: parent.width
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter
        /*
        id: forecast
        width: parent.width
        height: 100
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter
        */
        //Layout.margins: Style.panel.margin
        //anchors.fill: parent
        //anchors.margins: Style.panel.margin


        model: ForecastModel {
            id: forecast_model
            weather: weather_item
        } // from cpp
        delegate: ForecastDelegate {
            id: forecast_delegate
        }
}

    /*
    ListView {
        id: forecast
        Layout.preferredWidth: parent.width
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter
        anchors.fill: parent
        anchors.margins: Style.panel.margin

        spacing: Style.forecast.list_spacing
        interactive: false
        //clip: true

        model: ForecastModel {
            id: forecast_model
            weather: weather_item
        } // from cpp
        delegate: ForecastDelegate {
            id: forecast_delegate
        }
    }
    */
}
