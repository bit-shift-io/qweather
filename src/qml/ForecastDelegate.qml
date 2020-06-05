import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import  'Style'

ItemDelegate {
    property variant days_of_week : [1,2,3,4,5,6,0] // fixed
    id: forecast_delegate
    width: parent.width
    checkable: true
    onClicked: ListView.view.currentIndex = index

    RowLayout {
        id: forecast_component
        Layout.fillWidth: true

        Label {
            id: day
            text: "Monday"
            color: Style.panel.font_color
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        Label {
            id: rain
            text: "1mm"
            color: Style.forecast.color_rain
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        Label {
            id: temp_high
            text: "15C"
            color: Style.forecast.color_temp_high
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        Label {
            id: temp_low
            text: "10C"
            color: Style.forecast.color_temp_low
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }
    }
}

