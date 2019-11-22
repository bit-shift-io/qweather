#!/bin/bash
# https://doc.qt.io/qt-5/qtquick-debugging.html#qml-debugging-infrastructure
cd build/src
QT_QUICK_CONTROLS_MOBILE=true QT_QUICK_CONTROLS_STYLE=Plasma ./qweather
#./qalarm -qmljsdebugger=port:29777,block # not available on linux?
$SHELL