#include "CorePlugin.h"

#include <QApplication>
#include <QSplashScreen>

#include "Window/WindowSystem.h"

#include "Window/ICoreWindow.h"
#include "Window/ICoreWindowAddOn.h"

#include <extensionsystem/pluginmanager.h>

namespace Core {

    namespace Internal {

        static ICore *icore = nullptr;

        CorePlugin::CorePlugin() {
        }

        CorePlugin::~CorePlugin() {
        }

        bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            // Init ICore instance
            icore = new ICore(this);

            // Add basic windows and add-ons
            auto winMgr = icore->windowSystem();
            winMgr->addWindow(new ICoreWindowFactory("home"));
            winMgr->addWindow(new ICoreWindowFactory("project"));
            winMgr->addAddOn(new ICoreWindowAddOnFactory());

            return true;
        }

        void CorePlugin::extensionsInitialized() {
        }

        bool CorePlugin::delayedInitialize() {
            // PluginDialog dlg(nullptr);
            // dlg.exec();

            auto winMgr = icore->windowSystem();
            winMgr->createWindow("home");
            winMgr->createWindow("project");

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