import QtQuick 2.1
import org.kde.kirigami 2.4 as Kirigami
import QtQuick.Controls 2.0 as Controls

Kirigami.ApplicationWindow {
    id: root
    title: "Weather"

    // global navigation drawer
    globalDrawer: Kirigami.GlobalDrawer {
        actions: [
           Kirigami.Action {
               text: "View"
               iconName: "view-list-icons"
               Kirigami.Action {
                       text: "action 1"
               }
               Kirigami.Action {
                       text: "action 2"
               }
               Kirigami.Action {
                       text: "action 3"
               }
           },
           Kirigami.Action {
               text: "Sync"
               iconName: "folder-sync"
           }
        ]
    }

    // main page
    pageStack.initialPage: main_page_component
    Component {
        id: main_page_component
        MainPage {}
    }

    /*
    // main scroll window
    Kirigami.ScrollablePage {
        id: view
        supportsRefreshing: true
        onRefreshingChanged: {
            if (refreshing) {
                myModel.refresh();
            }
        }
        Rectangle {
                color: "black"
                height: 1000
                
                Controls.Label {
                    text:  qsTr("Hello weather")
                    color: "white"
                    anchors.centerIn: parent
                }
            }
        /*
        ListView {
            //NOTE: MyModel doesn't come from the components,
            //it's purely an example on how it can be used together
            //some application logic that can update the list model
            //and signals when it's done.
            model: MyModel {
                onRefreshDone: view.refreshing = false;
            }
            delegate: Kirigami.BasicListItem {}
        }
        * /
    }
    */


    /*
    Component {
        id: mainPageComponent

        Kirigami.Page {
            title: "Weather"

            Rectangle {
                color: "black"
                anchors.fill: parent
                
                Controls.Label {
                    text:  qsTr("Hello weather")
                    color: "white"
                    anchors.centerIn: parent
                }
            }
        }
    }
    */
}
