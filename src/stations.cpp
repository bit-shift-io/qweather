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
    QFile file(":/stations.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray file_data = file.readAll();
    file.close();

    QJsonParseError JsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(file_data, &JsonParseError);
    QJsonObject obj = doc.object();
    QJsonArray stations = obj["stations"].toArray();

    // debug
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    QString strFromObj = QJsonDocument(obj).toJson(QJsonDocument::Compact).toStdString().c_str();
    qDebug() << strFromObj;
}

Stations::~Stations()
{
}

QJsonObject Stations::byWMO(QString wmo) {


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
