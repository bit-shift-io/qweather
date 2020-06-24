import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import Weather 1.0
import Database 1.0
import 'Style'

Page {
    id: root
    title: qsTr("Find Location")

    Rectangle {
        id: background
        color: Style.app.color
        anchors.fill: parent

        ToolBar {
            // header
            Button {
                text: "Back"
                onClicked: {
                    stack_view.pop()
                }
            }
            TextInput {
                text: "Enter location, state, or lon,lat"
            }
        }

        ListView {
            id: search_result
        }
    }

}
