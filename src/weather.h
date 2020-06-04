#ifndef Weather_H
#define Weather_H

#include <QObject>
#include <QNetworkReply>

class WeatherPrivate;

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
    void requestWeather(const QString &xSearchString);

signals:
    void urlChanged();
    void resultFinished(const QJsonObject &xResult);

private slots:
    void replyFinished(QNetworkReply *xNetworkReply);

private:
    WeatherPrivate *m;
};

#endif // Weather_H
