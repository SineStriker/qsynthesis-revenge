#include "CorePlugin.h"

#include <QApplication>
#include <QDirIterator>
#include <QFileOpenEvent>
#include <QLoggingCategory>
#include <QSplashScreen>
#include <QThread>

#include <private/qguiapplication_p.h>
#include <private/qshortcutmap_p.h>

#include "QMDecoratorV2.h"
#include "QMSystem.h"
#include "QMView.h"

#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>

#include "CoreApi/ILoader.h"

#include "AddOn/HomeWindowAddOn.h"
#include "AddOn/ProjectWindowAddOn.h"
#include "Document/DspxSpec.h"
#include "Window/IHomeWindow.h"

#include "Internal/Settings/ActionConfigurePage.h"
#include "Internal/Settings/AppearanceTopPage.h"
#include "Internal/Settings/DisplayPage.h"
#include "Internal/Utils/LastWindowFilter.h"

namespace Core {

    extern void initDspxEntitiesSchema();

    namespace Internal {

        static ICore *icore = nullptr;

        static InitialRoutine *m_rout = nullptr;

        static int openFileFromCommand(QString workingDir, const QStringList &args, IWindow *iWin) {
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

                if (spec->open(info.canonicalFilePath(), iWin ? iWin->window() : nullptr)) {
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

            auto rout =
                qobject_cast<Core::InitialRoutine *>(ILoader::instance()->getFirstObject("choruskit_init_routine"));
            if (rout) {
                rout->splash()->showMessage(tr("Initializing core plugin..."));
            }
            m_rout = rout;
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

            winMgr->addAddOn("home", &HomeWindowAddOn::staticMetaObject);
            winMgr->addAddOn("project", &ProjectWindowAddOn::staticMetaObject);

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

            // Register basic items
            initDspxEntitiesSchema();

            // Add document types
            auto docMgr = icore->documentSystem();

            auto dspxSpex = new DspxSpec();
            docMgr->addDocType(dspxSpex);
            docMgr->setPreferredDocTypeId("dspx", dspxSpex->id());

            // QLoggingCategory::setFilterRules("qt.gui.shortcutmap=true");
            qApp->setProperty("closeHomeOnOpen", true);
            qApp->setProperty("projectDocTypeId", "org.ChorusKit.dspx");

            qApp->setWindowIcon(QIcon(":/svg/app/diffsinger.svg"));

            // Handle FileOpenEvent
            qApp->installEventFilter(this);

            return true;
        }

        void CorePlugin::extensionsInitialized() {
        }

        bool CorePlugin::delayedInitialize() {
            if (m_rout)
                m_rout->splash()->showMessage(tr("Initializing user interface..."));

            QTimer::singleShot(0, this, [this]() {
                auto winMgr = icore->windowSystem();

                LastWindowFilter g;

                if (m_rout) {
                    for (const auto &func : qAsConst(m_rout->routines())) {
                        func();
                    }
                }

                // Open files
                openFileFromCommand({}, ExtensionSystem::PluginManager::arguments(), nullptr);

                // If no window is created, create a default home window
                if (winMgr->count() == 0) {
                    auto home = IWindow::create<IHomeWindow>();
                    waitSplash(home->window());
                } else {
                    waitSplash(winMgr->firstWindow()->window());
                }
            });

            return false;
        }

        QObject *CorePlugin::remoteCommand(const QStringList &options, const QString &workingDirectory,
                                           const QStringList &args) {
            auto firstHandle = icore->windowSystem()->firstWindow();
            int cnt = openFileFromCommand(workingDirectory, args, firstHandle);
            if (firstHandle && cnt == 0) {
                QMView::bringWindowToForeground(firstHandle->window());
            }
            return nullptr;
        }

        bool CorePlugin::eventFilter(QObject *obj, QEvent *event) {
            if (event->type() == QEvent::FileOpen) {
                openFileFromCommand({}, {static_cast<QFileOpenEvent *>(event)->file()},
                                    icore->windowSystem()->firstWindow());
            }
            return QObject::eventFilter(obj, event);
        }

        void CorePlugin::waitSplash(QWidget *w) {
            // Get splash screen handle
            if (m_rout) {
                m_rout->splash()->finish(w);
                emit m_rout->done();
            }
        }

        // This scope is only to expose strings to Qt translation tool
        static void _qt_translation_CommandCategory() {
            QApplication::translate("Core::CommandCategory", "Create");
            QApplication::translate("Core::CommandCategory", "File");
            QApplication::translate("Core::CommandCategory", "Preferences");
            QApplication::translate("Core::CommandCategory", "Edit");
            QApplication::translate("Core::CommandCategory", "View");
            QApplication::translate("Core::CommandCategory", "Playback");
            QApplication::translate("Core::CommandCategory", "Help");
            QApplication::translate("Core::CommandCategory", "About");
        }

    } // namespace Internal

} // namespace Core
