import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0

Item {
    height: 200
    width: parent.width
    Layout.fillWidth: true

    Rectangle {
        id: background
        color: "gray"
        radius: 10
        anchors.fill: parent
    }

    DropShadow {
        anchors.fill: background
        horizontalOffset: 0
        verticalOffset: 0
        radius: 10.0
        samples: 20
        color: "#80000000"
        source: background
    }


}
