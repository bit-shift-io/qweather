import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import  'Style'


Rectangle {
    id: root
    color: Style.forecast.color_background
    radius: Style.forecast.radius_background
    height: 30
    width: parent.width
    Layout.fillWidth: true


    RowLayout {
        id: root_layout
        Layout.fillWidth: true
        //anchors.fill: parent
        //anchors.margins: 4
        //Layout.preferredWidth: parent.width

        ColumnLayout {
            id: day_column
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: false

            RowLayout {
                Image {
                    id: image
                    source: 'qrc:/' + model.icon
                    sourceSize.width: day.height
                    sourceSize.height: day.height
                }

                Label {
                    id: day
                    text: model.day
                    color: Style.panel.font_color
                    font.pixelSize: Style.forecast.font_size_day
                    font.weight: Style.forecast.font_weight_day
                    horizontalAlignment: Text.AlignLeft
                    Layout.fillWidth: true
                }
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignRight
            Layout.fillWidth: false

            RowLayout {
                Layout.alignment: Qt.AlignRight

                Label {
                    id: rain
                    font.pixelSize: Style.forecast.font_size_description
                    text: model.rainProbability + "% chance " + model.rainRange + "mm "
                    visible: (model.rainRange !== "")
                    color: Style.forecast.color_rain
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                }

                Label {
                    id: description
                    text: model.description + " "
                    font.pixelSize: Style.forecast.font_size_description
                    visible: (model.rainRange === "") // when rain is visible, hide this
                    color: Style.forecast.color_description
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true
                }

                Label {
                    id: temp_low
                    text: model.temperatureMinimum + '°'
                    color: Style.forecast.color_temp_low
                    font.pixelSize: Style.forecast.font_size_temp
                    font.weight: Style.forecast.font_weight_temp
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: false
                }

                Label {
                    id: temp_high
                    text: model.temperatureMaximum + '°'
                    color: Style.forecast.color_temp_high
                    font.pixelSize: Style.forecast.font_size_temp
                    font.weight: Style.forecast.font_weight_temp
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: false
                }
            }

        }

    }
}

