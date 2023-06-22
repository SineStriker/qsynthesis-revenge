#ifndef CHORUSKIT_SCRIPTMGRADDON_H
#define CHORUSKIT_SCRIPTMGRADDON_H

#include "CoreApi/ActionItem.h"
#include "CoreApi/IWindowAddOn.h"

#include "JsInternalObject.h"
#include "ScriptLoader.h"

namespace ScriptMgr::Internal {

    class ScriptMgrAddOnFactory : public Core::IWindowAddOnFactory {
    public:
        bool predicate(Core::IWindow *handle) const override;
        Core::IWindowAddOn *create(QObject *parent) override;
    private:
        QJSEngine *engine;
    };

    class ScriptMgrAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        explicit ScriptMgrAddOn(QObject *parent = nullptr);
        ~ScriptMgrAddOn();

        void initialize() override;
        void extensionsInitialized() override;

        void reloadStrings();

        void initializeActions();
        void reloadActions();
        void createScriptActions(const QList<ScriptEntry> &entries);

    signals:
        void handleJsReloadStrings();

    private:
        Core::ActionItem *batchProcessMainGroup;
        Core::ActionItem *batchProcessMainMenu;
        Core::ActionItem *scriptOperationsGroup;
        Core::ActionItem *reloadScriptsAction;
        Core::ActionItem *scriptSettingsAction;

        QString windowKey;
        JsInternalObject *internalObject;
        QList<QAction *> scriptActions;
    };

} // Internal

#endif // CHORUSKIT_SCRIPTMGRADDON_H
