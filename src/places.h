#ifndef Places_H
#define Places_H

#include <QObject>
#include <QNetworkReply>

class QQmlEngine;
class QJSEngine;

// using bom weather
// sample url http://www.bom.gov.au/fwo/IDQ60801/IDQ60801.94182.json
// http://www.bom.gov.au/fwo/[state_id]/[state_id].[wmo_id].json

class Places : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    static Places *instance();
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    Places(QObject *parent = 0);
    ~Places();

    // map states to id
    const QMap<QString, QString> STATE_ID {
        {"SA", "IDS60801"},
        {"ACT", "IDN60903"},
        {"NSW", "IDN60801"},
        {"NT", "IDD60801"},
        {"QLD", "IDQ60801"},
        {"TAS", "IDT60801"},
        {"VIC", "IDV60801"},
        {"WA", "IDW60801"},
    };

    // url to scrape location id's
    const QMap<QString, QString> STATE_URL {
        {"SA", "http://www.bom.gov.au/sa/observations/saall.shtml"},
        {"ACT", "http://reg.bom.gov.au/act/observations/canberra.shtml"},
        {"NSW", "http://www.bom.gov.au/nsw/observations/nswall.shtml"},
        {"NT", "http://www.bom.gov.au/nt/observations/ntall.shtml"},
        {"QLD", "http://www.bom.gov.au/qld/observations/qldall.shtml"},
        {"TAS", "http://www.bom.gov.au/tas/observations/tasall.shtml"},
        {"VIC", "http://www.bom.gov.au/vic/observations/vicall.shtml"},
        {"WA", "http://www.bom.gov.au/wa/observations/waall.shtml"},
    };

private:
    QNetworkAccessManager *mNetworkAccessManager;
    static Places* m_pThis;
    int update_count = 0;
    void updateNext();

public slots:
    void update();

signals:
    void resultFinished(const QJsonObject &xResult);

private slots:
    void replyFinished(QNetworkReply *xNetworkReply);

};

#endif // Places_H
