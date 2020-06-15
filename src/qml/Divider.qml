import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import 'Style'

Item {
    Layout.preferredHeight: 10
    Layout.preferredWidth: parent.width
    Layout.fillWidth: true
    Layout.alignment: Qt.AlignCenter


    Rectangle {
        id: background
        color: Style.divider.color
        radius: Style.divider.radius
        anchors.margins: Style.divider.margin
        anchors.fill: parent
    }

}


