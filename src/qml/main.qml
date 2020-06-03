import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.4 as Kirigami

ApplicationWindow {
    id: window
    title: application.name
    width: 350
    height: 500
    visible: true


    ScrollView {
        id: scroll_view
        anchors.fill: parent

        ColumnLayout {
            spacing: 10
            anchors.fill: parent

            Current {}

            Forecast {}

            //GraphicForecast {}

            Radar {}

        }
    }
}

/*
Kirigami.ApplicationWindow {
    id: window
    visible: true
    title: application.name
    width: 350
    height: 500

    pageStack.initialPage: Qt.resolvedUrl("WeatherPage.qml")

    globalDrawer: Kirigami.GlobalDrawer {
        title: "Hello App"
        titleIcon: "applications-graphics"
        actions: [
            Kirigami.Action {
                text: "Add location"
            },
            Kirigami.Action {
                text: "Remove location"
            }
            ]
    }

}
*/
