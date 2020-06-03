pragma Singleton
import QtQuick 2.0
import QtQuick.Controls.Material 2.4

/*
Info on theme from here:
https://stackoverflow.com/questions/44389883/dynamically-change-qml-theme-at-runtime-on-mouseclick

more info here too:
http://imaginativethinking.ca/make-qml-component-singleton/
*/

Theme {
    // themes here
    property Theme active: theme_dark
    property Theme theme_dark: ThemeDark { }
    property Theme theme_light: ThemeLight { }

    // bind theme properties

    // colors
    color_day: (active && active.color_day ? active.color_day : 'color_day')
    color_night: (active && active.color_night ? active.color_night : 'color_night')
    color_panel: (active && active.color_panel ? active.color_panel : 'color_panel')

    // main body text
    font_default_color: (active && active.font_default_color ? active.font_default_color : 'font_default_color')
    font_default_family: (active && active.font_default_family ? active.font_default_family : 'font_default_family')
    font_default_size: (active && active.font_default_size ? active.font_default_size : 'font_default_size')
    font_default_capitalization: (active && active.font_default_capitalization ? active.font_default_capitalization : 'font_default_capitalization')

    // material theme
    // https://doc.qt.io/qt-5/qtquickcontrols2-material.html# for more
    material_theme: (active && active.material_theme ? active.material_theme : 'material_theme')
    material_accent: (active && active.material_accent ? active.material_accent : 'material_accent')


}
