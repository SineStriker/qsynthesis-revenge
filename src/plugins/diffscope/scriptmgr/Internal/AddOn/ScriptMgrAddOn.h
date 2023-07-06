#ifndef CHORUSKIT_SCRIPTMGRADDON_H
#define CHORUSKIT_SCRIPTMGRADDON_H

#include <CoreApi/IWindowAddOn.h>
#include <CoreApi/ActionItem.h>
#include <Widgets/CommandPalette.h>
#include <Collections/QMChronMap.h>

#include "ScriptLoader.h"

namespace ScriptMgr::Internal {

    class ScriptMgrAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit ScriptMgrAddOn(QObject *parent = nullptr);
        ~ScriptMgrAddOn();

        void initialize() override;
        void extensionsInitialized() override;


    private:
        void reloadStrings();

        void initializeActions();
        void reloadActions();
        void createScriptActions(const QList<ScriptEntry> &entries);
        void cleanupScriptActions();
        void selectScript();

        Core::ActionItem *batchProcessMainGroup;
        Core::ActionItem *batchProcessMainMenu;
        Core::ActionItem *scriptOperationsGroup;
        Core::ActionItem *reloadScriptsAction;
        Core::ActionItem *scriptSettingsAction;
        Core::ActionItem *runScriptAction;

        QScopedPointer<QWidget> scriptActionContext;

        QString windowKey;
        JsInternalObject *internalObject;
        QList<QAction *> allActions;
        QMChronMap<QString, QAction *> scriptMainActionDict;
    };

} // Internal

#endif // CHORUSKIT_SCRIPTMGRADDON_H
