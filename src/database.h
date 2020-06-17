#ifndef Database_H
#define Database_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

class QQmlEngine;
class QJSEngine;

// using bom weather
// sample url http://www.bom.gov.au/fwo/IDQ60801/IDQ60801.94182.json
// http://www.bom.gov.au/fwo/[state_id]/[state_id].[wmo_id].json
// data can also be found here:
// ftp://ftp.bom.gov.au/anon2/home/ncc/metadata/sitelists/stations.zip
// xml can be found here:
// ftp://ftp.bom.gov.au/anon/gen/fwo/IDS60920.xml

class Database : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    // used to access station data array
    enum station {
        WMO = 0,
        STATE_ID,
        FORECAST_ID,
        AREA_CODE,
        RADAR_ID,
        LATITUDE,
        LONGDITUDE,
        STATE,
        NAME,
        STATE_SIZE,
    };

    static Database *instance();
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    Database(QObject *parent = 0);
    ~Database();

    QJsonArray getStationByWmo(const QString &xWmo);
    QString getObservationUrl(const QString &xWmo);
    QString getForecastUrl(const QString &xWmo);
    QString getRadarId(const QString &xWmo);
    QString getAreaCode(const QString &xWmo);
    QString getIcon(int xIconCode, const QString &xDescription);
    float getDistance(const QPointF &xLonLatA, const QPointF &xLonLatB);

private:
    static Database* m_pThis;
    QJsonArray station_data;
    QJsonArray icon_data;

public slots:
    //void update();

signals:
    //void resultFinished(const QJsonObject &xResult);

private slots:
    //void replyFinished(QNetworkReply *xNetworkReply);

};

#endif // Database_H
