#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QJsonObject>
#include <QString>
#include <QVector>
#include <QImage>
#include "src/qftp/qftp.h"

class QNetworkReply;
class QNetworkAccessManager;


class Weather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString station READ station WRITE setStation NOTIFY stationChanged)
    Q_PROPERTY(bool radarEnabled READ radarEnabled WRITE setRadarEnabled NOTIFY radarEnabledChanged)

    // 24hr time for detailed outlook/forecast
    const QVector<QString> detailed_column_times = { "0030", "0330", "0630", "0930", "1230", "1530", "1830", "2130" };
    const QVector<QString> detailed_row_labels = { "rain_50", "rain_25", "rain_10", "rain_chance", "air_temp", "feel_temp", "dew_point", "uv_index", "thunderstorms", "snow", "rain", "fog", "frost", "wind_speed", "wind_dir", "humidity", "forest_dry", "mix_height" };

public:
    Weather(QObject *parent = 0);
    ~Weather();

    QString station() const;
    void setStation(const QString &xStationId);

    bool radarEnabled() const;
    void setRadarEnabled(const bool &xRadar);
    QString getRadarId() const;
    QVector<QImage> copyRadarImages();

    //QJsonObject getForecast() const { return mForecastData; };
    //QJsonObject getObservations() const { return mObservationData; };
    QJsonObject mForecastData;
    QJsonObject mObservationData;


public slots:
    void requestForecast();
    void requestObservation();
    void requestRadar();
    void requestDetailedForecast();

signals:
    void stationChanged();
    void radarEnabledChanged();
    void resultObservationFinished(const QJsonObject &xResult);
    void resultForecastFinished(const QJsonObject &xResult);
    void resultDetailedForecastFinished(const QJsonObject &xResult);
    void resultRadarFinished();

private slots:
    void replyForecastFinished(QNetworkReply *xNetworkReply);
    void replyObservationFinished(QNetworkReply *xNetworkReply);
    void replyDetailedForecastFinished(QNetworkReply *xNetworkReply);
    void replyRadarImageFinished(QNetworkReply *xNetworkReply);
    void requestRadarImages();

    // ftp commands for radar
    void ftpAddToList(const QUrlInfo &xUrlInfo);
    void ftpCommandFinished(int xCommandId, bool xError);

private:
    void read();
    void write();

    QString mStationId;
    QString mObservationUrl;
    QString mAreaCode;
    QString mForecastUrl;
    QString mDetailedForecastUrl;

    // radar

    QImage read(const QString xFileName);
    void write(const QString xFileName, QImage &xImage);
    void renderRadarImages();

    QString mRadarId;
    bool mRadarEnabled = true;
    int mRadarImageRequests;
    QVector<QImage> mRadarFrames;
    QVector<QImage> mRadarImages;
    QVector<QString> mRadarFileList;
    QImage mRadarBackground;
    QImage mRadarTopography;
    QImage mRadarLocation;
    QFtp *mFtp;
    void destroyRadarImages();
    void destroyRadarFrames();

    // network managers
    QNetworkAccessManager *mNetObservations;
    QNetworkAccessManager *mNetForecast;
    QNetworkAccessManager *mNetRadar;
    QNetworkAccessManager *mNetDetailedForecast;
};

#endif // Weather_H
