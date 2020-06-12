#ifndef Weather_H
#define Weather_H

#include <QObject>
#include <QNetworkReply>

class Weather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
public:
    Weather(QObject *parent = 0);
    ~Weather();

    QString url() const;
    void setUrl(const QString &xUrl);

public slots:
    void update();


signals:
    void urlChanged();
    void resultFinished(const QJsonObject &xResult);

private slots:
    void replyFinished(QNetworkReply *xNetworkReply);

private:
    void requestForecast();
    void requestWeather();
    QString mWMO;
    QString mUrl;
    QString mForecastUrl;
    QNetworkAccessManager *mNetworkAccessManager;
};

#endif // Weather_H
