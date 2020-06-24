import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import Weather 1.0
import Database 1.0
import 'Style'

ApplicationWindow {
    id: root
    title: Qt.application.name
    width: 350
    height: 500
    visible: true
    color: Style.app.color

    property string database_file: "weather.json" // store custom settings here
    property var station_list: ["94672"] // todo: store this in settings

    Component.onCompleted: {
        //console.log("app load complete");
    }


    Drawer {
        id: drawer
        width: 0.66 * root.width
        height: root.height
        background: Rectangle {
            color: Style.drawer.color
        }

        ColumnLayout {
            Button {
                text: "Find location"
                onClicked: {
                    stack_view.push("SearchPage.qml", {}) // ("SearchPage.qml", {user_info: user_data})
                    drawer.close()
                }
            }

            Button {
                text: "Refresh"
            }
        }
    }

    StackView {
        id: stack_view
        anchors.fill: parent
        initialItem: MainPage {
            weather_station: station_list[0]
        }
    }
}
