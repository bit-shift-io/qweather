#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QDebug>

#include "weather.h"
#include "database.h"


Weather::Weather(QObject *parent) : QObject(parent)
{
}

Weather::~Weather()
{
    //mNetworkAccessManager->deleteLater();
}

QString Weather::station() const
{
    return mStationId;
}

void Weather::setStation(const QString &xStationId)
{
    // get station data
    Database *database = Database::instance();
    QString observation_url = database->getObservationUrl(xStationId);
    QString forecast_url = database->getForecastUrl(xStationId);
    QString area_code = database->getAreaCode(xStationId);
    QString radar_id = database->getRadarId(xStationId);
    QString detailed_forecast_url = database->getDetailedUrl(xStationId);

    // assign url
    if(mStationId != xStationId)
    {
        mStationId = xStationId;
        mObservationUrl = observation_url;
        mAreaCode = area_code;
        mForecastUrl = forecast_url;
        mRadarId = radar_id;
        mDetailedForecastUrl = detailed_forecast_url;
        emit stationChanged();
    }
}

QString Weather::getRadarId() const
{
    return mRadarId;
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
    // pass the request to radar
    // which is its own element in cpp
    emit updateRadar();
}

void Weather::requestDetailedForecast()
{
    QUrl u = QUrl(mDetailedForecastUrl);
    QNetworkRequest request;
    request.setUrl(u);
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );

    QNetworkAccessManager *net = new QNetworkAccessManager(this);
    connect(net, &QNetworkAccessManager::finished, this, &Weather::replyDetailedForecastFinished);
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

void Weather::replyDetailedForecastFinished(QNetworkReply *xNetworkReply)
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


    QString html = QString::fromUtf8(data);

    // trim pre html
    //int html_start = html.lastIndexOf("<html");
    //html = html.replace(0, html_start, "");

    // remove head
    int start = html.lastIndexOf("<head>");
    int end = html.lastIndexOf("</head>");
    html.replace(start, end-start+7, "");

    // remove extra div
    //html.left(html.size()-6);

    // this symbol causing issue
    html.replace("&ndash;", "");
    html.replace("&deg;", "");
    html.replace("&trade;", "");

    // add end tags
    //html.append("</body></html>");

    //qPrintable(html);
    qDebug().noquote() << html;

    // get database instance
    Database *database = Database::instance();

    // XML version
    // this is for the forecast
    QXmlStreamReader xml(html);



    while(!xml.atEnd())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        //qDebug() << QXmlStreamReader::TokenType(token);

        if (xml.hasError()) {
            qDebug() << xml.errorString();
        }

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {
            //qDebug() << "start " << xml.name();

            if (xml.name()=="div") {
                QString item_class = xml.attributes().value("class").toString();
                if (item_class.contains("forecast-day")) {
                    QString date = xml.attributes().value("id").toString();
                    qDebug() << "forecast" << date;
                }
            }

            if (xml.name()=="table") {
                QString summary = xml.attributes().value("summary").toString();
                qDebug() << summary;

            }

            if (xml.name()=="tr") {
                qDebug() << "row";
            }

            if (xml.name()=="td") {
                QString value = xml.readElementText();
                if (xml.hasError())
                    qDebug() << xml.errorString();
                qDebug() << value;
            }
        }

        if(token == QXmlStreamReader::EndElement) {
            //qDebug() << "end " << xml.name();


        }


    }



    emit resultDetailedForecastFinished(weather_data);
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
