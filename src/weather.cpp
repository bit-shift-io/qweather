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
}

Weather::~Weather()
{
    //mNetworkAccessManager->deleteLater();
}

QString Weather::station() const
{
    return mWMO;
}

void Weather::setStation(const QString &xUrl)
{
    // get station data
    Stations *stations = Stations::instance();
    QString url = stations->getObservationUrl(xUrl);
    QString forecast_url = stations->getForecastUrl(xUrl);
    QString area_code = stations->getAreaCode(xUrl);

    // assign url
    if(mObservationUrl != url)
    {
        mWMO = xUrl;
        mObservationUrl = url;
        mAreaCode = area_code;
        mForecastUrl = forecast_url;
        emit stationChanged();
    }
}

void Weather::update() {
    requestForecast();
    requestObservation();
}

void Weather::requestObservation()
{
    QUrl u = QUrl(mObservationUrl);
    QNetworkRequest request;
    request.setUrl(u);
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );

    QNetworkAccessManager *net = new QNetworkAccessManager(this);
    connect(net, &QNetworkAccessManager::finished, this, &Weather::replyObservationFinished);
    net->get(request);
}

void Weather::requestForecast()
{
    QUrl u = QUrl(mForecastUrl);
    QNetworkRequest request;
    request.setUrl(u);
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );

    QNetworkAccessManager *net = new QNetworkAccessManager(this);
    connect(net, &QNetworkAccessManager::finished, this, &Weather::replyForecastFinished);
    net->get(request);
}

void Weather::replyObservationFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() != QNetworkReply::NoError) {
        qDebug() << "network error " << xNetworkReply->error() << " " << xNetworkReply->url().toString();
        xNetworkReply->deleteLater();
        return;
    }

    QString url = xNetworkReply->url().toString();
    QByteArray data = xNetworkReply->readAll();
    xNetworkReply->deleteLater();
    QJsonObject weather_data;
    qDebug() << "download " << data.size() << "bytes";
    //qDebug() << QString(data);
    //qDebug() << mUrl;


    // json version
    // this is for the observations
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
    weather_data = station_data[0].toObject();
    //qDebug() << weather_data;

    // debug
    //qDebug() << QJsonDocument(result).toJson(QJsonDocument::Compact).toStdString().c_str();
    emit resultObservationFinished(weather_data);
}

void Weather::replyForecastFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() != QNetworkReply::NoError) {
        qDebug() << "network error " << xNetworkReply->error() << " " << xNetworkReply->url().toString();
        xNetworkReply->deleteLater();
        return;
    }

    QString url = xNetworkReply->url().toString();
    QByteArray data = xNetworkReply->readAll();
    xNetworkReply->deleteLater();
    QJsonObject weather_data;
    qDebug() << "download " << data.size() << "bytes";
    //qDebug() << QString(data);
    //qDebug() << mUrl;


    // XML version
    // this is for the forecast
    QXmlStreamReader xml(data);
    QJsonObject result;

    QString cur_area_name;
    QString cur_forecast_date;
    QJsonObject cur_forecast;
    QJsonObject cur_info;

    /*
     * data should look like this...
    {
       "Adelaide" : {
        "lat": xx
        "lon": xx
        "forecast": {
                "2020-10-02": {}
                "2020-10-03": {}
                "2020-10-04": {}
            }
       }
       "Area Name" : {
       }
    }*/
    while(!xml.atEnd())
    {
        if (xml.readNextStartElement()) {

            // qDebug() << xml.name();

            if(xml.name()=="area" && !xml.isEndElement())
            {
                // new area
                QString type = xml.attributes().value("type").toString();
                if (type == "location") {

                    // previous area
                    if (cur_forecast.isEmpty()) {
                        result.insert(cur_area_name, cur_forecast);
                        // TODO: need to put lon/lat here from database for matching location
                    }

                    QString cur_area_name = xml.attributes().value("description").toString();

                    // reset forecast
                    cur_forecast = {};

                    //qDebug() << name;
                }

            }

            if(xml.name()=="forecast-period" && !xml.isEndElement())
            {
                // previous forecast
                if (!cur_forecast.isEmpty()) {
                    cur_forecast.insert(cur_forecast_date, cur_info);
                }

                // new forecast
                QString cur_forecast_date = xml.attributes().value("start-time-local").toString().left(10);

                // reset info
                cur_info = {};

                //qDebug() << date;
            }

            if (xml.name()=="element" || xml.name()=="text")
            {
                // forecast info
                QString type = xml.attributes().value("type").toString();
                QString value = xml.readElementText();
                cur_info.insert(type, value);
                //qDebug() << type << " : " << value;
            }
        }
    }

    // debug
    //qDebug() << QJsonDocument(result).toJson(QJsonDocument::Compact).toStdString().c_str();
    //emit resultFinished(weather_data);
}
