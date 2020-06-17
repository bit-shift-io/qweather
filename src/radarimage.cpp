#include <QPainter>
#include <QNetworkReply>


#include "radarimage.h"
#include "weather.h"

RadarImage::RadarImage()
{

}


void RadarImage::paint(QPainter *xPainter)
{
    if (mImage.isNull())
        return;

    QImage scaled = mImage.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    xPainter->drawImage(0,0, scaled);
}

void RadarImage::setImage(const QImage &xImage)
{
    mImage = xImage;
    emit imageChanged();
    setImplicitWidth(xImage.width());
    setImplicitHeight(xImage.height());
    update();
}

Weather *RadarImage::weather() const
{
    return mWeather;
}

void RadarImage::setWeather(Weather *xWeather)
{
    if (mWeather) {
        // crash on windows?
        //mWeather->disconnect(this);
    }

    mWeather = xWeather;

    if (mWeather) {
        connect(mWeather, &Weather::updateRadar, this, &RadarImage::updateRadar);
    }
}


void RadarImage::updateRadar()
{
    mFileList = new QVector<QString>;

    // ftp listing
    mFtp = new QFtp(this);
    connect(mFtp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(ftpAddToList(QUrlInfo)));
    connect(mFtp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCommandFinished(int, bool)));

    // TODO: fix qftp
    //string[0] is depreciated
    mFtp->connectToHost("ftp.bom.gov.au");
    mFtp->login();
    mFtp->cd("/anon/gen/radar/");
    mFtp->list(); // this is async
}

void RadarImage::requestImages()
{
    QNetworkAccessManager *net = new QNetworkAccessManager(this);
    connect(net, &QNetworkAccessManager::finished, this, &RadarImage::replyImageFinished);

    // one off images
    if (mBackgroundUrlList == nullptr) {
        QString radar = mWeather->getRadarId();
        // ordered list
        mBackgroundUrlList = new QVector<QString>;
        mBackgroundUrlList->append(QString("http://www.bom.gov.au/products/radar_transparencies/%1.background.png").arg(radar));
        mBackgroundUrlList->append(QString("http://www.bom.gov.au/products/radar_transparencies/%1.topography.png").arg(radar));
        mBackgroundUrlList->append(QString("http://www.bom.gov.au/products/radar_transparencies/%1.locations.png").arg(radar));

        for (QString &url : *mBackgroundUrlList) {
            QNetworkRequest request;
            request.setUrl(url);
            request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
            net->get(request);
        }
    }


    // get ftp images
    for (QString &item : *mFileList) {
        QString u = QString("ftp://ftp.bom.gov.au/anon/gen/radar/%1").arg(item);
        QNetworkRequest request;
        request.setUrl(u);
        request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
        net->get(request);
    }
}


void RadarImage::replyImageFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() != QNetworkReply::NoError) {
        qDebug() << "network error " << xNetworkReply->error() << " " << xNetworkReply->url().toString();
        xNetworkReply->deleteLater();
        return;
    }

    QString url = xNetworkReply->url().toString();
    QString file_name = QUrl(url).fileName();
    qDebug() << file_name;

    QByteArray data = xNetworkReply->readAll();
    xNetworkReply->deleteLater();

    // image from data
    // TODO: should these be pointers? I think so!
    QImage img;
    img.loadFromData(data);

    if (url.contains("radar_transparencies")) {
        // background images
        mBackgroundImages.append(img);
    }

    if (url.contains("ftp.bom.gov.au")) {
        // animation images
        mAnimationImages.insert(file_name, img);
    }


    //setImage(mBackgroundImage);
    return;
}

void RadarImage::ftpAddToList(const QUrlInfo &xUrlInfo)
{
    QString radar_id = mWeather->getRadarId();
    if (xUrlInfo.name().startsWith(radar_id) && xUrlInfo.name().endsWith(".png")) {
        //qDebug() << xUrlInfo.name() << " - " << xUrlInfo.size() << " bytes";
        mFileList->append(xUrlInfo.name());
    }
}

void RadarImage::ftpCommandFinished(int commandId, bool error)
{
    if (mFtp->currentCommand() == QFtp::List) {
        mFtp->close();
        mFtp->deleteLater();
        requestImages();
        return;
    }
}
