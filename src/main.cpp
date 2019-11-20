#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("bitshift");
    QCoreApplication::setOrganizationDomain("bitshift.io");
    QCoreApplication::setApplicationName("Weather");

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    int ret = app.exec();
    return ret;
}
