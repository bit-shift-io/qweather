#ifndef RADARIMAGE_H
#define RADARIMAGE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>
#include <QMap>


class Weather;
class QUrlInfo;

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
    void updateImages();

    Weather *weather() const;
    void setWeather(Weather *xWeather);

signals:
    //void imageChanged();

private slots:
    void startTimer();
    void nextFrame();

private:
    void pauseTimer();

    QVector<QImage*> *mRadarImages;
    Weather *mWeather;
    QImage *mActiveFrame;
    QTimer *mTimer = nullptr;
    int mFramePosition = 0;
    int mPauseCount = 0;

};

#endif // RADARIMAGE_H
