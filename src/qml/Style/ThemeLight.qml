import QtQuick 2.0
import QtQuick.Controls.Material 2.4

Theme {
    // colors
    property color color_day: '#1ab2ff'
    property color color_night: 'purple'
    property color color_panel: 'gray'

    // main body text
    property color font_default_color: 'blue'
    property string font_default_family: 'sans'
    property int font_default_size: 9

    // material theme
    // https://doc.qt.io/qt-5/qtquickcontrols2-material.html# for more
    property int material_theme: Material.Light
    property color material_accent: "#F44336"
}
