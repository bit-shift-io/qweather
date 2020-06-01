import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.4 as Kirigami

Kirigami.ApplicationWindow {
    visible: true
    title: qsTr("Weather")

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
