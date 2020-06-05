#include "weather.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QXmlStreamReader>
#include <QDebug>

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
    QUrl u = QUrl(m->mUrl); // + xSearchString);
    QNetworkRequest request;
    request.setUrl(u);
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    m->mNetworkAccessManager->get(request);
}

void Weather::replyFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() == QNetworkReply::NoError)
    {
        QByteArray data = xNetworkReply->readAll();
        qDebug() << "XML download size:" << data.size() << "bytes";
        //qDebug() << QString(data);
        QXmlStreamReader xml(data);

        //qDebug() << data;
        //Q_ASSERT(xml.isStartElement());

        QJsonObject result;


        while(!xml.atEnd())
        {
            if (xml.readNextStartElement()) {
                if(xml.name()=="station" && !xml.isEndElement())
                {
                    QString name = xml.attributes().value("description").toString();
                    QString wmo_id = xml.attributes().value("wmo-id").toString();

                    // process station



                    qDebug() << name << " " << wmo_id;
                }
            }
        }

        //QJsonObject tJsonObject = QJsonDocument::fromJson(xNetworkReply->readAll()).object();
        emit resultFinished(result);
    } else {
        qDebug()<<"error";
    }
    delete xNetworkReply;
}
