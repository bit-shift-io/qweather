#ifndef Stations_H
#define Stations_H

#include <QObject>
#include <QNetworkReply>
#include <QMultiHash>

class QQmlEngine;
class QJSEngine;

// using bom weather
// sample url http://www.bom.gov.au/fwo/IDQ60801/IDQ60801.94182.json
// http://www.bom.gov.au/fwo/[state_id]/[state_id].[wmo_id].json

class Stations : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    static Stations *instance();
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    Stations(QObject *parent = 0);
    ~Stations();

    QJsonObject byName(QString name);
    QJsonObject byLatLon(QString lat, QString lon);
    QUrl getUrl(QJsonObject station);

private:
    static Stations* m_pThis;

public slots:
    //void update();

signals:
    //void resultFinished(const QJsonObject &xResult);

private slots:
    //void replyFinished(QNetworkReply *xNetworkReply);

};

#endif // Stations_H
