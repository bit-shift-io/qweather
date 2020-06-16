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
    void setStation(const QString &xUrl);

    QJsonObject mForecastData;
    QJsonObject mObservationData;

public slots:
    void requestForecast();
    void requestObservation();
    void requestRadar();

signals:
    void stationChanged();
    void resultObservationFinished(const QJsonObject &xResult);
    void resultForecastFinished(const QJsonObject &xResult);
    void resultRadarFinished(const QImage &xResult);

private slots:
    void replyForecastFinished(QNetworkReply *xNetworkReply);
    void replyObservationFinished(QNetworkReply *xNetworkReply);
    void replyRadarFinished(QNetworkReply *xNetworkReply);

private:
    QString mWMO;
    QString mObservationUrl;
    QString mAreaCode;
    QString mForecastUrl;
    QString mRadarUrl;
};

#endif // Weather_H
