//
// Created by Functioner on 2023/3/22.
//

#ifndef CHORUSKIT_PLUGINBRIDGE_H
#define CHORUSKIT_PLUGINBRIDGE_H

#include <QSplashScreen>

#include <extensionsystem/extensionsystem_global.h>

class QSplashScreen;

namespace ExtensionSystem {

    class EXTENSIONSYSTEM_EXPORT PluginBridge {
    public:
        PluginBridge();
        ~PluginBridge();

        static QSplashScreen *splash();
        static void setSplash(QSplashScreen *splash);

        static QString loaderText();
        static void setLoaderText(const QString &text);

    public:
        static PluginBridge *instance();

    protected:
        virtual void updateLoaderText(const QString &text);
    };

}



#endif // CHORUSKIT_PLUGINBRIDGE_H
