#include "CorePlugin.h"

#include <QApplication>
#include <QMainWindow>
#include <QSplashScreen>

#include "Windows/PlainWindow.h"
#include "plugindialog.h"

#include <extensionsystem/pluginmanager.h>

namespace Core {

    namespace Internal {

        CorePlugin::CorePlugin() {
            icore = nullptr;
        }

        CorePlugin::~CorePlugin() {
        }

        bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            // Init ICore instance
            icore = new ICore(this);

            return true;
        }

        void CorePlugin::extensionsInitialized() {
        }

        bool CorePlugin::delayedInitialize() {
            PluginDialog dlg(nullptr);
            dlg.exec();
            return true;
        }

        QObject *CorePlugin::remoteCommand(const QStringList &options, const QString &workingDirectory,
                                           const QStringList &args) {
            qDebug() << options;
            qDebug() << workingDirectory;
            qDebug() << args;
            return nullptr;
        }

        void CorePlugin::waitSplash(QWidget *w) {
            // Get splash screen handle
            auto var = qApp->property("__choruskit_init_splash__");
            auto screen = var.value<QSplashScreen *>();
            if (screen) {
                screen->finish(w);
            }
        }

    }

}