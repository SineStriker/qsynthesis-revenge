#ifndef CHORUSKIT_SCRIPTMGRADDON_H
#define CHORUSKIT_SCRIPTMGRADDON_H

#include "CoreApi/IWindowAddOn.h"

namespace ScriptMgr::Internal {

    class ScriptMgrAddOnFactory : public Core::IWindowAddOnFactory {
    public:
        bool predicate(Core::IWindow *handle) const override;
        Core::IWindowAddOn *create(QObject *parent) override;
    };

    class ScriptMgrAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        explicit ScriptMgrAddOn(QObject *parent = nullptr);
        ~ScriptMgrAddOn();

        void initialize() override;
        void extensionsInitialized() override;

    public:
        void reloadStrings();
        void loadBuiltInScripts();
        void loadUserScripts();

    private:
    };

} // Internal

#endif // CHORUSKIT_SCRIPTMGRADDON_H
