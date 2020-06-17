#ifndef RADARIMAGE_H
#define RADARIMAGE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>
#include <QVector>
#include <QNetworkReply>


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
    void setImage(const QImage &xImage);

    Weather *weather() const;
    void setWeather(Weather *xWeather);
    void updateRadar();
    void replyRadarFinished(QNetworkReply *xNetworkReply);

signals:
    void imageChanged();

private slots:
    void replyImageFinished(QNetworkReply *xNetworkReply);

    // ftp commands
    void ftpAddToList(const QUrlInfo &xUrlInfo);
    void ftpCommandFinished(int xCommandId, bool xError);

private:
    QImage mImage;
    QImage mBackgroundImage;
    QImage mTopographyImage;
    QImage mLocationImage;
    QVector<QImage> mAnimationImages;
    Weather *mWeather;
};

#endif // RADARIMAGE_H
