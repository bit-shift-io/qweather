#include <QPainter>
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
        connect(mWeather, &Weather::resultRadarFinished, this, &RadarImage::updateRadar);
    }
}

void RadarImage::updateRadar(QImage xResult)
{
    qDebug() << "radar cpp";
    setImage(xResult);
}
