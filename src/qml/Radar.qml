import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import 'Style'

Item {
    id: panel
    Layout.fillWidth: true
    width: parent.width
    height: 200

    onWidthChanged: {
        // keep square
        panel.height = panel.height
    }

    Rectangle {
        id: background
        color: Style.panel.color
        radius: Style.panel.radius
        anchors.fill: parent
    }

    DropShadow {
        anchors.fill: background
        horizontalOffset: 0
        verticalOffset: 0
        radius: Style.shadow.radius
        samples: Style.shadow.samples
        color: Style.shadow.color
        source: background
    }

    Image {
        id: image
        source: "qrc:/heavy-showers.svg"
        //sourceSize.width: Style.today.image_size
        //sourceSize.height: Style.today.image_size
        anchors.fill: parent
    }

}
