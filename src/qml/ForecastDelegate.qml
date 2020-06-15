import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import  'Style'

ItemDelegate {
    id: forecast_delegate
    width: parent.width
    Layout.preferredWidth: parent.width
    Layout.fillWidth: true
    Layout.alignment: Qt.AlignCenter

    RowLayout {
        width: parent.width
        Layout.fillWidth: true

        ColumnLayout {
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: false

            Label {
                id: day
                text: model.day
                color: Style.panel.font_color
                font.pointSize: Style.forecast.font_size_day
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: true
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignRight
            Layout.fillWidth: false

            RowLayout {
                Layout.alignment: Qt.AlignRight

                ColumnLayout {
                    Label {
                        id: description
                        text: model.description
                        color: Style.forecast.color_description
                        horizontalAlignment: Text.AlignRight
                        Layout.fillWidth: true
                    }

                    Label {
                        id: rain
                        text: model.rainProbability + "% chance of " + model.rainRange + "mm"
                        color: Style.forecast.color_rain
                        horizontalAlignment: Text.AlignLeft
                        Layout.fillWidth: true
                    }
                }

                Label {
                    id: temp_low
                    text: model.temperatureMinimum
                    color: Style.forecast.color_temp_low
                    font.pointSize: Style.forecast.font_size_temp
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: false
                }

                Label {
                    id: temp_high
                    text: model.temperatureMaximum
                    color: Style.forecast.color_temp_high
                    font.pointSize: Style.forecast.font_size_temp
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: false
                }
            }

        }

    }
}

