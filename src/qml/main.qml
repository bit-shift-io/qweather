import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import Weather 1.0
import Database 1.0
import 'Style'

ApplicationWindow {
    id: root
    title: Qt.application.name
    width: 350
    height: 500
    visible: true
    color: Style.app.color

    // todo: store this in settings
    property var places: []

    Component.onCompleted: {
        //console.log("app load complete");
    }


    Drawer {
        id: drawer
        width: 0.66 * window.width
        height: window.height
        background: Rectangle {
            Rectangle {
                x: parent.width - 1
                width: 1
                height: parent.height
                color: "#21be2b"
            }
        }

        Label {
            text: "Content goes here!"
            anchors.centerIn: parent
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainPage {}
    }
}
