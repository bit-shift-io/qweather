#include "places.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QDebug>
#include <QQmlEngine>

Places* Places::m_pThis = nullptr;

Places::Places(QObject *parent) : QObject(parent)
{
    mNetworkAccessManager = new QNetworkAccessManager(this);
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &Places::replyFinished);
    update();
}

Places::~Places()
{
}

Places *Places::instance()
{
    if (m_pThis == nullptr) // avoid creation of new instances
        m_pThis = new Places;
    return m_pThis;
}

QObject *Places::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    // C++ and QML instance they are the same instance
    return Places::instance();
}

void Places::update()
{
    qDebug() << "places!";
    update_count = STATE_URL.size();
    updateNext();
}

void Places::updateNext() {
    if (!update_count)
        return;

    update_count--;
    QNetworkRequest request;
    QUrl u = QUrl(STATE_URL.values().at(update_count));
    request.setUrl(u);
    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    mNetworkAccessManager->get(request);
}

void Places::replyFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() == QNetworkReply::NoError)
    {
        QByteArray data = xNetworkReply->readAll();
        qDebug() << "HTML size:" << data.size() << "bytes";
        //qDebug() << data;

        QXmlStreamReader xml(data);

        QJsonObject result;
        QJsonObject cur_station {};

        while(!xml.atEnd())
        {
            if (xml.readNextStartElement()) {

                if(xml.name()=="a" && !xml.isEndElement())
                {
                    // station is in a table
                    // tr = station row
                    // th - a is the link in the table

                    if (!cur_station.isEmpty()) {
                        // append previous station
                        result.insert(cur_station.value("id").toString(), cur_station);
                    }

                    QString href = xml.attributes().value("href").toString();
                    //QString name = xml.readElementText();
                    qDebug() << href << " " << href;

                    // new station json
                    /*
                    cur_station = {
                        {"id", wmo_id},
                        {"name", name}
                    };*/

                }

            }
        }

        updateNext();
        //emit resultFinished(result);
    } else {
        qDebug()<<"error";
    }

    //delete xNetworkReply; // do we need to clean? this crashes
}
