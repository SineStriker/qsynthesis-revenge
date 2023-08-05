#include "ScriptMgrAddOn.h"

#include <QDebug>

#include <CoreApi/IWindow.h>
#include <coreplugin/ICore.h>
#include <QMDecoratorV2.h>

#include "Internal/Objects/JsInternalObject.h"
#include "Internal/Settings/ScriptSettingsConfigurableDialog.h"
#include "QMSvg.h"
#include "QsFrameworkNamespace.h"
#include "ScriptLoader.h"
#include "Window/IProjectWindow.h"

namespace ScriptMgr {

    using namespace Core;

    namespace Internal {

        ScriptMgrAddOn::ScriptMgrAddOn(QObject *parent) : IWindowAddOn(parent) {

        }

        ScriptMgrAddOn::~ScriptMgrAddOn() {
            ScriptLoader::instance()->removeHandles(windowKey);
        }

        void ScriptMgrAddOn::initialize() {
            initializeActions();
            internalObject = new JsInternalObject(this);
            connect(ScriptLoader::instance(), &ScriptLoader::engineWillReload, this, &ScriptMgrAddOn::cleanupScriptActions);
            connect(ScriptLoader::instance(), &ScriptLoader::engineReloaded, this, &ScriptMgrAddOn::reloadActions);
            qIDec->installLocale(this, _LOC(ScriptMgrAddOn, this));
        }

        void ScriptMgrAddOn::extensionsInitialized() {
            reloadActions();
        }

        void ScriptMgrAddOn::reloadStrings() {
            batchProcessMainGroup->setText(tr("Batch Process Actions"));
            batchProcessMainMenu->setText(tr("&Batch Process"));
            reloadScriptsAction->setText(tr("Reload Scripts"));
            scriptSettingsAction->setText(tr("Script Settings"));
            runScriptAction->setText(tr("Run"));
        }

        void ScriptMgrAddOn::initializeActions() {
            auto iWin = windowHandle();
            auto win = iWin->window();
            runScriptAction = new ActionItem("scriptmgr.RunScript", new QAction(this), this);
            batchProcessMainGroup = new ActionItem("scriptmgr.BatchProcessGroup", new QActionGroup(this), this);
            batchProcessMainMenu = new ActionItem("scriptmgr.BatchProcessMenu", ICore::createCoreMenu(win), this);
            scriptOperationsGroup = new ActionItem("scriptmgr.ScriptOperations", new QActionGroup(this), this);
            reloadScriptsAction = new ActionItem("scriptmgr.ReloadScripts", new QAction(this), this);
            scriptSettingsAction = new ActionItem("scriptmgr.ScriptSettings", new QAction(this), this);

            connect(reloadScriptsAction->action(), &QAction::triggered, ScriptLoader::instance(), &ScriptLoader::reloadEngine);
            connect(scriptSettingsAction->action(), &QAction::triggered, this, [=](){
                ScriptSettingsConfigurableDialog dlg(windowHandle()->window());
                dlg.exec();
            });
            connect(runScriptAction->action(), &QAction::triggered, this, &ScriptMgrAddOn::selectScript);
            iWin->addActionItems({
                runScriptAction,
                batchProcessMainGroup,
                batchProcessMainMenu,
                scriptOperationsGroup,
                reloadScriptsAction,
                scriptSettingsAction,
            });
        }

        void ScriptMgrAddOn::cleanupScriptActions() {
            for(auto action: allActions) {
                batchProcessMainMenu->menu()->removeAction(action);
                if(action->menu()) {
                    windowHandle()->removeShortcutContext(action->menu());
                    action->menu()->deleteLater();
                }
                action->deleteLater();
            }
            scriptMainActionDict.clear();
            allActions.clear();
        }

        static QString keySequenceToRichText(const QKeySequence &seq) {
            if (!seq.isEmpty()) {
                auto seqs = seq.toString(QKeySequence::PortableText).split(", ");
                QStringList seqsText;
                for (const auto &seqItem : qAsConst(seqs)) {
                    auto keys = seqItem.split('+');
                    for (auto &key : keys) {
                        key = "<quote> " + key + " </quote>";
                    }
                    seqsText.append(keys.join('+'));
                }
                return "<x0.8>" + seqsText.join("  ") + "</x0.8>";
            }
            return {};
        }

        void ScriptMgrAddOn::selectScript() {
            auto cp = windowHandle()->cast<Core::IProjectWindow>()->commandPalette();
            cp->abandon();
            for(auto key: scriptMainActionDict.keys()) {
                auto item = new QListWidgetItem;
                auto action = scriptMainActionDict.value(key);
                if(key.contains('.')) {
                    auto parentName = static_cast<QAction *>(action->data().value<void *>())->text();
                    item->setText(parentName + " > " + action->text());
                } else {
                    item->setText(action->text());
                }
                item->setData(QsApi::SubtitleRole, key);
                item->setData(QsApi::DescriptionRole, keySequenceToRichText(action->shortcut()));
                item->setData(QsApi::ObjectPointerRole, QVariant::fromValue(static_cast<void *>(action)));
                item->setData(QsApi::AlignmentRole, int(Qt::AlignTop));
                cp->addItem(item);
            }
            cp->setFilterHint(tr("Search for batch process actions"));
            cp->setCurrentRow(0);
            cp->start();
            auto obj = new QObject;
            connect(cp, &QsApi::CommandPalette::finished, obj, [=](QListWidgetItem *item){
                delete obj;
                if(!item) return;
                auto action = static_cast<QAction *>(item->data(QsApi::ObjectPointerRole).value<void *>());
                QTimer::singleShot(0, action, &QAction::trigger);
            });
        }

        void ScriptMgrAddOn::reloadActions() {
            windowKey = ScriptLoader::instance()->createHandles(internalObject);
            scriptActionContext.reset(new QWidget);
            createScriptActions(ScriptLoader::instance()->builtInScriptEntries());
            createScriptActions(ScriptLoader::instance()->scriptEntries());
//            qDebug() << scriptActionContext.data()->actions();
            windowHandle()->addShortcutContext(scriptActionContext.data(), IWindow::Mutable);
        }

        void ScriptMgrAddOn::createScriptActions(const QList<ScriptEntry> &entries) {
            auto actionBefore = reloadScriptsAction->action();
            for(const auto &entry: entries) {
                qDebug() << "Create action:" << entry.id;
                auto mainAction = new QAction(this);
                mainAction->setText(ScriptLoader::instance()->getName(entry.id));
                connect(ScriptLoader::instance(), &ScriptLoader::scriptNameReloaded, mainAction, [=](){
                    mainAction->setText(ScriptLoader::instance()->getName(entry.id));
                });
                if(entry.type == ScriptEntry::Script) {
                    mainAction->setShortcut(ScriptLoader::instance()->cachedCustomShortcuts().value(entry.id, QKeySequence(entry.shortcut)));
                    connect(mainAction, &QAction::triggered, this, [=](){
                        ScriptLoader::instance()->invoke(windowKey, entry.id);
                    });
                    scriptMainActionDict.append(entry.id, mainAction);
                    scriptActionContext->addAction(mainAction);
                } else {
                    auto menu = ICore::createCoreMenu(windowHandle()->window());
                    for(int i = 0; i < entry.childrenId.size(); i++) {
                        auto childId = entry.childrenId[i];
                        auto childShortcut = entry.childrenShortcut[i];
                        auto childAction = new QAction(this);
                        childAction->setText(ScriptLoader::instance()->getName(entry.id + '.' + childId));
                        connect(ScriptLoader::instance(), &ScriptLoader::scriptNameReloaded, childAction, [=](){
                            childAction->setText(ScriptLoader::instance()->getName(entry.id + '.' + childId));
                        });
                        childAction->setShortcut(ScriptLoader::instance()->cachedCustomShortcuts().value(entry.id + "." + childId, QKeySequence(childShortcut)));
                        connect(childAction, &QAction::triggered, this, [=](){
                            ScriptLoader::instance()->invoke(windowKey, entry.id, i);
                        });
                        childAction->setData(QVariant::fromValue(static_cast<void *>(mainAction)));
                        menu->addAction(childAction);
                        scriptActionContext->addAction(childAction);
                        scriptMainActionDict.append(entry.id + "." + childId, childAction);
                    }
                    mainAction->setMenu(menu);
                }
                batchProcessMainMenu->menu()->insertAction(actionBefore, mainAction);
                allActions.append(mainAction);
            }
            allActions.append(batchProcessMainMenu->menu()->insertSeparator(actionBefore));
        }
    }
}
