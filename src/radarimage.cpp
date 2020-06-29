#include <QPainter>
#include <QNetworkReply>
#include <QTimer>
#include <QStandardPaths>
#include <QFile>
#include <QDir>

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

    if (!mAnimationImages.isEmpty()) {
        // radar
        QImage *img = mAnimationImages[mFramePosition];
        if (img != nullptr)
            xPainter->drawImage(0,0, img->scaled(width(), height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        //qDebug() << "frame " << mFramePosition << " of " << mAnimationImages.size()-1;


        if (mFramePosition >= mAnimationImages.size()-1 && mPauseCount == 0) {
            // we hit the last frame
            // set the pause count
            mPauseCount = 4;
            //qDebug() << "pause";
        }
        else if (mPauseCount > 0) {
            // we are a repeat frame
            //qDebug() << "repeat";
            mPauseCount--;

            if (mPauseCount == 0) {
                // end pause
                // reset animation
                //qDebug() << "end";
                mFramePosition = 0;
            }
        }
        else {
            // increment frame
            mFramePosition++;
        }

    }

    if (mLocationImage != nullptr) {
        // locations
        xPainter->drawImage(0,0, mLocationImage->scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
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

        // first try and load from cache
        mBackgroundImage = read(QString("%1.background.png").arg(radar));
        mTopographyImage = read(QString("%1.topography.png").arg(radar));
        mLocationImage = read(QString("%1.locations.png").arg(radar));

        if (mBackgroundImage == nullptr) {
            // get from online source
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
    }


    // match file list size
    mAnimationImages = QVector<QImage*>(mFileList->size());

    // TODO: use existing mAnimationImages?
    for (int i=0; i < mFileList->size(); i++) {
        QString item = mFileList->at(i);

        // check cache
        QImage* img = read(item);
        if (img != nullptr) {
            mAnimationImages[i] = img;
        } else {
            // get ftp images
            QString u = QString("ftp://ftp.bom.gov.au/anon/gen/radar/%1").arg(item);
            QNetworkRequest request;
            request.setUrl(u);
            request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
            net->get(request);
        }
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
        connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
    }

    mFramePosition = 0;
    update();
    mTimer->start(300);
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
        for (int i=0; i < mFileList->size(); i++){
            if (mFileList->at(i).contains(file_name)){
                mAnimationImages[i] = img;
            }
        }
    }

    write(file_name, img);
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

QImage* RadarImage::read(const QString xFileName)
{
    QString cache_dir = QStandardPaths::locate(QStandardPaths::CacheLocation, QString(), QStandardPaths::LocateDirectory);
    QString station_cache = QDir(cache_dir).filePath(mWeather->station());
    QString path = QDir(station_cache).filePath(xFileName);
    if (!QFile::exists(path))
        return nullptr;

    // load from file
    QImage *img = new QImage();
    img->load(path);

    return img;
}

void RadarImage::write(const QString xFileName, QImage* xImage)
{
    QString cache_dir = QStandardPaths::locate(QStandardPaths::CacheLocation, QString(), QStandardPaths::LocateDirectory);
    QString station_cache = QDir(cache_dir).filePath(mWeather->station());

    // check directory exists
    QDir dir;
    if (!dir.exists(station_cache))
        dir.mkpath(station_cache);

    QString path = QDir(station_cache).filePath(xFileName);
    xImage->save(path);
}
