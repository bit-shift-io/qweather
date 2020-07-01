#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>
#include <QtCore>

// some forward declarations
class QQmlEngine;
class QJSEngine;

// https://stackoverflow.com/questions/8894531/reading-a-line-from-a-txt-or-csv-file-in-qml-qt-quick

class FileIO : public QObject
{
    Q_OBJECT

public:
    static FileIO *instance();
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    explicit FileIO(QObject *parent = 0);
    ~FileIO();

    // invokable functions
    Q_INVOKABLE QString readText(const QString &xFilePath);
    Q_INVOKABLE void writeText(const QString &xFilePath, const QString &xText);
    Q_INVOKABLE bool fileExists(const QString &xFilePath) const;
    Q_INVOKABLE bool removeFile(const QString &xFilePath) const;
    Q_INVOKABLE QString getAppConfigLocation();
    Q_INVOKABLE QString getCacheLocation();
    Q_INVOKABLE bool clearCache();

//signals:

//public slots:

private:
    static FileIO* m_pThis;

};


#endif // FILEIO_H
