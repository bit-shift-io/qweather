#include "stations.h"

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QXmlStreamReader>
#include <QDebug>
#include <QQmlEngine>
#include <QFile>

Stations* Stations::m_pThis = nullptr;

Stations::Stations(QObject *parent) : QObject(parent)
{
    QFile file(":/database.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray file_data = file.readAll();
    file.close();

    QJsonParseError JsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(file_data, &JsonParseError);
    if (JsonParseError.error != QJsonParseError::NoError) {
        qDebug() << JsonParseError.errorString();
    }

    QJsonObject obj = doc.object();
    station_data = obj["stations"].toArray();
    forecast_data = obj["forecast"].toObject();

    // debug
    //QString jsonString = doc.toJson(QJsonDocument::Indented);
    //QString strFromObj = QJsonDocument(obj).toJson(QJsonDocument::Compact).toStdString().c_str();
    //qDebug() << "stations " << station_data.size();
}

Stations::~Stations()
{
}

QString Stations::WmoToUrl(QString xWmo) {
    foreach (const QJsonValue & value, station_data) {
        QJsonArray station = value.toArray();
        QString station_wmo = station[0].toString();
        //qDebug() << station_wmo;
        if (station_wmo == xWmo) {
            return StationToUrl(station);
        }
    }
    return "";
}

QString Stations::WmoToState(QString xWmo) {
    foreach (const QJsonValue & value, station_data) {
        QJsonArray station = value.toArray();
        QString station_wmo = station[0].toString();
        //qDebug() << station_wmo;
        if (station_wmo == xWmo) {
            return station[2].toString();
        }
    }
    return "";
}

QString Stations::WmoToForecastUrl(QString xWmo) {
    QString state = WmoToState(xWmo);
    QString url = forecast_data[state].toString();
    return url;
}

QString Stations::StationToUrl(QJsonArray xStation) {
    QString wmo = xStation[0].toString();
    QString state_id = xStation[1].toString();
    QString url_string = QString("http://www.bom.gov.au/fwo/%2/%2.%1.json").arg(wmo, state_id);
    return url_string;
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
