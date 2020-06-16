import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import 'Style'
import RadarImage 1.0

Item {
    id: panel
    Layout.preferredHeight: 300
    Layout.preferredWidth: parent.width
    Layout.fillWidth: true
    Layout.alignment: Qt.AlignCenter

    onWidthChanged: {
        // keep square
        panel.height = panel.width
    }

    Item {
        Layout.preferredWidth: parent.width
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter
        anchors.fill: parent
        anchors.margins: Style.panel.margin

        Rectangle {
            id: background
            color: Style.panel.color
            radius: Style.panel.radius
            anchors.fill: parent
        }

        RadarImage {
            id: image
            //fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            anchors.margins: Style.today.margin
            weather: weather_item
        }
    }

}
