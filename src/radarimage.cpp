#include <QPainter>
#include <QTimer>

#include "radarimage.h"
#include "weather.h"

RadarImage::RadarImage()
{

}


void RadarImage::paint(QPainter *xPainter)
{
    if (mActiveFrame == nullptr)
        return;

    xPainter->drawImage(0,0, mActiveFrame->scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void RadarImage::nextFrame()
{
    mActiveFrame = &mRadarImages.at(mFramePosition);
    mFramePosition++;

    if (mFramePosition == mRadarImages.size())
        pauseTimer();

    // super
    update();
}

void RadarImage::updateImages()
{
    mRadarImages = mWeather->copyRadarImages();
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
}



