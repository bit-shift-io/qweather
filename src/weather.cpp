#include "weather.h"
#include "database.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QDebug>
#include <QImage>
#include <QEventLoop>


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
    Database *database = Database::instance();
    QString url = database->getObservationUrl(xUrl);
    QString forecast_url = database->getForecastUrl(xUrl);
    QString area_code = database->getAreaCode(xUrl);
    QString radar_url = database->getRadarUrl(xUrl);

    // assign url
    if(mObservationUrl != url)
    {
        mWMO = xUrl;
        mObservationUrl = url;
        mAreaCode = area_code;
        mForecastUrl = forecast_url;
        mRadarUrl = radar_url;
        emit stationChanged();
    }
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

void Weather::requestRadar()
{
    QNetworkAccessManager *net = new QNetworkAccessManager(this);
    QUrl u = QUrl(mRadarUrl + "background.png");
    QNetworkRequest request;
    request.setUrl(u);
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );

    // async method
    connect(net, &QNetworkAccessManager::finished, this, &Weather::replyRadarFinished);
    net->get(request);

    /*
    // synchronous method!
    QNetworkReply* reply = net->get(request);
    QEventLoop eventLoop;
    connect(net,SIGNAL(finished()),&eventLoop,SLOT(quit()));
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QImageReader imageReader(reply);
        imageReader.setAutoDetectImageFormat (false);
        QImage pic = imageReader.read();
        emit resultRadarFinished(pic);
        //ui->label_2->setPixmap(QPixmap::fromImage(pic));

     }
     */
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
    //qDebug() << "download " << data.size() << "bytes";
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
    mObservationData = weather_data;
    emit resultObservationFinished(weather_data);
}

void Weather::replyRadarFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() != QNetworkReply::NoError) {
        qDebug() << "network error " << xNetworkReply->error() << " " << xNetworkReply->url().toString();
        xNetworkReply->deleteLater();
        return;
    }

    QString url = xNetworkReply->url().toString();
    QByteArray data = xNetworkReply->readAll();
    QImage *img = new QImage();
    img->loadFromData(data);

    xNetworkReply->deleteLater();
    emit resultRadarFinished(*img);
    return;
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
    //qDebug() << "download " << data.size() << "bytes";
    //qDebug() << QString(data);
    //qDebug() << url;

    // get database instance
    Database *database = Database::instance();

    // XML version
    // this is for the forecast
    QXmlStreamReader xml(data);

    QJsonObject result;
    QJsonArray forecast;
    QJsonObject forecast_info;

    /*
     * data should look like this...
    {
        "forecast": [
            [{"date":"2020-10-02", ....}],
            [{"date":"2020-10-03", ...}],
            [{"date":"2020-10-04", ...}],
        ]
    }
    */
    bool area_found = false;

    while(!xml.atEnd())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {
            if (!area_found) {
                // search for area code

                if (xml.name()=="area") {
                    QString type = xml.attributes().value("type").toString();
                    QString area_code = xml.attributes().value("aac").toString();
                    if (type == "location" and area_code == mAreaCode) {
                        area_found = true;
                        //qDebug() << xml.attributes().value("description").toString();
                    }
                }
            }
            else {
                // area is now found, lets get teh data

                if(xml.name()=="forecast-period")
                {
                    forecast_info = QJsonObject();
                    QString cur_forecast_date = xml.attributes().value("start-time-local").toString().left(10);
                    forecast_info.insert("date", cur_forecast_date);
                    //qDebug() << cur_forecast_date;
                }

                if (xml.name()=="element" || xml.name()=="text")
                {
                    // forecast info
                    QString type = xml.attributes().value("type").toString();
                    QString value = xml.readElementText();

                    if (type == "precipitation_range")
                        value.replace(" mm", "").replace(" to ", "-");

                    if (type == "probability_of_precipitation")
                        value.replace("%", "");

                    if (type == "precis")
                        value.replace(".", "");

                    forecast_info.insert(type, value);
                    //qDebug() << type << " : " << value;
                }
            }
        }

        if(token == QXmlStreamReader::EndElement) {
            if (area_found) {
                // area is found, now we care about the end tags

                if(xml.name()=="forecast-period") {
                    // replace icon code with actual icon
                    int icon_code = forecast_info["forecast_icon_code"].toString().toInt(); // some bug, need to goto string first
                    QString description = forecast_info["precis"].toString();
                    forecast_info.insert("forecast_icon", database->getIcon(icon_code, description));
                    //qDebug() << icon_code << " " << description;
                    //qDebug() << database->getIcon(icon_code, description);

                    // append json
                    forecast.append(forecast_info);
                }

                if (xml.name()=="area") {
                    // close the area
                    xml.clear();
                    result.insert("forecast", forecast);

                    // debug
                    //qDebug() << QJsonDocument(result).toJson(QJsonDocument::Compact).toStdString().c_str();
                    mForecastData = result;
                    emit resultForecastFinished(result);
                    return;
                }
            }
        }


    }

}
