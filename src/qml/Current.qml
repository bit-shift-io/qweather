import QtQuick 2.6
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami


Kirigami.AbstractCard {
    Layout.fillHeight: true
    header: Kirigami.Heading {
        text: qsTr("AbstractCard")
        level: 2
    }
    contentItem: Controls.Label {
        wrapMode: Text.WordWrap
        text: qsTr("An AbstractCard is the simplest form of card. It's just a rectangle with a shadow, which can contain any Item in it. It can also have items assigned to the Header or Footer properties. In this case a Kirigami.Heading is its header and a Label with WordWrap is the contentItem.")
    }
}

