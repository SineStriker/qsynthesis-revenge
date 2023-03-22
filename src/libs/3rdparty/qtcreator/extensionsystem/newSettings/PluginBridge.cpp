#include "PluginBridge.h"

namespace ExtensionSystem {

    struct PluginBridgePrivate {
        QSplashScreen *splash;
        QString text;

        PluginBridgePrivate() {
            splash = nullptr;
        }
    };

    PluginBridge *m_instance = nullptr;

    PluginBridgePrivate *d = nullptr;

    PluginBridge::PluginBridge() {
        m_instance = this;
        d = new PluginBridgePrivate();
    }

    PluginBridge::~PluginBridge() {
        delete d;
        m_instance = nullptr;
    }

    QSplashScreen *PluginBridge::splash() {
        return d ? d->splash : nullptr;
    }

    void PluginBridge::setSplash(QSplashScreen *splash) {
        if (!d)
            return;
        d->splash = splash;
    }

    QString PluginBridge::loaderText() {
        return d ? d->text : QString();
    }

    void PluginBridge::setLoaderText(const QString &text) {
        if (!d)
            return;
        d->text = text;
        m_instance->updateLoaderText(text);
    }

    PluginBridge *PluginBridge::instance() {
        return m_instance;
    }

    void PluginBridge::updateLoaderText(const QString &text) {
        qDebug() << "ExtensionSystem::PluginBridge::setLoaderText():" << text;
    }


}