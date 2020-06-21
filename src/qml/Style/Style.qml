pragma Singleton
import QtQuick 2.0
import QtQuick.Controls.Material 2.4

QtObject {

    property QtObject app: QtObject {
        property color color: '#4d4d4d'
        property color font_color: '#ffffff'
        property int margin: 10

        property string font_family: Qt.application.font.family
        property int font_size: Qt.application.font.pixelSize
        property int font_capitalization: Font.MixedCase

        // material theme
        // https://doc.qt.io/qt-5/qtquickcontrols2-material.html# for more
        property int material_theme: Material.Dark
        property color material_accent: "#F44336"
    }

    property QtObject shadow: QtObject {
        property color color: '#333333'
        property int radius: 4
        property int samples: 10
    }

    property QtObject panel: QtObject {
        property color color: '#464646'
        property color font_color: app.font_color
        property int radius: 0
        property int margin: 4
        property int margin_internal: 4
    }

    property QtObject divider: QtObject {
        property color color: '#828282'
        property int radius: 2
        property int margin: 4
    }

    property QtObject today: QtObject {
        property color color_day: '#1784d2' //'#1ab2ff'
        property color color_night: 'purple'
        property color color_shadow: '#436273'
        property color font_color: app.font_color
        property int font_size: app.font_size * 1.2
        property int font_size_place: app.font_size * 1.5
        property int font_size_temp: app.font_size * 2.8
        property int font_weight_temp: Font.DemiBold
        property int font_weight_place: Font.DemiBold
        property int image_size: 100
        property int margin: app.margin
    }

    property QtObject forecast: QtObject {
        property color color_temp_low: '#4debff'
        property color color_temp_high: '#ffe34d'
        property color color_rain: app.font_color
        property color color_description: app.font_color
        property int font_size_description: app.font_size
        property int font_size_temp: app.font_size * 1.2
        property int font_size_day: app.font_size * 1.2
        property int font_weight_temp: Font.DemiBold
        property int font_weight_day: Font.Medium
        property color color_background: '#464646'
        property int radius_background: 0
        property int list_spacing: 4
        property int internal_margin: 4
    }


}
