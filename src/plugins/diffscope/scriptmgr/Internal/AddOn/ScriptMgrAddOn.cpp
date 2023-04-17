#include "ScriptMgrAddOn.h"
#include "JsInternalObject.h"

#include <CoreApi/IWindow.h>
#include <QMDecoratorV2.h>

namespace ScriptMgr {

    using namespace Core;

    namespace Internal {

        bool ScriptMgrAddOnFactory::predicate(Core::IWindow *handle) const {
            return handle->id() == "project" || handle->id() == "home"; //TODO temporarily add to home
        }

        Core::IWindowAddOn *ScriptMgrAddOnFactory::create(QObject *parent) {
            return new ScriptMgrAddOn(engine);
        }

        ScriptMgrAddOnFactory::ScriptMgrAddOnFactory(QJSEngine *engine): engine(engine) {
        }

        ScriptMgrAddOn::ScriptMgrAddOn(QJSEngine *engine, QObject *parent) : IWindowAddOn(parent), engine(engine) {
        }

        ScriptMgrAddOn::~ScriptMgrAddOn() {
        }

        void ScriptMgrAddOn::initialize() {
            qIDec->installLocale(this, _LOC(ScriptMgrAddOn, this));
            qobject_cast<JsInternalObject *>(engine->globalObject().property("_internal").toQObject())->setAddOn(this);
            qDebug(engine->evaluate("JSON.stringify(_internal.form('Test', [{type: 'TextBox', label: '111', defaultValue: '114514'}]))").toString().toLocal8Bit());
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