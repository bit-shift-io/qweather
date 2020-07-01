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
    QString dir = QStandardPaths::locate(QStandardPaths::AppConfigLocation, QString(), QStandardPaths::LocateDirectory);
    return dir;
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
