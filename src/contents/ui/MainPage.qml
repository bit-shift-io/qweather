
import QtQuick 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami
import QtQuick.Controls 2.0 as Controls


Kirigami.ScrollablePage {
    id: pageRoot

    
    supportsRefreshing: true
    onRefreshingChanged: {
        if (refreshing) {
            refreshRequestTimer.running = true;
        } else {
            showPassiveNotification("Example refreshing completed")
        }
    }

    implicitWidth: Kirigami.Units.gridUnit * 20
    background: Rectangle {
        color: Kirigami.Theme.backgroundColor
    }

    title: qsTr("Weather")

    // pullback header

    ListView {
        id: mainList

        //headerPositioning: ListView.OverlayHeader
        headerPositioning: ListView.PullBackHeader
        header: Kirigami.ItemViewHeader {
            //backgroundImage.source: "../banner.jpg"
            title: page.title
        }


        // list items of page
        model: 5
        /*
        delegate: Kirigami.BasicListItem {
            id: listItem
            label: "Item " + modelData
        }*/

        delegate: Kirigami.AbstractCard {
            //NOTE: never put a Layout as contentItem as it will cause binding loops
            //SEE: https://bugreports.qt.io/browse/QTBUG-66826
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                        //IMPORTANT: never put the bottom margin
                    }
                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: width > Kirigami.Units.gridUnit * 20 ? 4 : 2
                    Kirigami.Icon {
                        source: "applications-graphics"
                        Layout.fillHeight: true
                        Layout.maximumHeight: Kirigami.Units.iconSizes.huge
                        Layout.preferredWidth: height
                    }
                    ColumnLayout {
                        Kirigami.Heading {
                            level: 2
                            text: qsTr("Product ")+ modelData
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }
                        Controls.Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: qsTr("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam id risus id augue euismod accumsan.")
                        }
                    }
                    Controls.Button {
                        Layout.alignment: Qt.AlignRight|Qt.AlignVCenter
                        Layout.columnSpan: 2 
                        text: qsTr("Install")
                        onClicked: showPassiveNotification("Install for Product " + modelData + " clicked");
                    }
                }
            }
        }
    }

    /*
    // card list view
    Kirigami.CardsListView {
        id: view

        headerPositioning: ListView.PullBackHeader
        header: Kirigami.ItemViewHeader {
            //backgroundImage.source: "../banner.jpg"
            title: page.title
        }

        model: 100

        delegate: Kirigami.AbstractCard {
            //NOTE: never put a Layout as contentItem as it will cause binding loops
            //SEE: https://bugreports.qt.io/browse/QTBUG-66826
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                        //IMPORTANT: never put the bottom margin
                    }
                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: width > Kirigami.Units.gridUnit * 20 ? 4 : 2
                    Kirigami.Icon {
                        source: "applications-graphics"
                        Layout.fillHeight: true
                        Layout.maximumHeight: Kirigami.Units.iconSizes.huge
                        Layout.preferredWidth: height
                    }
                    ColumnLayout {
                        Kirigami.Heading {
                            level: 2
                            text: qsTr("Product ")+ modelData
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }
                        Controls.Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: qsTr("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam id risus id augue euismod accumsan.")
                        }
                    }
                    Controls.Button {
                        Layout.alignment: Qt.AlignRight|Qt.AlignVCenter
                        Layout.columnSpan: 2 
                        text: qsTr("Install")
                        onClicked: showPassiveNotification("Install for Product " + modelData + " clicked");
                    }
                }
            }
        }
    }*/

}