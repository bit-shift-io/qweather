import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.12
import Qt.labs.settings 1.0
import Qt.labs.platform 1.1 // for standard paths
import FileIO 1.0
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

    property int database_version: 1
    property string database_file: (FileIO.getAppConfigLocation() + 'database.json')
    property var station_list: ["94672"]

    Settings {
        category: "window"
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }

    Component.onCompleted: {
        load();
    }

    function load() {
        if (!FileIO.fileExists(database_file))
            return;

        var json = JSON.parse(FileIO.readText(database_file));

        if (json.version !== database_version) {
            console.log('database version mismatch');
            FileIO.removeFile(database_file);
        }
        station_list = json.station_list;
    }

    function save() {
        var data = {};
        data['version'] = database_version;
        data['station_list'] = station_list;
        FileIO.writeText(database_file, JSON.stringify(data))
    }

    function add_station(x_wmo) {
        // for now we store a single item
        station_list[0] = x_wmo;

        /*
        // check if exists
        var is_new = true;
        for (let i in station_list) {
            if (x_wmo === station_list[i])
                is_new = false;
        }


        station_list.unshift(x_wmo);
        */

        stack_view.replace("MainPage.qml", {weather_station: station_list[0]})
        save();
    }


    Drawer {
        id: drawer
        width: 0.66 * root.width
        height: root.height
        background: Rectangle {
            color: Style.drawer.color
        }

        ColumnLayout {
            id: column_layout
            width: parent.width

            Button {
                text: "Search"
                implicitWidth: parent.width
                onClicked: {
                    stack_view.push("SearchPage.qml")
                    drawer.close()
                }
            }

            Button {
                text: "Pop"
                implicitWidth: parent.width
                onClicked: {
                    stack_view.pop()
                    drawer.close()
                }
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
