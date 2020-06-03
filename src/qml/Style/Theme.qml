import QtQuick 2.0
import QtQuick.Controls.Material 2.4

QtObject {
    // colors
    property color color_day
    property color color_night
    property color color_panel

    // main body text
    property color font_default_color
    property string font_default_family
    property int font_default_size
    property int font_default_capitalization

    // material theme
    // https://doc.qt.io/qt-5/qtquickcontrols2-material.html# for more
    property int material_theme
    property color material_accent
}
