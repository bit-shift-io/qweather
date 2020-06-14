#ifndef Weather_H
#define Weather_H

#include <QObject>
#include <QNetworkReply>

class Weather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString station READ station WRITE setStation NOTIFY stationChanged)
public:
    Weather(QObject *parent = 0);
    ~Weather();

    QString station() const;
    void setStation(const QString &xUrl);

public slots:
    void update();


signals:
    void stationChanged();
    void resultObservationFinished(const QJsonObject &xResult);
    void resultForecastFinished(const QJsonObject &xResult);

private slots:
    void replyForecastFinished(QNetworkReply *xNetworkReply);
    void replyObservationFinished(QNetworkReply *xNetworkReply);

private:
    void requestForecast();
    void requestObservation();
    QString mWMO;
    QString mObservationUrl;
    QString mAreaCode;
    QString mForecastUrl;
    //QNetworkAccessManager *mNetworkAccessManager;
};

#endif // Weather_H
