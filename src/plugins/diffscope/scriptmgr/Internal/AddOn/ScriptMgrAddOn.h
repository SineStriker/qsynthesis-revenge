#ifndef CHORUSKIT_SCRIPTMGRADDON_H
#define CHORUSKIT_SCRIPTMGRADDON_H

#include "CoreApi/IWindowAddOn.h"
#include <QJSEngine>

namespace ScriptMgr::Internal {

    class ScriptMgrAddOnFactory : public Core::IWindowAddOnFactory {
    public:
        explicit ScriptMgrAddOnFactory(QJSEngine *engine);
        bool predicate(Core::IWindow *handle) const override;
        Core::IWindowAddOn *create(QObject *parent) override;
    private:
        QJSEngine *engine;
    };

    class ScriptMgrAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        explicit ScriptMgrAddOn(QJSEngine* engine, QObject *parent = nullptr);
        ~ScriptMgrAddOn();

        void initialize() override;
        void extensionsInitialized() override;

    public:
        void reloadStrings();
        void loadBuiltInScripts();
        void loadUserScripts();

    private:
        QJSEngine* engine;
    };

} // Internal

#endif // CHORUSKIT_SCRIPTMGRADDON_H
