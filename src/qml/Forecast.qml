import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0

Item {
    height: 400
    width: parent.width
    Layout.fillWidth: true

    Rectangle {
        id: background
        color: "gray"
        radius: 10
        anchors.fill: parent
        //anchors.margins: 10
    }

    DropShadow {
        anchors.fill: background
        horizontalOffset: 0
        verticalOffset: 2
        radius: 10.0
        samples: 20
        color: "#80000000"
        source: background
    }

    ListView {
        id: forecast
        anchors.fill: parent
        //anchors.margins: 10 * 2
        model: 7
        //model: ForecastModel {}
        delegate: ForecastDelegate {
           // pass the alarm model into the delegate
           // so all our util functions are in the model
           //property variant alarm_model: alarm_list_view.model
        }
    }

}

