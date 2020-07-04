#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QImage>
#include <QPainter>

#include "weather.h"
#include "database.h"


Weather::Weather(QObject *parent) : QObject(parent)
{
}

Weather::~Weather()
{
    destroyRadarImages();
    destroyRadarFrames();
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
        read(); // read cache
    }
}

bool Weather::radarEnabled() const
{
    return mRadarEnabled;
}

void Weather::setRadarEnabled(const bool &xRadar)
{
    if (mRadarEnabled != xRadar) {
        mRadarEnabled = xRadar;
        emit radarEnabledChanged();
    }
}

QString Weather::getRadarId() const
{
    return mRadarId;
}

QVector<QImage> Weather::copyRadarImages()
{
    QVector<QImage> copy;
    for (int i=0; i < mRadarImages.size(); i++){
        copy.append(mRadarImages[i].copy());
    }
    return copy;
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



void Weather::ftpAddToList(const QUrlInfo &xUrlInfo)
{
    if (xUrlInfo.name().startsWith(mRadarId) && xUrlInfo.name().endsWith(".png")) {
        //qDebug() << xUrlInfo.name() << " - " << xUrlInfo.size() << " bytes";
        mRadarFileList.append(xUrlInfo.name());
    }
}


void Weather::ftpCommandFinished(int commandId, bool error)
{
    if (mFtp->currentCommand() == QFtp::List) {
        mFtp->close();
        mFtp->deleteLater();
        requestRadarImages();
        return;
    }
}


QImage Weather::read(const QString xFileName)
{
    QImage img;

    QString cache_dir = QStandardPaths::locate(QStandardPaths::CacheLocation, QString(), QStandardPaths::LocateDirectory);
    QString station_cache = QDir(cache_dir).filePath(mStationId);
    QString path = QDir(station_cache).filePath(xFileName);
    if (!QFile::exists(path))
        return img;

    // load from file
    img.load(path);

    return img;
}


void Weather::write(const QString xFileName, QImage &xImage)
{
    QString cache_dir = QStandardPaths::locate(QStandardPaths::CacheLocation, QString(), QStandardPaths::LocateDirectory);
    QString station_cache = QDir(cache_dir).filePath(mStationId);

    // check directory exists
    QDir dir;
    if (!dir.exists(station_cache))
        dir.mkpath(station_cache);

    QString path = QDir(station_cache).filePath(xFileName);
    xImage.save(path);
}


void Weather::destroyRadarImages()
{
    mRadarImages.clear();
}

void Weather::destroyRadarFrames()
{
    mRadarFrames.clear();
}


void Weather::renderRadarImages()
{
    if (mRadarBackground.isNull())
        return;

    destroyRadarImages();
    mRadarImages = QVector<QImage>(mRadarFileList.size());

    for (int i=0; i < mRadarFrames.size(); i++){
        QImage surface(mRadarBackground);
        QPainter painter(&surface);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.drawImage(0,0, mRadarTopography);
        painter.drawImage(0,0, mRadarFrames[i]);
        painter.drawImage(0,0, mRadarLocation);
        painter.end();
        mRadarImages[i] = surface;
    }

    emit resultRadarFinished();
}


void Weather::replyRadarImageFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() != QNetworkReply::NoError) {
        qDebug() << "network error " << xNetworkReply->error() << " " << xNetworkReply->url().toString();
        xNetworkReply->deleteLater();
        return;
    }

    QString url = xNetworkReply->url().toString();
    QString file_name = QUrl(url).fileName();
    //qDebug() << file_name;

    QByteArray data = xNetworkReply->readAll();
    xNetworkReply->deleteLater();

    // image from data
    QImage img;
    img.loadFromData(data);

    // check image is in correct format
    // so we can blend them together
    if (img.format() == QImage::Format_Indexed8) {
        img = img.convertToFormat(QImage::Format_ARGB32);
    }

    if (url.contains("background.png")) {
        mRadarImageRequests--;
        mRadarBackground = img;
    } else if (url.contains("topography.png")) {
        mRadarImageRequests--;
        mRadarTopography = img;
    } else if (url.contains("locations.png")) {
        mRadarImageRequests--;
        mRadarLocation = img;
    } else if (url.contains("ftp.bom.gov.au")) {
        for (int i=0; i < mRadarFileList.size(); i++){
            if (mRadarFileList.at(i).contains(file_name)){
                mRadarImageRequests--;
                mRadarFrames[i] = img;
            }
        }
    }

    write(file_name, img);

    if (mRadarImageRequests == 0)
        renderRadarImages();

    return;
}


void Weather::requestRadar()
{
    if (!mRadarEnabled)
        return;

    // keep a track of how many files are requested to download
    // some are in cache
    mRadarImageRequests = 0;

    // request the static radar images
    QNetworkAccessManager *net = new QNetworkAccessManager(this);
    connect(net, &QNetworkAccessManager::finished, this, &Weather::replyRadarImageFinished);

    if (mRadarBackground.isNull()) {
        // first try and load from cache
        mRadarBackground = read(QString("%1.background.png").arg(mRadarId));

        if (mRadarBackground.isNull()) {
            mRadarImageRequests++;
            QNetworkRequest request;
            request.setUrl(QString("http://www.bom.gov.au/products/radar_transparencies/%1.background.png").arg(mRadarId));
            net->get(request);
        }
    }

    if (mRadarTopography.isNull()) {
        // first try and load from cache
        mRadarTopography = read(QString("%1.topography.png").arg(mRadarId));

        if (mRadarTopography.isNull()) {
            mRadarImageRequests++;
            QNetworkRequest request;
            request.setUrl(QString("http://www.bom.gov.au/products/radar_transparencies/%1.topography.png").arg(mRadarId));
            net->get(request);
        }
    }

    if (mRadarLocation.isNull()) {
        // first try and load from cache
        mRadarLocation = read(QString("%1.locations.png").arg(mRadarId));

        if (mRadarLocation.isNull()) {
            mRadarImageRequests++;
            QNetworkRequest request;
            request.setUrl(QString("http://www.bom.gov.au/products/radar_transparencies/%1.locations.png").arg(mRadarId));
            net->get(request);
        }
    }


    // radar images from the ftp
    mRadarFileList.clear();

    // ftp listing
    mFtp = new QFtp(this);
    connect(mFtp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(ftpAddToList(QUrlInfo)));
    connect(mFtp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCommandFinished(int, bool)));

    mFtp->connectToHost("ftp.bom.gov.au");
    mFtp->login();
    mFtp->cd("/anon/gen/radar/");
    mFtp->list(); // this is async
}


void Weather::requestRadarImages()
{
    // we now have our file listing, lets request the images
    QNetworkAccessManager *net = new QNetworkAccessManager(this);
    connect(net, &QNetworkAccessManager::finished, this, &Weather::replyRadarImageFinished);

    // match file list size
    destroyRadarFrames();
    mRadarFrames = QVector<QImage>(mRadarFileList.size());

    // TODO: use existing mAnimationImages?
    for (int i=0; i < mRadarFileList.size(); i++) {
        QString item = mRadarFileList.at(i);

        // check cache
        QImage img = read(item);
        if (!img.isNull()) {
            mRadarFrames[i] = img;
        } else {
            // get ftp images
            mRadarImageRequests++;
            QString u = QString("ftp://ftp.bom.gov.au/anon/gen/radar/%1").arg(item);
            QNetworkRequest request;
            request.setUrl(u);
            request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
            net->get(request);
        }
    }

    // check if we have loaded all the images
    if (mRadarImageRequests == 0)
        renderRadarImages();

    // clean network
    //net->deleteLater();
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
    QJsonArray day_hour_data;
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
    html.replace("&amp;", "");
    html.replace(";", "");
    html.replace("&", "");

    // add end tags
    //html.append("</body></html>");

    //qDebug().noquote() << html;

    // get database instance
    Database *database = Database::instance();

    // XML version
    // this is for the forecast
    QXmlStreamReader xml(html);


    /*
     * data should look like this...
    {
        "outlook": [
            [{"date":"2020-10-02", "time(24hr)":"0930", "chance_of_rain":"60", "air_temp":"24.2" ....}],
            [{"date":"2020-10-02", "time":"1230" ...}],
            [{"date":"2020-10-04", ...}],
        ]
    }
    */

    int day_count = -1;
    int row_count = 0;
    int column_count = 0;
    bool in_body = false;

    /*
    while(!xml.atEnd())
    {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (xml.hasError()) {
            qDebug() << xml.errorString();
        }

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {
            if (xml.name()=="div") {
                QString item_class = xml.attributes().value("class").toString();
                if (item_class.contains("forecast-day")) {
                    QString date = xml.attributes().value("id").toString().right(10);

                    // populate dates and times for new day
                    for (QString time : detailed_column_times) {
                        QJsonObject column; // pointer for this??
                        column.insert("date", date);
                        column.insert("time", time);
                        day_hour_data.append(column);
                        //qDebug() << "forecast" << date << time;
                    }

                    // set day count
                    day_count++;
                    row_count = 0;
                    qDebug() << "day" << day_count;
                }
            }

            if (xml.name()=="tbody") {
                in_body = true;
                qDebug() << "tbody";
            }

            if (in_body) {
                if (xml.name()=="th") {
                    // head of row, reset counter
                    //QString value = xml.readElementText();
                    qDebug() << detailed_row_labels[row_count];
                    column_count = 0;
                }

                if (xml.name()=="td") {
                    QString value = xml.readElementText();
                    if (xml.hasError())
                        qDebug() << xml.errorString();
                    qDebug() << detailed_column_times[column_count] << value;
                    column_count++;
                }
            }
        }

        if(token == QXmlStreamReader::EndElement) {
            if (in_body) {
                if (xml.name()=="tr") {
                    // end of row, increment counters
                    row_count++;
                    qDebug() << "end row";
                }

                if (xml.name()=="tbody") {
                    qDebug() << "end tbody";
                    in_body = false;
                }
            }
        }

    }
    */


    emit resultDetailedForecastFinished(weather_data);
    write();
}

void Weather::read()
{
    QString cache_dir = QStandardPaths::locate(QStandardPaths::CacheLocation, QString(), QStandardPaths::LocateDirectory);
    QString station_cache = QDir(cache_dir).filePath(mStationId);

    if (mForecastData.isEmpty()) {
        QString path = QDir(station_cache).filePath("forecast.json");
        QFile file(path);
        if (file.exists()) {
            file.open(QFile::ReadOnly);
            mForecastData = QJsonDocument().fromJson(file.readAll()).object();
            file.close();
            emit resultForecastFinished(mForecastData);
            //qDebug() << QJsonDocument(mForecastData).toJson(QJsonDocument::Compact).toStdString().c_str();
        }
    }

    if (mObservationData.isEmpty()) {
        QString path = QDir(station_cache).filePath("observation.json");
        QFile file(path);
        if (file.exists()) {
            file.open(QFile::ReadOnly);
            mObservationData = QJsonDocument().fromJson(file.readAll()).object();
            file.close();
            emit resultObservationFinished(mObservationData);
            //qDebug() << QJsonDocument(mObservationData).toJson(QJsonDocument::Compact).toStdString().c_str();
        }
    }
}

void Weather::write()
{
    QString cache_dir = QStandardPaths::locate(QStandardPaths::CacheLocation, QString(), QStandardPaths::LocateDirectory);
    QString station_cache = QDir(cache_dir).filePath(mStationId);

    // check directory exists
    QDir dir;
    if (!dir.exists(station_cache))
        dir.mkpath(station_cache);

    if (!mForecastData.isEmpty()) {
        QString path = QDir(station_cache).filePath("forecast.json");
        QFile file(path);
        file.open(QFile::WriteOnly);
        file.write(QJsonDocument(mForecastData).toJson(QJsonDocument::Compact));
        file.close();
    }

    if (!mObservationData.isEmpty()) {
        QString path = QDir(station_cache).filePath("observation.json");
        QFile file(path);
        file.open(QFile::WriteOnly);
        file.write(QJsonDocument(mObservationData).toJson(QJsonDocument::Compact));
        file.close();
    }
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
                    write();
                    return;
                }
            }
        }

    }

}
