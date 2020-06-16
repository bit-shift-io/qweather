#include <QPainter>
#include "radarimage.h"
#include "weather.h"

RadarImage::RadarImage()
{

}


void RadarImage::paint(QPainter *painter)
{
    QImage scaled = mImage.scaled(width(), height());
    painter->drawImage(0,0, scaled);
}

void RadarImage::setImage(const QImage &image)
{
    mImage = image;
    emit imageChanged();
    setImplicitWidth(image.width());
    setImplicitHeight(image.height());
    update();
}

Weather *RadarImage::weather() const
{
    return mWeather;
}

void RadarImage::setWeather(Weather *weather)
{
    if (mWeather) {
        mWeather->disconnect(this);
    }

    mWeather = weather;

    if (mWeather) {
        connect(mWeather, &Weather::resultRadarFinished, this, &RadarImage::updateRadar);
    }
}

void RadarImage::updateRadar(QImage xResult)
{
    qDebug() << "radar cpp";
    setImage(xResult);
}
