import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import Database 1.0
import 'Style'

Page {
    id: root
    title: qsTr("Find Location")

    Rectangle {
        id: background
        color: Style.app.color
        anchors.fill: parent

        ColumnLayout {
            id: root_layout
            anchors.fill: parent

            Item {
                id: tool_bar
                Layout.fillWidth: true
                height: back_button.height

                RowLayout {
                    id: tool_row
                    anchors.fill: parent

                    Button {
                        id: back_button
                        text: "Back"
                        font.pixelSize: Style.forecast.font_size_day
                        font.weight: Style.forecast.font_weight_day
                        onClicked: {
                            stack_view.pop()
                        }
                    }
                    TextField {
                        id: search_input
                        placeholderText: qsTr("Location, Sate or Lon Lat")
                        font.pixelSize: Style.forecast.font_size_day
                        font.weight: Style.forecast.font_weight_day
                        cursorVisible: true
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        //background: {}
                        onTextChanged: {
                            if (search_input.text.length < 2)
                                return;

                            var result = Database.findStation(search_input.text);
                            if (result.length === 0)
                                return;

                            search_model.clear();

                            for (var i = 0; i < result.length; i++) {
                                var station = result[i];
                                var json = {"name": station[Database.NAME], "state": station[Database.STATE], "wmo": station[Database.WMO]};
                                search_model.append(json);
                            }
                        }
                    }
                }
            }

            ListView {
                id: search_result
                width: parent.width
                boundsBehavior: Flickable.DragOverBounds
                //highlightRangeMode: ListView.NoHighlightRange
                //snapMode: ListView.SnapToItem
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                spacing: 4
                highlight: highlight

                onCurrentItemChanged: {
                      //Myselect(search_result.currentIndex)
                      console.log("index changed see this " + currentIndex)
                }

                model:  ListModel {
                    id: search_model
                }

                delegate: Component {
                    id: search_delegate

                    Rectangle {
                        id: background
                        color: ListView.isCurrentItem? 'red' : Style.forecast.color_background
                        radius: Style.forecast.radius_background
                        implicitHeight: root_layout.height
                        implicitWidth: parent.width

                        RowLayout {
                            id: root_layout
                            Layout.fillWidth: true
                            width: parent.width

                            ColumnLayout {
                                id: test
                                implicitWidth: parent.width
                                Layout.margins: Style.today.margin
                                spacing: 10
                                Layout.alignment: Qt.AlignLeft

                                Label {
                                    text: model.name
                                    color: Style.panel.font_color
                                    font.pixelSize: Style.forecast.font_size_day
                                    font.weight: Style.forecast.font_weight_day
                                }
                            }

                            ColumnLayout {
                                id: test2
                                implicitWidth: parent.width
                                Layout.margins: Style.today.margin
                                spacing: 10
                                Layout.alignment: Qt.AlignRight

                                Label {
                                    text:  model.state
                                    color: Style.panel.font_color
                                    font.pixelSize: Style.forecast.font_size_day
                                    font.weight: Style.forecast.font_weight_day
                                }
                            }
                        }
                    }
                }
            }

        }
    }
}
