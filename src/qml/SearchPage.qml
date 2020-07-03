import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import Qt.labs.settings 1.0
import Database 1.0
import 'Style'

Page {
    id: root_search
    title: qsTr("Find Location")

    Rectangle {
        id: background
        color: Style.app.color
        anchors.fill: parent

        ColumnLayout {
            id: column_layout
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
                        onClicked: {
                            stack_view.pop()
                            var index = search_result.currentIndex;
                            if (index >= 0) {
                                // set selected as active
                                var wmo = search_model.get(index).wmo
                                root.add_station(wmo)
                            }
                        }
                    }
                    TextField {
                        id: search_input
                        placeholderText: qsTr("Location, Sate or Lon Lat")
                        cursorVisible: true
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        focus: true

                        //background: {}
                        onTextChanged: {
                            if (search_input.text.length < 2)
                                return;

                            search_result.currentIndex = -1;

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
                //snapMode: ListView.SnapToItem
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                spacing: Style.search.list_spacing
                currentIndex: -1 // no selected default

                model:  ListModel {
                    id: search_model
                }

                delegate: Component {
                    id: search_delegate

                    Rectangle {
                        id: background
                        color: ListView.isCurrentItem? Style.search.color_highlight : Style.search.color_background
                        radius: Style.search.radius_background
                        implicitHeight: row_layout.height
                        implicitWidth: search_result.width

                        RowLayout {
                            id: row_layout
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
                                }
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                search_result.currentIndex = index;
                            }
                        }
                    }
                }
            }
        }
    }
}
