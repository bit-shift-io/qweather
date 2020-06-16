
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDirIterator>
#include <QDebug>
#include <QQmlDebuggingEnabler>

#include "database.h"
#include "weather.h"
#include "forecastmodel.h"
#include "radarimage.h"

#ifdef Q_OS_ANDROID
#include "./3rdparty/kirigami/src/kirigamiplugin.h"
#endif

int main(int argc, char *argv[])
{
    QQmlDebuggingEnabler enabler;

    // register types
    qmlRegisterSingletonType<Database>("Database", 1, 0, "Database", &Database::qmlInstance);
    qmlRegisterType<Weather>("Weather", 1, 0, "Weather");
    qmlRegisterType<ForecastModel>("Forecast", 1, 0, "ForecastModel");
    qmlRegisterType<RadarImage>("RadarImage", 1, 0, "RadarImage");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

#ifdef Q_OS_ANDROID
    KirigamiPlugin::getInstance().registerTypes();
#endif

    // used for settings
    app.setOrganizationName("bitshift");
    app.setOrganizationDomain("bitshift");
    app.setApplicationName("Weather");

    //app.setWindowIcon(QIcon("qrc:/bitshift.alarm.png"));

    // add imports
    engine.addImportPath(".");
    engine.addImportPath("./lib");
    engine.addImportPath("./plugin");
    engine.addImportPath("./qml");
    engine.addImportPath("qrc:/qml");

    // load qml
    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // debug, print qrc resources
    if (false) {
        QDirIterator it(":/", QDirIterator::Subdirectories);
        while (it.hasNext()) {
            qDebug() << it.next();
        }
    }

    // get instance in C++
    Database *database = Database::instance();

    return app.exec();
}
