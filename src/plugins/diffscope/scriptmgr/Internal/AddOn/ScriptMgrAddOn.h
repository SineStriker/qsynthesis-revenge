#ifndef CHORUSKIT_SCRIPTMGRADDON_H
#define CHORUSKIT_SCRIPTMGRADDON_H

#include "Collections/QMChronMap.h"
#include "Collections/QMChronSet.h"
#include "CoreApi/ActionItem.h"
#include "CoreApi/IWindowAddOn.h"
#include "JsInternalObject.h"
#include <QJSEngine>

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
        enum ScriptRole {
            Arrangement = 0x01,
            Pianoroll = 0x02,
            Parameter = 0x04,
        };
        void initializeActions();
        void registerScript(const QString &id, int role, const QString &shortcut);
        void registerScript(const QString &id, const QStringList &children, int role,
                            const QStringList &childrenShortcuts);
        QString getName(const QString &id) const;
        QString getName(const QString &id, int index) const;
        void invoke(const QString &id) const;
        void invoke(const QString &id, int index) const;

    signals:
        void handleJsReloadStrings();

    public:
        bool initializeEngine();
        void alertJsUncaughtError(const QJSValue &error) const;
        void warningCannotLoadFile(const QString &path) const;
        void warningCannotGetName(const QString &id) const;
        void criticalCannotInitializeEngine() const;
        void criticalScriptExecutionFailed(const QString &id) const;
        void criticalScriptExecutionFailed(const QString &id, int index) const;
        bool loadScriptFile(const QString &path);
        void loadScripts();

    private:
        friend class ScriptLoader;
        QJSEngine engine;
        QList<QString> userScriptIdRegistry;
        bool builtInScriptInitialized = false;

    private:
        Core::ActionItem *batchProcessMainGroup;
        Core::ActionItem *batchProcessMainMenu;
        Core::ActionItem *builtInScriptsMainGroup;
        Core::ActionItem *userScriptsMainGroup;
        Core::ActionItem *scriptOperationsGroup;
        Core::ActionItem *reloadScriptsAction;
        Core::ActionItem *scriptSettingsAction;
    };

} // Internal

#endif // CHORUSKIT_SCRIPTMGRADDON_H
