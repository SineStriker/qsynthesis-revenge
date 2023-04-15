#include "CorePlugin.h"

#include <QApplication>
#include <QDirIterator>
#include <QSplashScreen>
#include <QThread>

#include "AddOn/HomeWindowAddOn.h"
#include "AddOn/ProjectWindowAddOn.h"

#include "Window/ICoreWindowFactory.h"

#include "QMDecoratorV2.h"
#include "QMSystem.h"
#include "QMView.h"

#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>

#include "CoreApi/ILoader.h"

#include "Internal/Settings/ActionConfigurePage.h"
#include "Internal/Settings/AppearanceTopPage.h"
#include "Internal/Settings/DisplayPage.h"

#include "Internal/Documents/DspxDocument.h"

namespace Core {

    namespace Internal {

        static ICore *icore = nullptr;

        static QSplashScreen *m_splash = nullptr;

        CorePlugin::CorePlugin() {
        }

        CorePlugin::~CorePlugin() {
        }

        bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(QMFs::PathFindDirPath(pluginSpec()->filePath()) +"/translations");
            qIDec->addThemePath(QMFs::PathFindDirPath(pluginSpec()->filePath()) +"/themes");

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing core plugin..."));
            }
            m_splash = splash;
            // QThread::msleep(2000);

            // Init ICore instance
            icore = new ICore(this);

            // Add basic actions
            auto actionMgr = icore->actionSystem();

            if (actionMgr->loadContexts(":/core_actions.xml").isEmpty()) {
                *errorMessage = tr("Failed to load action configuration!");
                return false;
            }

            // Add basic windows and add-ons
            auto winMgr = icore->windowSystem();

            winMgr->addWindow(new Internal::IHomeWindowFactory());
            winMgr->addWindow(new Internal::IProjectWindowFactory());

            winMgr->addAddOn(new HomeWindowAddOnFactory());
            winMgr->addAddOn(new ProjectWindowAddOnFactory());

            // Add setting panels
            auto sc = icore->settingCatalog();
            {
                auto appearance = new AppearanceTopPage();

                auto display = new DisplayPage();
                auto actionConfigure = new ActionConfigurePage();

                appearance->addPage(display);
                appearance->addPage(actionConfigure);

                sc->addPage(appearance);
            }

            // Add mime types
            auto docMgr = icore->documentSystem();

            docMgr->addDocType(new DspxSpec());

            return true;
        }

        void CorePlugin::extensionsInitialized() {
        }

        bool CorePlugin::delayedInitialize() {
            // PluginDialog dlg(nullptr);
            // dlg.exec();

            if (m_splash)
                m_splash->showMessage(tr("Initializing user interface..."));

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

            QMView::bringWindowToForeground(icore->windowSystem()->firstWindow()->window());

            return nullptr;
        }

        void CorePlugin::waitSplash(QWidget *w) {
            // Get splash screen handle

            if (m_splash)
                m_splash->finish(w);
        }

    }

}