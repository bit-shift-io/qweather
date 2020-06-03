import QtQuick 2.6
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami


Rectangle {
    height: 400
    width: parent.width
    color: "#e02323"

    ListView {
        id: forecast
        anchors.fill: parent
        model: 7
        //model: ForecastModel {}
        delegate: ForecastDelegate {
           // pass the alarm model into the delegate
           // so all our util functions are in the model
           //property variant alarm_model: alarm_list_view.model
        }

    }
}

