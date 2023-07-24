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
#include "ScriptLoader.h"
#include "ScriptSettingsPage.h"

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

            auto rout = qobject_cast<Core::InitialRoutine *>(ILoader::instance()->getFirstObject("choruskit_init_routine"));
            if (rout) {
                rout->splash()->showMessage(tr("Initializing script manager..."));
            }

            auto actionMgr = ICore::instance()->actionSystem();
            if (actionMgr->loadContexts(":/scriptmgr_actions.xml").isEmpty()) {
                *errorMessage = tr("Failed to load action configuration!");
                return false;
            }

            auto sc = ICore::instance()->settingCatalog();
            sc->addPage(new ScriptSettingsPage);

            // Add basic windows and add-ons
            new ScriptLoader(this);
            ScriptLoader::instance()->setVersion(pluginSpec()->version().split("_")[0]);
            ScriptLoader::instance()->reloadEngine();

            auto winMgr = ICore::instance()->windowSystem();
            winMgr->addAddOn("project", &ScriptMgrAddOn::staticMetaObject);

            Core::ILoader::instance()->addObject("ScriptMgr.ScriptLoader", ScriptLoader::instance());

            return true;
        }

        void ScriptMgrPlugin::extensionsInitialized() {
            Core::ILoader::instance()->removeObjects("ScriptMgr.ScriptLoader");
        }

        bool ScriptMgrPlugin::delayedInitialize() {
            return false;
        }

        static void _qt_translation_CommandCategory() {
            QApplication::translate("Core::CommandCategory", "Script Manager");
            QApplication::translate("Core::CommandCategory", "Batch Process");
        }

    } // namespace Internal

} // namespace ScriptMgr
