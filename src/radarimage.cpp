#include <QPainter>
#include <QTimer>

#include "radarimage.h"
#include "weather.h"

RadarImage::RadarImage()
{

}


void RadarImage::paint(QPainter *xPainter)
{
    if (mActiveFramePosition >= mRadarImages.size()) {
        //qDebug() << "frame pos to large!" << mActiveFramePosition;
        return;
    }

    //qDebug() << mActiveFramePosition;

    xPainter->drawImage(0,0, mRadarImages.at(mActiveFramePosition).scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void RadarImage::nextFrame()
{
    // this needs to be its own seperate value as paint is done in its own thread
    mActiveFramePosition = mFramePosition;

    if (mFramePosition >= mRadarImages.size()-1) {
        pauseTimer();
    }

    // super
    update();

    mFramePosition++;
}

void RadarImage::updateImages()
{
    mUpdateNextPause = true;
    startTimer();
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
        connect(mWeather, &Weather::resultRadarFinished, this, &RadarImage::updateImages);
    }
}



void RadarImage::startTimer()
{
    if (mTimer == nullptr) {
        mTimer = new QTimer(this);
        connect(mTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    }

    mFramePosition = 0;
    nextFrame();
    mTimer->start(300);
}

void RadarImage::pauseTimer()
{
    if (mTimer == nullptr)
        return;

    mTimer->stop();
    QTimer::singleShot(4000, this, &RadarImage::startTimer);

    if (mUpdateNextPause) {
        //qDebug() << "paused, updating images";
        mUpdateNextPause = false;
        mRadarImages = mWeather->copyRadarImages();
    }

}



