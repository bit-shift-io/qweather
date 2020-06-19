#ifndef Weather_H
#define Weather_H

#include <QObject>
#include <QJsonObject>

class QNetworkReply;


class Weather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString station READ station WRITE setStation NOTIFY stationChanged)
public:
    Weather(QObject *parent = 0);
    ~Weather();

    QString station() const;
    void setStation(const QString &xStationId);

    QString getRadarId() const;

    // these a public for now as they will not be altered
    QJsonObject mForecastData;
    QJsonObject mObservationData;

public slots:
    void requestForecast();
    void requestObservation();
    void requestRadar();
    void requestDetailedForecast();

signals:
    void stationChanged();
    void resultObservationFinished(const QJsonObject &xResult);
    void resultForecastFinished(const QJsonObject &xResult);
    void resultDetailedForecastFinished(const QJsonObject &xResult);
    void updateRadar();

private slots:
    void replyForecastFinished(QNetworkReply *xNetworkReply);
    void replyObservationFinished(QNetworkReply *xNetworkReply);
    void replyDetailedForecastFinished(QNetworkReply *xNetworkReply);

private:
    QString mStationId;
    QString mObservationUrl;
    QString mAreaCode;
    QString mForecastUrl;
    QString mRadarId;
    QString mDetailedForecastUrl;
};

#endif // Weather_H
