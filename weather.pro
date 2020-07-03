TEMPLATE = app

QT += charts qml quick svg 

android: {
    QT += androidextras
#    include(3rdparty/kirigami/kirigami.pri)
}

CONFIG += c++11

SOURCES += \
    src/database.cpp \
    src/fileio.cpp \
    src/forecastmodel.cpp \
    src/main.cpp \
    src/qftp/qftp.cpp \
    src/qftp/qurlinfo.cpp \
    src/radarimage.cpp \
    src/settings.cpp \
    src/weather.cpp

RESOURCES += \
    src/qml.qrc \
    res/res.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/database.h \
    src/fileio.h \
    src/forecastmodel.h \
    src/qftp/qftp.h \
    src/qftp/qurlinfo.h \
    src/radarimage.h \
    src/settings.h \
    src/weather.h

SUBDIRS += \
    src/qftp/qftp.pro

DISTFILES += \
    android/AndroidManifest.xml \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

