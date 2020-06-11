import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
//import org.kde.kirigami 2.4 as Kirigami
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.4
import "util.js" as Util
import Weather 1.0
import Stations 1.0
import 'Style'

ApplicationWindow {
    id: window
    title: Qt.application.name
    width: 350
    height: 500
    visible: true
    color: Style.app.color

    // todo: store this in settings
    property var places: []

    Component.onCompleted: {
        console.log("app load complete");
    }


    Weather {
        // weather object
        id: weather
        url: 'ftp://ftp.bom.gov.au/anon/gen/fwo/IDS60920.xml'
        onResultFinished: {
            console.log(xResult);
        }
        Component.onCompleted: {
            console.log("weather complete");
            weather.requestWeather("Koblenz,de");
        }
    }

    /*
    Timer {
        // Refresh the forecast every 5 minutes
        interval: 300000
        repeat: true
        triggeredOnStart: true
        running: true
        onTriggered: {
            var url = 'http://www.bom.gov.au';
            var test_url = 'ftp://ftp.bom.gov.au/anon/gen/fwo/IDS60920.xml';
            var station = '94648';
            var xhr = new XMLHttpRequest;
            xhr.open("GET", test_url);
            xhr.onreadystatechange = function() {
                if (xhr.readyState == XMLHttpRequest.DONE) {
                    var doc = xhr.responseXML.documentElement;
                    //var result = Util.parse(xhr.responseText);
                    var o = getElementsByNodeName(doc, 'observations');
                    console.log(o);

                    /*
                    //showRequestInfo("xhr length: " + doc.childNodes.length );

                    for (var i = 0; i < doc.childNodes.length; ++i) {
                        var child = doc.childNodes[i];

                        // get observations
                        for (var j = 0; j < child.childNodes.length; ++j) {
                            if ( child.nodeName ===  "observations") {

                                var kid = child.childNodes[j];
                                var length = kid.childNodes.length;

                                for ( var k = 0; k < length; k ++) {
                                    var cube = kid.childNodes[k];
                                    console.log(cube.nodeName);
                                    if ( cube.nodeName === "station") {
                                        var len = cube.attributes.length;
                                        var currency = cube.attributes[0].nodeValue;
                                        var rate = cube.attributes[1].nodeValue;
                                        currencies.append({"currency": currency, "rate": parseFloat(rate)})
                                    }
                                }
                            }

                        }
                    }
                    * /
                }
            }
            xhr.send();
        }
    }
    */

    function getElementsByNodeName(rootElement, nodeName) {
        var childNodes = rootElement.childNodes;
        var elements = [];
        for(var i = 0; i < childNodes.length; i++) {
            console.log(childNodes[i].nodeName);
            if(childNodes[i].nodeName === nodeName) {
                elements.push(childNodes[i]);
            }
        }
        return elements;
    }

    function getElementsByTagName(rootElement, tagName) {
        var childNodes = rootElement.childNodes;
        var elements = [];
        for(var i = 0; i < childNodes.length; i++) {
            if(childNodes[i].tagName === tagName) {
                elements.push(childNodes[i]);
            }
        }
        return elements;
    }


    ScrollView {
        // main page layout
        id: scroll_view
        anchors.fill: parent

        ColumnLayout {
            spacing: 10
            anchors.fill: parent

               /*
            Button {
                text: "refresh"
                onClicked: {
                    weather.requestWeather("Koblenz,de");
                }

            }*/

            Today {}

            Forecast {}

            GraphicForecast {}

            Radar {}
        }
    }

}

/*
Kirigami.ApplicationWindow {
    id: window
    visible: true
    title: application.name
    width: 350
    height: 500

    pageStack.initialPage: Qt.resolvedUrl("WeatherPage.qml")

    globalDrawer: Kirigami.GlobalDrawer {
        title: "Hello App"
        titleIcon: "applications-graphics"
        actions: [
            Kirigami.Action {
                text: "Add location"
            },
            Kirigami.Action {
                text: "Remove location"
            }
            ]
    }

}
*/
