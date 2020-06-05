import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import QtCharts 2.0


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

    /*
    //![1]
    ChartView {
        id: chartView
        title: "Weather forecast"
    //![1]
        height: parent.height / 4 * 3
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        legend.alignment: Qt.AlignTop
        antialiasing: true

    //![2]
        BarCategoryAxis {
            id: barCategoriesAxis
            titleText: "Date"
        }

        ValueAxis{
            id: valueAxisY2
            min: 0
            max: 10
            titleText: "Rainfall [mm]"
        }

        ValueAxis {
            id: valueAxisX
            // Hide the value axis; it is only used to map the line series to bar categories axis
            visible: false
            min: 0
            max: 5
        }

        ValueAxis{
            id: valueAxisY
            min: 0
            max: 15
            titleText: "Temperature [&deg;C]"
        }

        LineSeries {
            id: maxTempSeries
            axisX: valueAxisX
            axisY: valueAxisY
            name: "Max. temperature"
        }

        LineSeries {
            id: minTempSeries
            axisX: valueAxisX
            axisY: valueAxisY
            name: "Min. temperature"
        }

        BarSeries {
            id: myBarSeries
            axisX: barCategoriesAxis
            axisYRight: valueAxisY2
            BarSet {
                id: rainfallSet
                label: "Rainfall"
            }
        }
    //![2]
    }
    */


    Row {
        id: weatherImageRow
        //anchors.top: chartView.bottom
        anchors.topMargin: 5
        anchors.bottom: poweredByText.top
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        //height: parent.height - chartView.height - anchors.topMargin

        ListModel {
            id: weatherImageModel
        }

        Repeater {
            id: repeater
            model: weatherImageModel
            delegate: Image {
                source: imageSource
                width: weatherImageRow.height
                height: width
                fillMode: Image.PreserveAspectCrop
            }
        }
    }

    Text {
        id: poweredByText
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 25
        height: parent.height / 25
        text: "Powered by World Weather Online"
    }

}
