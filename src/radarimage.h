#ifndef RADARIMAGE_H
#define RADARIMAGE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>
#include <QVector>

class Weather;

// https://stackoverflow.com/questions/41162110/extend-qml-image-type-with-an-update-function
class RadarImage : public QQuickPaintedItem
{
    Q_OBJECT
    //Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    // make aware of the weather class which provides the data
    Q_PROPERTY(Weather *weather READ weather WRITE setWeather)

public:
    RadarImage();
    void paint(QPainter *xPainer);
    void setImage(const QImage &xImage);
    void setBackgroundImage(const QImage &xImage);
    void setTopographyImage(const QImage &xImage);
    void setLocationsImage(const QImage &xImage);
    void setAnimationImage(const QVector<QImage> &xImages);

    Weather *weather() const;
    void setWeather(Weather *xWeather);
    void updateRadar(QImage xResult);

signals:
    void imageChanged();

private:
    QImage mImage;
    QImage mBackgroundImage;
    QImage mTopographyImage;
    QImage mLocationImage;
    QVector<QImage> mAnimationImages;
    Weather *mWeather;
};

#endif // RADARIMAGE_H
