#include "stations.h"

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QXmlStreamReader>
#include <QDebug>
#include <QQmlEngine>
#include <QFile>
#include <QPointF>
#include <QLineF>

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

    // debug
    //QString jsonString = doc.toJson(QJsonDocument::Indented);
    //QString strFromObj = QJsonDocument(obj).toJson(QJsonDocument::Compact).toStdString().c_str();
    //qDebug() << "stations " << station_data.size();
}

Stations::~Stations()
{
}

QString Stations::getObservationUrl(QString xWmo) {
    QJsonArray station = getStationByWmo(xWmo);
    QString state_id = station[1].toString();
    QString url_string = QString("http://www.bom.gov.au/fwo/%2/%2.%1.json").arg(xWmo, state_id);
    return url_string;
}


float Stations::getDistance(QPointF xLonLatA, QPointF xLonLatB) {
    QLineF l(xLonLatA, xLonLatB);
    return l.length();
}

QJsonArray Stations::getStationByWmo(QString xWmo) {
    foreach (const QJsonValue & value, station_data) {
        QJsonArray station = value.toArray();
        QString station_wmo = station[0].toString();
        if (station_wmo == xWmo) {
            return station;
        }
    }
    return QJsonArray();
}

QString Stations::getAreaCode(QString xWmo) {
    return getStationByWmo(xWmo)[3].toString();
}


QString Stations::getForecastUrl(QString xWmo) {
    QJsonArray station = getStationByWmo(xWmo);
    QString forecast_url = station[2].toString();
    QString url_string = QString("ftp://ftp.bom.gov.au/anon/gen/fwo/%1.xml").arg(forecast_url);
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
