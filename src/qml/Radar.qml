import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import 'Style'
import RadarImage 1.0

Rectangle {
    id: root
    color: Style.forecast.color_background
    radius: Style.forecast.radius_background
    implicitHeight: parent.width
    width: parent.width
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.alignment: Qt.AlignCenter

    RadarImage {
        id: image
        //fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        anchors.margins: Style.today.margin
        weather: weather_item
    }
}
