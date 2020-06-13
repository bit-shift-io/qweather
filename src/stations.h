#ifndef Stations_H
#define Stations_H

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

class Stations : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    static Stations *instance();
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    Stations(QObject *parent = 0);
    ~Stations();

    QString WmoToUrl(QString xWmo);
    QString WmoToState(QString xWmo);
    QString StationToUrl(QJsonArray xStation);
    QString WmoToForecastUrl(QString xWmo);
    QPointF WmoToLonLat(QString xWmo);
    QString LonLatToAreaCode(QPointF xLonLat);
    float getDistance(QPointF xLonLatA, QPointF xLonLatB);
    QString AreaCodeToForecastUrl(QString xWmo);

private:
    static Stations* m_pThis;
    QJsonArray station_data;
    QJsonArray area_data;
    QJsonArray forecast_data;


public slots:
    //void update();

signals:
    //void resultFinished(const QJsonObject &xResult);

private slots:
    //void replyFinished(QNetworkReply *xNetworkReply);

};

#endif // Stations_H
