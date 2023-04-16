#include "ScriptMgrAddOn.h"

#include <CoreApi/IWindow.h>
#include <QMDecoratorV2.h>

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
        }

        void ScriptMgrAddOn::initialize() {
            qIDec->installLocale(this, _LOC(ScriptMgrAddOn, this));
        }

        void ScriptMgrAddOn::extensionsInitialized() {
        }

        void ScriptMgrAddOn::reloadStrings() {
        }

        void ScriptMgrAddOn::loadBuiltInScripts() {
        }

        void ScriptMgrAddOn::loadUserScripts() {
        }

    }
}