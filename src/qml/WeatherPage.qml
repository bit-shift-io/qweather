import QtQuick 2.6
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami


Kirigami.ScrollablePage {
    id: page
    title: qsTr("Adelaide, Australia")
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

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
