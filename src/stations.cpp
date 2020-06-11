#include "stations.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QDebug>
#include <QQmlEngine>
#include <QFile>

Stations* Stations::m_pThis = nullptr;

Stations::Stations(QObject *parent) : QObject(parent)
{
    QFile File(":/stations.json");
    File.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonParseError JsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(File.readAll(), &JsonParseError);
    File.close();

    QJsonObject obj = doc.object();
    //QJsonArray stations = obj.begin().toArray();

    // debug
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    qDebug() << jsonString;
}

Stations::~Stations()
{
}

QJsonObject Stations::byName(QString name) {


}

QJsonObject Stations::byLatLon(QString lat, QString lon) {


}

QUrl Stations::getUrl(QJsonObject station) {


}

Stations *Stations::instance()
{
    if (m_pThis == nullptr) // avoid creation of new instances
        m_pThis = new Stations;
    return m_pThis;
}

QObject *Stations::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    // C++ and QML instance they are the same instance
    return Stations::instance();
}
