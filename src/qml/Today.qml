import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import Style 1.0

Item {
    height: 200
    width: parent.width
    Layout.fillWidth: true


    Rectangle {
        id: background
        color: Style.color_day
        anchors.fill: parent
    }

    DropShadow {
        anchors.fill: background
        horizontalOffset: 0
        verticalOffset: 0
        radius: 10.0
        samples: 20
        color: Theme.colors.drop_shadow
        source: background
    }

    RowLayout {
        anchors.fill: parent

        ColumnLayout {

            Label {
                id: temp
                //font.pixelSize: Qt.application.font.pixelSize * 2
                text: "12C"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {
                id: feel
                //font.pixelSize: Qt.application.font.pixelSize
                text: "Feels like 23C"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {
                id: description
                //font.pixelSize: Qt.application.font.pixelSize
                text: "Sunny"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Image {
                id: image
                source: "qrc:/heavy-showers.svg"

            }

        }

        ColumnLayout {

            Label {
                id: locale
                //font.pixelSize: Qt.application.font.pixelSize
                text: "Adelaide"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            RowLayout {

                Label {
                    id: low
                    //font.pixelSize: Qt.application.font.pixelSize
                    text: "12C"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }

                Label {
                    id: high
                    //font.pixelSize: Qt.application.font.pixelSize
                    text: "24C"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }
            }

            Label {
                id: rain
                //font.pixelSize: Qt.application.font.pixelSize
                text: "1mm"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {
                id: wind
                //font.pixelSize: Qt.application.font.pixelSize
                text: "12km/s ssw"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {

                id: humid
                //font.pixelSize: Qt.application.font.pixelSize
                text: "18%"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

        }

    }
    

}

