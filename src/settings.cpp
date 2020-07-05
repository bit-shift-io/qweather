#include "settings.h"

Settings* Settings::m_pThis = nullptr;


Settings::~Settings()
{
    m_pThis = nullptr;
}

Settings *Settings::instance()
{
    if (m_pThis == nullptr) // avoid creation of new instances
        m_pThis = new Settings;
    return m_pThis;
}

QObject *Settings::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    // C++ and QML instance they are the same instance
    return Settings::instance();
}
