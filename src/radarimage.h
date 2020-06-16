#ifndef RADARIMAGE_H
#define RADARIMAGE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>

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
    void paint(QPainter *painer);
    void setImage(const QImage &image);

    Weather *weather() const;
    void setWeather(Weather *weather);
    void updateRadar(QImage xResult);

signals:
    void imageChanged();

private:
    QImage mImage;
    Weather *mWeather;

};

#endif // RADARIMAGE_H
