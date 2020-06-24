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

        ColumnLayout{
            id: root_layout
            anchors.fill: parent


            ToolBar {
                id: tool_bar
                Layout.fillWidth: true
                RowLayout {
                    anchors.fill: parent

                    Button {
                        text: "Back"
                        onClicked: {
                            stack_view.pop()
                        }
                    }
                    TextInput {
                        id: search_input
                        text: "adelaide"
                        cursorVisible: true
                        Layout.fillWidth: true
                        onTextChanged: {
                            var result = Database.findStation(search_input.text);
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
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                model:  ListModel {
                    id: search_model
                }
                delegate: Component {
                    id: search_delegate
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        Text {
                            text: model.name
                        }
                        Text {
                            text: model.state
                        }
                        Text {
                            text: model.wmo
                        }
                    }
                }
            }
        }

    }

}
