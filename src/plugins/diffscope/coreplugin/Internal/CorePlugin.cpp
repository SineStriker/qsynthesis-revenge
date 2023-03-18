#include "CorePlugin.h"

#include <QApplication>
#include <QSplashScreen>

#include "Window/ICoreWindowFactory.h"
#include "Window/WindowSystem.h"

#include "AddOn/HomeWindowAddOn.h"
#include "AddOn/ProjectWindowAddOn.h"

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

            winMgr->addWindow(new Internal::IHomeWindowFactory());
            winMgr->addWindow(new Internal::IProjectWindowFactory());

            winMgr->addAddOn(new HomeWindowAddOnFactory());
            winMgr->addAddOn(new ProjectWindowAddOnFactory());

            return true;
        }

        void CorePlugin::extensionsInitialized() {
        }

        bool CorePlugin::delayedInitialize() {
            // PluginDialog dlg(nullptr);
            // dlg.exec();

            auto winMgr = icore->windowSystem();
            auto handle = winMgr->createWindow("home");
            waitSplash(handle->window());
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
            if (screen && screen->isVisible()) {
                screen->finish(w);
            }
        }

    }

}