#include <QPainter>
#include <QNetworkReply>

#include "src/qftp/qftp.h"
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
        connect(mWeather, &Weather::updateRadar, this, &RadarImage::updateRadar);
    }
}


void RadarImage::updateRadar()
{
    qDebug() << "radar cpp";

    QString background_url = QString("http://www.bom.gov.au/products/radar_transparencies/%1.background.png").arg(mWeather->getRadarId());

    QNetworkAccessManager *net = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(background_url);
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );

    // async method
    connect(net, &QNetworkAccessManager::finished, this, &RadarImage::replyImageFinished);
    net->get(request);

    // ftp test
    QFtp *ftp = new QFtp(this);

    connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(ftpAddToList(QUrlInfo)));
    connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCommandFinished(int, bool)));

    // TODO: fix qftp
    //string[0] is depreciated
    ftp->connectToHost("ftp.bom.gov.au");
    ftp->login();
    ftp->cd("/anon/gen/radar/");
    ftp->list(); // this is async
    //ftp->get();
    //ftp->close();
    //ftp->deleteLater();

    /*
    // synchronous method!
    QNetworkReply* reply = net->get(request);
    QEventLoop eventLoop;
    connect(net,SIGNAL(finished()),&eventLoop,SLOT(quit()));
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        QImageReader imageReader(reply);
        imageReader.setAutoDetectImageFormat (false);
        QImage pic = imageReader.read();
        emit resultRadarFinished(pic);
        //ui->label_2->setPixmap(QPixmap::fromImage(pic));

     }
     */


}


void RadarImage::replyImageFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() != QNetworkReply::NoError) {
        qDebug() << "network error " << xNetworkReply->error() << " " << xNetworkReply->url().toString();
        xNetworkReply->deleteLater();
        return;
    }

    QString url = xNetworkReply->url().toString();
    QByteArray data = xNetworkReply->readAll();

    xNetworkReply->deleteLater();
    mBackgroundImage.loadFromData(data);
    setImage(mBackgroundImage);
    return;
}

void RadarImage::ftpAddToList(const QUrlInfo &xUrlInfo)
{
    QString radar_id = mWeather->getRadarId();
    if (xUrlInfo.name().startsWith(radar_id) && xUrlInfo.name().endsWith(".png")) {
        qDebug() << xUrlInfo.name() << " - " << xUrlInfo.size() << " bytes";
    }
}

void RadarImage::ftpCommandFinished(int commandId, bool error)
{
    qDebug() << "ftp completed";
    /*
    if (ftp->currentCommand() == QFtp::ConnectToHost) {
        if (error) {
            QMessageBox::information(this, tr("FTP"),
                                     tr("Unable to connect to the FTP server "
                                        "at %1. Please check that the host "
                                        "name is correct.")
                                     .arg(ftpServerLineEdit->text()));
            connectOrDisconnect();
            return;
        }
        statusLabel->setText(tr("Logged onto %1.")
                             .arg(ftpServerLineEdit->text()));
        fileList->setFocus();
        downloadButton->setDefault(true);
        connectButton->setEnabled(true);
        return;
    }*/
}
