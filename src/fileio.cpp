#include "fileio.h"

FileIO* FileIO::m_pThis = nullptr;

FileIO::FileIO(QObject *parent) : QObject(parent)
{

}

FileIO::~FileIO()
{

}

/** read text file **/
QString FileIO::readText(const QString &xFilePath)
{
    QString result;
    QFile file(xFilePath);
    if(!file.exists()) {
        qWarning() << "Does not exits: " << file;
        return result;
    }
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        result = stream.readAll();
    }

    return result;
}

void FileIO::writeText(const QString &xFilePath, const QString &xText)
{
    QFile file(xFilePath);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << xText;
    }
}

bool FileIO::fileExists(const QString &xFilePath) const
{
    bool result = false;
    QFile f;
    f.setFileName(xFilePath);
    if(f.exists())
    {
        result = true;
    }
    return result;
}

bool FileIO::removeFile(const QString &xFilePath) const
{
    QFile file(xFilePath);
    return file.remove();
}

QString FileIO::getAppConfigLocation()
{
    return QStandardPaths::locate(QStandardPaths::AppConfigLocation, QString(), QStandardPaths::LocateDirectory);
}

QString FileIO::getCacheLocation()
{
    return QStandardPaths::locate(QStandardPaths::CacheLocation, QString(), QStandardPaths::LocateDirectory);
}

bool FileIO::clearCache()
{
    QString cache_dir = getCacheLocation();
    QDirIterator it(cache_dir, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    qint64 time_difference = 86400; // 1 day in seconds
    qint64 current_time = QDateTime::currentDateTime().toSecsSinceEpoch() - time_difference;

    while (it.hasNext()) {
        QString file_path = it.next();
        QFileInfo f(file_path);
        //qint64 last_modified = f.lastModified().toSecsSinceEpoch();
        qint64 last_read = f.lastRead().toSecsSinceEpoch();

        if (last_read < current_time) {
            // time stamp older than 1 day
            removeFile(file_path);
        }
    }
    return true;
}


FileIO *FileIO::instance()
{
    if (m_pThis == nullptr) // avoid creation of new instances
        m_pThis = new FileIO;
    return m_pThis;
}

QObject *FileIO::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    // C++ and QML instance they are the same instance
    return FileIO::instance();
}
