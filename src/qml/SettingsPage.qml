import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import Qt.labs.settings 1.0
import Database 1.0
import 'Style'

Page {
    id: root_settings
    title: qsTr("Settings")

    Settings {
        id: settings
        property alias font_size: font_scale_spin.value
        property alias radar_enabled: radar_check.checked
    }

    header: Rectangle {
        id: tool_bar
        Layout.fillWidth: true
        height: back_button.height
        color: Style.app.color

        RowLayout {
            id: tool_row
            anchors.fill: parent

            Button {
                id: back_button
                text: "Back"
                onClicked: {
                    stack_view.pop()
                }
            }
        }
    }

    Rectangle {
        id: content
        color: Style.app.color
        anchors.fill: parent

        ColumnLayout {
            id: column_layout
            width: parent.width


            Rectangle {
                id: font_scale
                color: Style.search.color_background
                implicitHeight: font_scale_spin.height
                implicitWidth: content.width

                Label {
                    text: 'Font Scale'
                    color: Style.settings.font_color
                    font.pixelSize: Style.settings.font_size
                    font.weight: Style.settings.font_weight
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                    anchors.margins: Style.settings.margin
                }

                SpinBox {
                    id: font_scale_spin
                    value: 100
                    from: 50
                    to: 200
                    Layout.alignment: Qt.AlignRight
                    anchors.right: parent.right
                    anchors.margins: Style.settings.margin
                }
            }


            Rectangle {
                id: radar_enabled
                color: Style.search.color_background
                radius: Style.panel.radius_background
                implicitHeight: radar_check.height
                implicitWidth: content.width

                Label {
                    text: 'Radar Enabled'
                    color: Style.settings.font_color
                    font.pixelSize: Style.settings.font_size
                    font.weight: Style.settings.font_weight
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                    anchors.margins: Style.settings.margin
                }

                CheckBox {
                    id: radar_check
                    checked: true
                    Layout.alignment: Qt.AlignRight
                    anchors.right: parent.right
                    anchors.margins: Style.settings.margin
                }
            }


        }
    }

}

