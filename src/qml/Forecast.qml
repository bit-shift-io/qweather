import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import 'Style'
import Forecast 1.0 // forecast model

Item {
    Layout.preferredHeight: 250
    Layout.preferredWidth: parent.width
    Layout.fillWidth: true
    Layout.alignment: Qt.AlignCenter

    ListView {
        Layout.preferredWidth: parent.width
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter

        // disable input
        interactive: false
        //clip: true

        id: forecast
        anchors.fill: parent
        anchors.margins: Style.panel.margin

        model: ForecastModel {
            id: forecast_model
            weather: weather_item
        } // from cpp
        delegate: ForecastDelegate {
            id: forecast_delegate
        }
    }
}
