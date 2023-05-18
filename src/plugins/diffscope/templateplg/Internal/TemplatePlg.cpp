#include "TemplatePlg.h"

#include "AddOn/TmplAddOn.h"
#include "CoreApi/ILoader.h"
#include "CoreApi/SettingCatalog.h"
#include "Settings/ConfigPage.h"
#include "Settings/TreeConfigPage.h"
#include <QApplication>
#include <QMDecoratorV2.h>
#include <QMSystem.h>
#include <QThread>
#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

namespace TemplatePlg {
    using namespace Core;
    namespace Internal {
        TmplPlg::TmplPlg() {
        }

        TmplPlg::~TmplPlg() {
        }

        // Plugin Initialize
        bool TmplPlg::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(pluginSpec()->location() + "/translations");
            // qIDec->addThemePath(pluginSpec()->location() +"/themes");

            // Add initialization information
            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing Template Plugin..."));
            }
            // QThread::msleep(2000);

            // Add Menu
            auto actionMgr = ICore::instance()->actionSystem();
            if (actionMgr->loadContexts(":/template_actions.xml").isEmpty()) {
                *errorMessage = tr("Failed to load action configuration!");
                return false;
            }

            // Add Setting Page
            auto configPage = new ConfigPage();
            ICore::instance()->settingCatalog()->addPage(configPage);

            // First, set false and create "plugin/Res/configurations/config. json".
            // After loading and saving the configuration in the settings window, set true to enter application mode
            auto jsonConfigPage = new TreeConfigPage(pluginSpec()->location() + "/configs/config.json", true);
            ICore::instance()->settingCatalog()->addPage(jsonConfigPage);

            // Add basic windows and add-ons
            auto winMgr = ICore::instance()->windowSystem();
            winMgr->addAddOn(new TmplAddOnFactory());
            return true;
        }

        void TmplPlg::extensionsInitialized() {
        }

        bool TmplPlg::delayedInitialize() {
            return false;
        }
    }

}