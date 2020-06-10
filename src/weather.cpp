#include "weather.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
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
        QJsonObject cur_station {};

        while(!xml.atEnd())
        {
            if (xml.readNextStartElement()) {
                if(xml.name()=="station" && !xml.isEndElement())
                {

                    if (!cur_station.isEmpty()) {
                        // append previous station
                        result.insert(cur_station.value("id").toString(), cur_station);
                    }

                    QString name = xml.attributes().value("description").toString();
                    QString wmo_id = xml.attributes().value("wmo-id").toString();
                    //qDebug() << name << " " << wmo_id;

                    // new station json
                    cur_station = {
                        {"id", wmo_id},
                        {"name", name}
                    };

                }
                if(xml.name()=="element" && !xml.isEndElement())
                {

                    // append to current station
                    QString key = xml.attributes().value("type").toString();
                    QString value = xml.readElementText();
                    cur_station.insert(key, value);
                    //qDebug() << key << " " << value;
                }
                if (xml.name()=="station" && xml.isEndElement())
                {
                    qDebug() << "end station";
                }
            }
        }

        // debug
        //qDebug() << QJsonDocument(result).toJson(QJsonDocument::Compact).toStdString().c_str();

        emit resultFinished(result);
    } else {
        qDebug()<<"error";
    }

    //delete xNetworkReply; // do we need to clean? this crashes
}
