#include "ScriptMgrPlugin.h"

#include <QApplication>
#include <QJSEngine>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

#include "AddOn/ScriptMgrAddOn.h"
#include "JsInternalObject.h"
#include "ScriptLoader.h"

namespace ScriptMgr {

    using namespace Core;

    namespace Internal {

        ScriptMgrPlugin::ScriptMgrPlugin() {
        }

        ScriptMgrPlugin::~ScriptMgrPlugin() {
        }

        static ScriptLoader *scriptLoader = nullptr;

        bool ScriptMgrPlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(pluginSpec()->location() + "/translations");

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing script manager..."));
            }

            auto actionMgr = ICore::instance()->actionSystem();
            if (actionMgr->loadContexts(":/scriptmgr_actions.xml").isEmpty()) {
                *errorMessage = tr("Failed to load action configuration!");
                return false;
            }

            // Add basic windows and add-ons
            scriptLoader = new ScriptLoader(this);
            auto winMgr = ICore::instance()->windowSystem();
            winMgr->addAddOn(new ScriptMgrAddOnFactory());

            return true;
        }

        void ScriptMgrPlugin::extensionsInitialized() {
        }

        bool ScriptMgrPlugin::delayedInitialize() {
            return true;
        }

    }

}