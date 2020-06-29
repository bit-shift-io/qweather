#ifndef RADARIMAGE_H
#define RADARIMAGE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>
#include <QMap>
#include <QNetworkReply>
#include "src/qftp/qftp.h"

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
    void setBackgroundImage();

    Weather *weather() const;
    void setWeather(Weather *xWeather);
    void updateRadar();
    void requestImages();

signals:
    void imageChanged();

private slots:
    void replyImageFinished(QNetworkReply *xNetworkReply);
    void startTimer();

    // ftp commands
    void ftpAddToList(const QUrlInfo &xUrlInfo);
    void ftpCommandFinished(int xCommandId, bool xError);

private:
    QImage* read(const QString xFileName);
    void write(const QString xFileName, QImage* xImage);

    QImage *mBackgroundImage = nullptr;
    QImage *mTopographyImage = nullptr;
    QImage *mLocationImage = nullptr;
    QVector<QString> *mFileList;
    QVector<QImage*> mAnimationImages;
    Weather *mWeather;
    QFtp *mFtp;
    QTimer *mTimer = nullptr;
    int mFramePosition = 0;
    int mPauseCount = 0;
};

#endif // RADARIMAGE_H
