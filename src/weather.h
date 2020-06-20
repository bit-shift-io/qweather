#ifndef Weather_H
#define Weather_H

#include <QObject>
#include <QJsonObject>

class QNetworkReply;


class Weather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString station READ station WRITE setStation NOTIFY stationChanged)

    // 24hr time for detailed outlook/forecast
    const QVector<QString> detailed_column_times = { "0030", "0330", "0630", "0930", "1230", "1530", "1830", "2130" };
    const QVector<QString> detailed_row_labels = { "rain_50", "rain_25", "rain_10", "rain_chance", "air_temp", "feel_temp", "dew_point", "uv_index", "thunderstorms", "snow", "rain", "fog", "frost", "wind_speed", "wind_dir", "humidity", "forest_dry", "mix_height" };

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
