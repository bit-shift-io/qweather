#include <QPainter>
#include <QNetworkReply>
#include <QTimer>

#include "radarimage.h"
#include "weather.h"

RadarImage::RadarImage()
{

}


void RadarImage::paint(QPainter *xPainter)
{
    if (mBackgroundImage == nullptr)
        return;

    // background
    xPainter->drawImage(0,0, mBackgroundImage->scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    if (mTopographyImage != nullptr) {
        // topology
        xPainter->drawImage(0,0, mTopographyImage->scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }


    if (mFramePosition != -1) {
        // radar
        QImage radar = mAnimationImages.values()[mFramePosition]->scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
        xPainter->drawImage(0,0, radar);
    }


    if (mLocationImage != nullptr) {
        // locations
        xPainter->drawImage(0,0, mLocationImage->scaled(width(), height(), Qt::KeepAspectRatio, Qt::FastTransformation));
    }

    //qDebug() << "paint " << mFramePosition;
}

void RadarImage::updateNext()
{
    if (!mAnimationImages.isEmpty()) {
        // increment the counter
        if (mFramePosition >= mAnimationImages.size()-1)
            mFramePosition = 0;
        else
            mFramePosition++;
    }

    // draw super
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

    if (mBackgroundImage == nullptr) {
        // one off images
        QString radar = mWeather->getRadarId();

        // ordered list
        QVector<QString> mBackgroundUrlList;
        mBackgroundUrlList.append(QString("http://www.bom.gov.au/products/radar_transparencies/%1.background.png").arg(radar));
        mBackgroundUrlList.append(QString("http://www.bom.gov.au/products/radar_transparencies/%1.topography.png").arg(radar));
        mBackgroundUrlList.append(QString("http://www.bom.gov.au/products/radar_transparencies/%1.locations.png").arg(radar));

        for (QString &url : mBackgroundUrlList) {
            QNetworkRequest request;
            request.setUrl(url);
            request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
            net->get(request);
        }
    }

    for (QString &item : *mFileList) {
        // get ftp images
        QString u = QString("ftp://ftp.bom.gov.au/anon/gen/radar/%1").arg(item);
        QNetworkRequest request;
        request.setUrl(u);
        request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
        net->get(request);
    }

    // start the animation
    startTimer();

    // clean network
    //net->deleteLater();
}

void RadarImage::startTimer()
{
    if (mTimer == nullptr) {
        mTimer = new QTimer(this);
        connect(mTimer, SIGNAL(timeout()), this, SLOT(updateNext()));
    }

    mTimer->start(500);
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
    //qDebug() << file_name;

    QByteArray data = xNetworkReply->readAll();
    xNetworkReply->deleteLater();

    // image from data
    // TODO: should these be pointers? I think so!
    QImage *img = new QImage();
    img->loadFromData(data);

    // check image is in correct format
    // so we can blend them together
    if (img->format() == QImage::Format_Indexed8) {
        *img = img->convertToFormat(QImage::Format_ARGB32);
    }

    if (url.contains("background.png")) {
        mBackgroundImage = img;
        setImplicitWidth(mBackgroundImage->width());
        setImplicitHeight(mBackgroundImage->height());
    } else if (url.contains("topography.png")) {
        mTopographyImage = img;
    } else if (url.contains("locations.png")) {
        mLocationImage = img;
    } else if (url.contains("ftp.bom.gov.au")) {
        mAnimationImages.insert(file_name, img);
    }

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
