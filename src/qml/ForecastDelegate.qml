import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami

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
            font.pixelSize: Qt.application.font.pixelSize
            text: "Monday"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        Label {
            id: temp_high
            font.pixelSize: Qt.application.font.pixelSize
            text: "15C"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        Label {
            id: temp_low
            font.pixelSize: Qt.application.font.pixelSize
            text: "10C"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }
    }
}

