#include "database.h"

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QXmlStreamReader>
#include <QDebug>
#include <QQmlEngine>
#include <QFile>
#include <QPointF>
#include <QLineF>

Database* Database::m_pThis = nullptr;

Database::Database(QObject *parent) : QObject(parent)
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
    icon_data = obj["icons"].toArray();

    // debug
    //QString jsonString = doc.toJson(QJsonDocument::Indented);
    //QString strFromObj = QJsonDocument(obj).toJson(QJsonDocument::Compact).toStdString().c_str();
    //qDebug() << "stations " << station_data.size();
}

Database::~Database()
{
}

QJsonArray Database::getStationByName(const QString &xName)
{
    QJsonArray result;
    foreach (const QJsonValue & value, station_data) {
        QJsonArray station = value.toArray();
        QString station_name = station[NAME].toString().toLower();
        if (station_name.contains(xName)) {
            result.append(station);
        }
    }
    return result;
}

QString Database::getObservationUrl(const QString &xWmo) {
    QJsonArray station = getStationByWmo(xWmo);
    QString state_id = station[STATE_ID].toString();
    QString url_string = QString("http://www.bom.gov.au/fwo/%2/%2.%1.json").arg(xWmo, state_id);
    return url_string;
}


float Database::getDistance(const QPointF &xLonLatA, const QPointF &xLonLatB) {
    QLineF l(xLonLatA, xLonLatB);
    return l.length();
}

QJsonArray Database::findStation(const QString &xQuery)
{
    QString query = xQuery.toLower();

    // search can be via several methods
    // name, state, lon/lat
    if (query.contains(",")) {

    }

    QJsonArray result = getStationByName(query);
    return result;
}

QJsonArray Database::getStationByWmo(const QString &xWmo) {
    foreach (const QJsonValue & value, station_data) {
        QJsonArray station = value.toArray();
        QString station_wmo = station[WMO].toString();
        if (station_wmo == xWmo) {
            return station;
        }
    }
    return QJsonArray();
}

QString Database::getAreaCode(const QString &xWmo) {
    return getStationByWmo(xWmo)[AREA_CODE].toString();
}

QString Database::getIcon(int xIconCode, const QString &xDescription)
{
    QString result = icon_data[0].toString(); // init missing.svg
    if (!xIconCode)
        return result;

    // special case for partly-cloud/mostly-sunny
    if (xIconCode == 3 && xDescription.toLower().contains("partly"))
        xIconCode = 20;

    result = icon_data[xIconCode].toString();
    return result;
}


QString Database::getForecastUrl(const QString &xWmo) {
    QJsonArray station = getStationByWmo(xWmo);
    QString forecast_url = station[FORECAST_ID].toString();
    QString url_string = QString("ftp://ftp.bom.gov.au/anon/gen/fwo/%1.xml").arg(forecast_url);
    return url_string;
}

QString Database::getDetailedUrl(const QString &xWmo)
{
    QJsonArray station = getStationByWmo(xWmo);
    QString detailed_url = station[DETAILED_FORECAST].toString();
    QString url_string = QString("http://www.bom.gov.au/places/%1/forecast/detailed/").arg(detailed_url);
    return url_string;
}

QString Database::getRadarId(const QString &xWmo)
{
    QJsonArray station = getStationByWmo(xWmo);
    return station[RADAR_ID].toString();
}

Database *Database::instance()
{
    if (m_pThis == nullptr) // avoid creation of new instances
        m_pThis = new Database;
    return m_pThis;
}

QObject *Database::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    // C++ and QML instance they are the same instance
    return Database::instance();
}
