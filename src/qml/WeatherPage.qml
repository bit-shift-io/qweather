import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0


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
