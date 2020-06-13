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
    area_data = obj["areas"].toArray();
    forecast_data = obj["forecasts"].toArray();

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

QPointF Stations::WmoToLonLat(QString xWmo) {
    QPoint result;
    foreach (const QJsonValue & value, station_data) {
        QJsonArray station = value.toArray();
        QString station_wmo = station[0].toString();
        //qDebug() << station_wmo;
        if (station_wmo == xWmo) {
            return QPointF(station[4].toDouble(),station[5].toDouble());
        }
    }
    return result;
}

float Stations::getDistance(QPointF xLonLatA, QPointF xLonLatB) {
    QLineF l(xLonLatA, xLonLatB);
    return l.length();
}

QString Stations::LonLatToAreaCode(QPointF xLonLat) {
    int closest_distance = 9999999; // TODO: int.max
    QString closest_area_code;

    foreach (const QJsonValue & value, area_data) {
        QJsonArray item = value.toArray();
        QString station_wmo = item[0].toString();
        //qDebug() << station_wmo;
        QPointF area_lon_lat = QPointF(item[3].toDouble(), item[4].toDouble());
        float d = getDistance(area_lon_lat, xLonLat);
        if (d < closest_distance) {
            closest_distance = d;
            closest_area_code = item[0].toString();
        }

    }
    return closest_area_code;
}

QString Stations::AreaCodeToForecastUrl(QString xAreaCode) {
    foreach (const QJsonValue & value, forecast_data) {
        QJsonArray item = value.toArray();
        QString forecast = item[0].toString();
        //qDebug() << station_wmo;
        if (forecast == xAreaCode) {
            return item[2].toString();
        }
    }
    return "";
}

QString Stations::WmoToForecastUrl(QString xWmo) {
    // find the wmo station -> get lon/lat -> seatch areas by lon/lat -> get aac are code -> find forecast by aac
    QPointF lon_lat = WmoToLonLat(xWmo);
    QString area_code = LonLatToAreaCode(lon_lat);
    QString url = AreaCodeToForecastUrl(area_code);
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
