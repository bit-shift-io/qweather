#include "fileio.h"

FileIO* FileIO::m_pThis = nullptr;

FileIO::FileIO(QObject *parent) : QObject(parent)
{

}

FileIO::~FileIO()
{

}

/** read text file **/
QString FileIO::readText(const QString &t_filePath)
{
    QString result;
    QFile file(t_filePath);
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

void FileIO::writeText(const QString &t_filePath, const QString &text)
{
    QFile file(t_filePath);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << text;
    }
}

bool FileIO::fileExists(const QString &t_filePath) const
{
    bool result = false;
    QFile f;
    f.setFileName(t_filePath);
    if(f.exists())
    {
        result = true;
    }
    return result;
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
