import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import  'Style'


Rectangle {
    id: root
    color: Style.forecast.color_background
    radius: Style.forecast.radius_background
    implicitHeight: image.height + (Style.panel.margin * 2)
    width: parent.width
    Layout.fillWidth: true

    RowLayout {
        id: root_layout
        Layout.fillWidth: true
        anchors.fill: parent
        anchors.margins: Style.panel.margin

        ColumnLayout {
            id: day_column
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: false

            RowLayout {
                Image {
                    id: image
                    source: 'qrc:/' + model.icon
                    sourceSize.width: temp_high.height
                    sourceSize.height: temp_high.height
                }

                Label {
                    id: day
                    text: model.day
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
                    text: model.temperatureMinimum
                    color: Style.forecast.color_temp_low
                    font.pixelSize: Style.forecast.font_size_temp
                    font.weight: Style.forecast.font_weight_temp
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: false
                }

                Label {
                    id: temp_high
                    text: model.temperatureMaximum
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

