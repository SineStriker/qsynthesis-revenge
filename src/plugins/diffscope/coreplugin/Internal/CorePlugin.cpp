#include "CorePlugin.h"

#include <QApplication>
#include <QDirIterator>
#include <QLoggingCategory>
#include <QSplashScreen>
#include <QThread>

#include <private/qguiapplication_p.h>
#include <private/qshortcutmap_p.h>

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

#include "Internal/Document/DspxSpec.h"

namespace Core {

    namespace Internal {

        static ICore *icore = nullptr;

        static QSplashScreen *m_splash = nullptr;

        static int openFileFromCommand(QString workingDir, const QStringList &args) {
            int cnt = 0;

            if (workingDir.isEmpty())
                workingDir = QDir::currentPath();

            for (const auto &arg : qAsConst(args)) {
                QFileInfo info(arg);
                if (info.isRelative()) {
                    info.setFile(workingDir + "/" + arg);
                }

                if (!info.isFile()) {
                    continue;
                }

                auto spec = icore->documentSystem()->supportedDocType(info.completeSuffix());
                if (!spec)
                    continue;

                if (spec->open(info.canonicalFilePath())) {
                    cnt++;
                }
            }

            return cnt;
        }

        CorePlugin::CorePlugin() {
        }

        CorePlugin::~CorePlugin() {
        }

        bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(pluginSpec()->location() + "/translations");
            qIDec->addThemePath(pluginSpec()->location() + "/themes");

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

            winMgr->addWindow(new IHomeWindowFactory());
            winMgr->addWindow(new IProjectWindowFactory());

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

            // Add document types
            auto docMgr = icore->documentSystem();

            auto dspxSpex = new DspxSpec();
            docMgr->addDocType(dspxSpex);
            docMgr->setPreferredDocTypeId("dspx", dspxSpex->id());

            // QLoggingCategory::setFilterRules("qt.gui.shortcutmap=true");
            qApp->setProperty("closeHomeOnOpen", true);
            qApp->setProperty("projectDocTypeId", "org.ChorusKit.dspx");

            return true;
        }

        void CorePlugin::extensionsInitialized() {
        }

        bool CorePlugin::delayedInitialize() {
            if (m_splash)
                m_splash->showMessage(tr("Initializing user interface..."));

            QTimer::singleShot(0, this, [this]() {
                auto winMgr = icore->windowSystem();
                auto handle = winMgr->createWindow("home");

                auto win = handle->window();
                waitSplash(win);

                // Check logs
                if (icore->documentSystem()->checkRemainingLogs(win) +
                        openFileFromCommand({}, qApp->arguments().mid(1)) >
                    0) {
                    if (qApp->property("closeHomeOnOpen").toBool())
                        win->close();
                }
            });

            return false;
        }

        QObject *CorePlugin::remoteCommand(const QStringList &options, const QString &workingDirectory,
                                           const QStringList &args) {
            int cnt = openFileFromCommand(workingDirectory, args);
            auto firstHandle = icore->windowSystem()->firstWindow();
            if (firstHandle && cnt == 0) {
                QMView::bringWindowToForeground(firstHandle->window());
            }

            return nullptr;
        }

        void CorePlugin::waitSplash(QWidget *w) {
            // Get splash screen handle

            if (m_splash)
                m_splash->finish(w);
        }

    }

}