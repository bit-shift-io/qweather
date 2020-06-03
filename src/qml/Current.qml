import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.4 as Kirigami


Rectangle {
    height: 200
    color: "lightblue"
    width: parent.width
    Layout.fillWidth: true

    RowLayout {
        anchors.fill: parent

        ColumnLayout {

            Label {
                id: temp
                font.pixelSize: Qt.application.font.pixelSize * 2
                text: "12C"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {
                id: feel
                font.pixelSize: Qt.application.font.pixelSize
                text: "Feels like 23C"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {
                id: description
                font.pixelSize: Qt.application.font.pixelSize
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
                font.pixelSize: Qt.application.font.pixelSize
                text: "Adelaide"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            RowLayout {

                Label {
                    id: low
                    font.pixelSize: Qt.application.font.pixelSize
                    text: "12C"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }

                Label {
                    id: high
                    font.pixelSize: Qt.application.font.pixelSize
                    text: "24C"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }
            }

            Label {
                id: rain
                font.pixelSize: Qt.application.font.pixelSize
                text: "1mm"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {
                id: wind
                font.pixelSize: Qt.application.font.pixelSize
                text: "12km/s ssw"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {
                id: humid
                font.pixelSize: Qt.application.font.pixelSize
                text: "18%"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

        }

    }
    

}

