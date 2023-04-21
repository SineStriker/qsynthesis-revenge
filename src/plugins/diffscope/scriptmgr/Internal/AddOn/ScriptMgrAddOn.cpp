#include "ScriptMgrAddOn.h"
#include "JsInternalObject.h"
#include "ScriptLoader.h"

#include <QDebug>

#include <CoreApi/IWindow.h>
#include <coreplugin/ICore.h>
#include <QDir>
#include <QFile>
#include <QMDecoratorV2.h>
#include <QMessageBox>

namespace ScriptMgr {

    using namespace Core;

    namespace Internal {

        bool ScriptMgrAddOnFactory::predicate(Core::IWindow *handle) const {
            return handle->id() == "project" || handle->id() == "home"; // TODO temporarily add to home
        }

        Core::IWindowAddOn *ScriptMgrAddOnFactory::create(QObject *parent) {
            return new ScriptMgrAddOn();
        }

        ScriptMgrAddOn::ScriptMgrAddOn(QObject *parent) : IWindowAddOn(parent) {
        }

        ScriptMgrAddOn::~ScriptMgrAddOn() {
        }

        void ScriptMgrAddOn::initialize() {
            initializeActions();
            if(!initializeEngine()) {
                criticalCannotInitializeEngine();
                return;
            }
            qIDec->installLocale(this, _LOC(ScriptMgrAddOn, this));
            ScriptLoader::instance()->registerAddon(this);
        }

        void ScriptMgrAddOn::extensionsInitialized() {
        }

        void ScriptMgrAddOn::reloadStrings() {
            batchProcessMainGroup->setText(tr("Batch Process Actions"));
            batchProcessMainMenu->setText(tr("&Batch Process"));
            builtInScriptsMainGroup->setText(tr("Built-in Script Actions"));
            userScriptsMainGroup->setText(tr("User Script Actions"));
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
            builtInScriptsMainGroup = new ActionItem("scriptmgr.BuiltInScripts", new QActionGroup(this), this);
            userScriptsMainGroup = new ActionItem("scriptmgr.UserScripts", new QActionGroup(this), this);
            scriptOperationsGroup = new ActionItem("scriptmgr.ScriptOperations", new QActionGroup(this), this);
            reloadScriptsAction = new ActionItem("scriptmgr.ReloadScripts", new QAction, this);
            scriptSettingsAction = new ActionItem("scriptmgr.ScriptSettings", new QAction, this);
            connect(reloadScriptsAction->action(), &QAction::triggered, ScriptLoader::instance(), &ScriptLoader::reloadScripts);
            connect(scriptSettingsAction->action(), &QAction::triggered, this, [=](){
                ICore::showSettingsDialog("core.Settings", windowHandle()->window()); //TODO script settings
            });
            iWin->addActionItems({
                batchProcessMainGroup,
                batchProcessMainMenu,
                builtInScriptsMainGroup,
                userScriptsMainGroup,
                scriptOperationsGroup,
                reloadScriptsAction,
                scriptSettingsAction,
            });
        }

        bool ScriptMgrAddOn::initializeEngine() {
            engine.globalObject().setProperty("_internal", engine.newQObject(new JsInternalObject(&engine, this)));
            engine.globalObject().setProperty("_q_tr_ext", engine.newObject());
            engine.installExtensions(QJSEngine::TranslationExtension, engine.globalObject().property("_q_tr_ext"));
            if(!loadScriptFile(":/scripts/internal.js")) {
                return false;
            }
            loadScripts();
            return true;
        }

        void ScriptMgrAddOn::loadScripts() {
            if(!builtInScriptInitialized) {
                QDir dir(":/scripts/");
                for(const auto& filename: dir.entryList(QDir::Files)) {
                    if(filename == "internal.js") continue;
                    if(!loadScriptFile(":/scripts/" + filename)){
                        warningCannotLoadFile(":/scripts/" + filename);
                    }
                }
                builtInScriptInitialized = true;
            }

            //clean-ups
            for(const auto& id: userScriptIdRegistry) {
                engine.globalObject().property("_registry").property("delete").call({id});
            }
            userScriptIdRegistry.clear();
            for(auto action: userScriptsMainGroup->actionGroup()->actions()) {
                userScriptsMainGroup->actionGroup()->removeAction(action);
                action->deleteLater();
            }

            //TODO load user scripts
        }

        void ScriptMgrAddOn::registerScript(const QString &id, int role, const QString &shortcut) {
            if(builtInScriptInitialized) {
                auto action = new QAction;
                userScriptsMainGroup->actionGroup()->addAction(action);
                connect(this, &ScriptMgrAddOn::handleJsReloadStrings, this, [=](){
                    action->setText(getName(id));
                });
                connect(action, &QAction::triggered, this, [=](){
                    invoke(id);
                });
                userScriptIdRegistry.append(id);
            } else {
                auto actionSpec = new ActionSpec("scriptmgr." + id);
                actionSpec->setShortcuts({shortcut});
                ICore::instance()->actionSystem()->addAction(actionSpec);
                auto actionItem = new ActionItem("scriptmgr." + id, new QAction, this);
                connect(this, &ScriptMgrAddOn::handleJsReloadStrings, this, [=](){
                    actionItem->setText(getName(id));
                });
                connect(actionItem->action(), &QAction::triggered, this, [=](){
                    invoke(id);
                });
                auto ctxActionItem = ICore::instance()->actionSystem()->context("home.MainMenu")->addAction("scriptmgr." + id, false); //TODO change to project
                ctxActionItem.setRules({ActionInsertRule("scriptmgr.BuiltInScripts", ActionInsertRule::Append)});
                windowHandle()->addActionItem(actionItem);
            }
        }

        void ScriptMgrAddOn::registerScript(const QString &id, const QStringList &children, int role,
                                            const QStringList &childrenShortcuts) {
            if(builtInScriptInitialized) {
                auto menu = ICore::createCoreMenu(windowHandle()->window());
                auto action = new QAction;
                action->setMenu(menu);
                connect(this, &ScriptMgrAddOn::handleJsReloadStrings, this, [=](){
                    action->setText(getName(id));
                });
                userScriptsMainGroup->actionGroup()->addAction(action);
                for(int i = 0; i < children.size(); i++) {
                    auto childAction = new QAction;
                    connect(this, &ScriptMgrAddOn::handleJsReloadStrings, this, [=](){
                        childAction->setText(getName(id, i));
                    });
                    connect(childAction, &QAction::triggered, this, [=](){
                        invoke(id, i);
                    });
                    menu->addAction(childAction);
                }
                userScriptIdRegistry.append(id);
            } else {
                ICore::instance()->actionSystem()->addAction(new ActionSpec("scriptmgr." + id));
                auto actionItem = new ActionItem("scriptmgr." + id, ICore::createCoreMenu(windowHandle()->window()), this);
                connect(this, &ScriptMgrAddOn::handleJsReloadStrings, this, [=](){
                    actionItem->setText(getName(id));
                });
                auto ctxActionItem = ICore::instance()->actionSystem()->context("home.MainMenu")->addAction("scriptmgr." + id, true); //TODO change to project
                ctxActionItem.setRules({ActionInsertRule("scriptmgr.BuiltInScripts", ActionInsertRule::Append)});
                windowHandle()->addActionItem(actionItem);
                for(int i = 0; i < children.length(); i++) {
                    const auto childId = "scriptmgr." + id + "." + children[i];
                    auto childActionSpec = new ActionSpec(childId);
                    childActionSpec->setShortcuts({childrenShortcuts[i]});
                    ICore::instance()->actionSystem()->addAction(childActionSpec);
                    auto childActionItem = new ActionItem(childId, new QAction, this);
                    connect(this, &ScriptMgrAddOn::handleJsReloadStrings, this, [=](){
                        childActionItem->setText(getName(id, i));
                    });
                    connect(childActionItem->action(), &QAction::triggered, this, [=](){
                        invoke(id, i);
                    });
                    auto childCtxActionItem = ICore::instance()->actionSystem()->context("home.MainMenu")->addAction(childId, false); //TODO change to project
                    childCtxActionItem.setRules({ActionInsertRule("scriptmgr." + id, ActionInsertRule::Append)});
                    windowHandle()->addActionItem(childActionItem);
                }
            }
        }

        QString ScriptMgrAddOn::getName(const QString &id) const {
            auto ret = engine.globalObject().property("_getName").call({id});
            if(ret.isString()) {
                return ret.toString();
            } else {
                if(ret.isError()) {
                    alertJsUncaughtError(ret);
                }
                warningCannotGetName(id);
                return id;
            }
        }

        QString ScriptMgrAddOn::getName(const QString &id, int index) const {
            auto ret = engine.globalObject().property("_getName").call({id, index});
            if(ret.isString()) {
                return ret.toString();
            } else {
                if(ret.isError()) {
                    alertJsUncaughtError(ret);
                }
                warningCannotGetName(id);
                return id; //TODO child id
            }
        }

        void ScriptMgrAddOn::invoke(const QString &id) const {
            auto ret = engine.globalObject().property("_invoke").call({id});
            if(ret.isError()) {
                alertJsUncaughtError(ret);
                criticalScriptExecutionFailed(id);
            }
        }

        void ScriptMgrAddOn::invoke(const QString &id, int index) const {
            auto ret = engine.globalObject().property("_invoke").call({id, index});
            if(ret.isError()) {
                alertJsUncaughtError(ret);
                criticalScriptExecutionFailed(id, index);
            }
        }

        bool ScriptMgrAddOn::loadScriptFile(const QString &path) {
            QFile scriptFile(path);
            if(!scriptFile.open(QIODevice::ReadOnly)) {
                alertJsUncaughtError(engine.evaluate(QString("new Error(`Cannot open file '%1'.`)").arg(path)));
                return false;
            }
            QTextStream stream(&scriptFile);
            stream.setCodec("UTF-8");
            auto ret =  engine.evaluate(scriptFile.readAll(), path);
            if(ret.isError()) {
                alertJsUncaughtError(ret);
                return false;
            }
            return true;
        }

        void ScriptMgrAddOn::alertJsUncaughtError(const QJSValue& error) const {
            QString msg = tr("Uncaught error.") + "\n\n" + QString("%1:%2").arg(error.property("fileName").toString(), error.property("lineNumber").toString()) + "\n\n" + error.toString() + "\n\n" + error.property("stack").toString();
            QMessageBox::warning(windowHandle()->window(), tr("JavaScript Error"), msg);
        }

        void ScriptMgrAddOn::warningCannotLoadFile(const QString &path) const {
            QMessageBox::warning(windowHandle()->window(), tr("Warning"), tr("Cannot load script file '%1'. The file is ignored.").arg(path));
        }

        void ScriptMgrAddOn::warningCannotGetName(const QString &id) const {
            QMessageBox::warning(windowHandle()->window(), tr("Warning"), tr("Cannot get name of script '%1'. Its name will be displayed as its id."));
        }

        void ScriptMgrAddOn::criticalCannotInitializeEngine() const {
            QMessageBox::critical(windowHandle()->window(), tr("Error"), tr("Cannot initialize JavaScript engine."));
        }

        void ScriptMgrAddOn::criticalScriptExecutionFailed(const QString &id) const {
            QMessageBox::critical(windowHandle()->window(), tr("Error"), tr("Script '%1' execution failed.").arg(getName(id)));
        }

        void ScriptMgrAddOn::criticalScriptExecutionFailed(const QString &id, int index) const {
            QMessageBox::critical(windowHandle()->window(), tr("Error"), tr("Script '%1' execution failed.").arg(getName(id, index)));
        }
    }
}