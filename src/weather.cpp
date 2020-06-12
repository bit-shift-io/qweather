#include "weather.h"
#include "stations.h"
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QDebug>


Weather::Weather(QObject *parent) : QObject(parent)
{
    // setup network manager
    mNetworkAccessManager = new QNetworkAccessManager(this);
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &Weather::replyFinished);
}

Weather::~Weather()
{
    mNetworkAccessManager->deleteLater();
}

QString Weather::url() const
{
    return mUrl;
}

void Weather::setUrl(const QString &xUrl)
{
    // get station data
    Stations *stations = Stations::instance();
    QString url = stations->getUrlbyWMO(xUrl);

    // assign url
    if(mUrl != url)
    {
        mWMO = xUrl;
        mUrl = url;
        emit urlChanged();
    }
}

void Weather::requestWeather()
{
    QUrl u = QUrl(mUrl);
    QNetworkRequest request;
    request.setUrl(u);
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    mNetworkAccessManager->get(request);
}

void Weather::replyFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() == QNetworkReply::NoError)
    {
        QByteArray data = xNetworkReply->readAll();
        //qDebug() << "download " << data.size() << "bytes";
        //qDebug() << QString(data);
        //qDebug() << mUrl;

        // json version
        QJsonParseError JsonParseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &JsonParseError);
        QJsonObject obj = doc.object();

        QJsonObject observations = obj["observations"].toObject();
        QJsonArray station_data = observations["data"].toArray();
        if (station_data.size() == 0) {
            qDebug() << "no data";
            return;
        }

        // get the current data
        QJsonObject weather_data = station_data[0].toObject();
        //qDebug() << weather_data;

        /*
        // XML version
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
        */

        emit resultFinished(weather_data);
    } else {
        qDebug() << "network error " << xNetworkReply->error();
    }
}
