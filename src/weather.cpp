#include "weather.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

class WeatherPrivate
{
    public:
        QString mUrl;
        QNetworkAccessManager *mNetworkAccessManager;
};

Weather::Weather(QObject *parent) : QObject(parent)
{
    m = new WeatherPrivate;
    m->mNetworkAccessManager = new QNetworkAccessManager(this);
    connect(m->mNetworkAccessManager, &QNetworkAccessManager::finished, this, &Weather::replyFinished);
}

Weather::~Weather()
{
    delete m;
}

QString Weather::url() const
{
    return m->mUrl;
}

void Weather::setUrl(const QString &xUrl)
{
    if(m->mUrl != xUrl)
    {
        m->mUrl = xUrl;
        emit urlChanged();
    }
}

void Weather::requestWeather(const QString &xSearchString)
{
    m->mNetworkAccessManager->get(QNetworkRequest(QUrl(m->mUrl + xSearchString)));
}

void Weather::replyFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() == QNetworkReply::NoError)
    {
        QJsonObject tJsonObject = QJsonDocument::fromJson(xNetworkReply->readAll()).object();
        emit resultFinished(tJsonObject);
    }
    delete xNetworkReply;
}
