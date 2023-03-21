#include "CorePlugin.h"

#include <QApplication>
#include <QSplashScreen>

#include "Window/ICoreWindowFactory.h"
#include "Window/WindowSystem.h"

#include "AddOn/HomeWindowAddOn.h"
#include "AddOn/ProjectWindowAddOn.h"

#include "QMDecorateDir.h"
#include "QMDecorator.h"
#include "QMSystem.h"

#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>

namespace Core {

    namespace Internal {

        class CorePluginPrivate {
        public:
            QMDecorateDir dec;
        };

        static ICore *icore = nullptr;

        static CorePluginPrivate *d = nullptr;

        CorePlugin::CorePlugin() {
        }

        CorePlugin::~CorePlugin() {
        }

        bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            auto &d = Internal::d;
            d = new CorePluginPrivate();

            // Init ICore instance
            icore = new ICore(this);

            // Add basic windows and add-ons
            auto winMgr = icore->windowSystem();

            winMgr->addWindow(new Internal::IHomeWindowFactory());
            winMgr->addWindow(new Internal::IProjectWindowFactory());

            winMgr->addAddOn(new HomeWindowAddOnFactory());
            winMgr->addAddOn(new ProjectWindowAddOnFactory());

            qIDec->addThemeTemplate("Global", ":/themes/global.qss.in");
            qIDec->addThemeTemplate("HomeWindow", ":/themes/home.qss.in");
            qIDec->addThemeTemplate("PianoWindow", ":/themes/piano.qss.in");

            QString path = QString("%1/share/%2.res.json").arg(QMFs::PathFindDirPath(pluginSpec()->filePath()), "Core");
            d->dec.load(path);

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