#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QtCore>
#include <QSettings>

class QQmlEngine;
class QJSEngine;

class Settings : public QSettings
{
    Q_OBJECT

public:
    /*
    explicit Settings(QObject *parent = 0) : QSettings(QSettings::IniFormat,
    QSettings::UserScope,
    QCoreApplication::instance()->organizationName(),
    QCoreApplication::instance()->applicationName(),
    parent) {}*/

    explicit Settings(QObject *parent = 0) : QSettings(QSettings::UserScope,
        QCoreApplication::instance()->organizationName(),
        QCoreApplication::instance()->applicationName(),
        parent) {}

    static Settings *instance();
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE inline void setValue(const QString &key, const QVariant &value) { QSettings::setValue(key, value); }
    Q_INVOKABLE inline QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const { QSettings::value(key, defaultValue); return QSettings::value(key, defaultValue); }

private:
    static Settings* m_pThis;

};


#endif // SETTINGS_H
