#include "ScriptMgrAddOn.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QMessageBox>

#include <CoreApi/IWindow.h>
#include <coreplugin/ICore.h>
#include <QMDecoratorV2.h>

#include "JsInternalObject.h"
#include "ScriptLoader.h"

namespace ScriptMgr {

    using namespace Core;

    namespace Internal {

        bool ScriptMgrAddOnFactory::predicate(Core::IWindow *handle) const {
            return handle->id() == "project";
        }

        Core::IWindowAddOn *ScriptMgrAddOnFactory::create(QObject *parent) {
            return new ScriptMgrAddOn();
        }

        ScriptMgrAddOn::ScriptMgrAddOn(QObject *parent) : IWindowAddOn(parent) {

        }

        ScriptMgrAddOn::~ScriptMgrAddOn() {
            ScriptLoader::instance()->removeHandles(windowKey);
        }

        void ScriptMgrAddOn::initialize() {
            initializeActions();
            connect(ScriptLoader::instance(), &ScriptLoader::engineWillReload, this, [=](){
                for(auto action: scriptActions) {
                    batchProcessMainMenu->menu()->removeAction(action);
                    if(action->menu()) {
                        windowHandle()->removeShortcutContext(action->menu());
                        action->menu()->deleteLater();
                    }
                    action->deleteLater();
                }
                scriptActions.clear();
            });
            connect(ScriptLoader::instance(), &ScriptLoader::engineReloaded, this, &ScriptMgrAddOn::reloadActions);
            qIDec->installLocale(this, _LOC(ScriptMgrAddOn, this));
        }

        void ScriptMgrAddOn::extensionsInitialized() {
            reloadActions();
        }

        void ScriptMgrAddOn::reloadStrings() {
            batchProcessMainGroup->setText(tr("Batch Process Actions"));
            batchProcessMainMenu->setText(tr("&Batch Process"));
            scriptOperationsGroup->setText(tr("Script Operation Actions"));
            reloadScriptsAction->setText(tr("Reload Scripts"));
            scriptSettingsAction->setText(tr("Script Settings"));
            emit handleJsReloadStrings();
        }

        void ScriptMgrAddOn::initializeActions() {
            auto iWin = windowHandle();
            auto win = iWin->window();
            batchProcessMainGroup = new ActionItem("scriptmgr.BatchProcessGroup", new QActionGroup(this), this);
            batchProcessMainMenu = new ActionItem("scriptmgr.BatchProcessMenu", ICore::createCoreMenu(win), this);
            scriptOperationsGroup = new ActionItem("scriptmgr.ScriptOperations", new QActionGroup(this), this);
            reloadScriptsAction = new ActionItem("scriptmgr.ReloadScripts", new QAction, this);
            scriptSettingsAction = new ActionItem("scriptmgr.ScriptSettings", new QAction, this);
            connect(reloadScriptsAction->action(), &QAction::triggered, ScriptLoader::instance(), &ScriptLoader::reloadEngine);
            connect(scriptSettingsAction->action(), &QAction::triggered, this, [=](){
                ICore::showSettingsDialog("scriptmgr.Script", windowHandle()->window());
            });
            iWin->addActionItems({
                batchProcessMainGroup,
                batchProcessMainMenu,
                scriptOperationsGroup,
                reloadScriptsAction,
                scriptSettingsAction,
            });
        }

        void ScriptMgrAddOn::reloadActions() {
            internalObject = new JsInternalObject(this);
            windowKey = ScriptLoader::instance()->createHandles(internalObject);
            createScriptActions(ScriptLoader::instance()->builtInScriptEntries());
            createScriptActions(ScriptLoader::instance()->scriptEntries());
            emit handleJsReloadStrings();
        }

        void ScriptMgrAddOn::createScriptActions(const QList<ScriptEntry> &entries) {
            scriptActions.append(batchProcessMainMenu->menu()->addSeparator());
            for(const auto &entry: entries) {
                auto mainAction = new QAction;
                connect(this, &ScriptMgrAddOn::handleJsReloadStrings, mainAction, [=](){
                    mainAction->setText(ScriptLoader::instance()->getName(entry.id));
                });
                if(entry.type == ScriptEntry::Script) {
                    if(!entry.shortcut.isEmpty()) mainAction->setShortcut(QKeySequence(entry.shortcut));
                    connect(mainAction, &QAction::triggered, this, [=](){
                        ScriptLoader::instance()->invoke(windowKey, entry.id);
                    });
                } else {
                    auto menu = ICore::createCoreMenu(windowHandle()->window());
                    for(int i = 0; i < entry.childrenId.size(); i++) {
                        auto childId = entry.childrenId[i];
                        auto childShortcut = entry.childrenShortcut[i];
                        auto childAction = new QAction;
                        connect(this, &ScriptMgrAddOn::handleJsReloadStrings, childAction, [=](){
                            childAction->setText(ScriptLoader::instance()->getName(entry.id, i));
                        });
                        if(!childShortcut.isEmpty()) childAction->setShortcut(QKeySequence(childShortcut));
                        connect(childAction, &QAction::triggered, this, [=](){
                            ScriptLoader::instance()->invoke(windowKey, entry.id, i);
                        });
                        menu->addAction(childAction);
                    }
                    mainAction->setMenu(menu);
                    windowHandle()->addShortcutContext(menu);
                }
                batchProcessMainMenu->menu()->addAction(mainAction);
                scriptActions.append(mainAction);
            }
        }
    }
}