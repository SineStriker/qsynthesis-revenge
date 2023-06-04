#include "TemplatePlg.h"
#include "AddOn/TmplAddOn.h"

#include <CoreApi/ILoader.h>
#include <CoreApi/SettingCatalog.h>
#include <CoreApi/IWindowAddOn.h>

#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

#include "Settings/TreeConfigPage.h"
#include "Widgets/TreeConfigWidget.h"

#include <QApplication>
#include <QMDecoratorV2.h>
#include <QMSystem.h>
#include <QThread>

namespace TemplatePlg {
    using namespace Core;
    namespace Internal {
        TmplPlg::TmplPlg() {
        }

        TmplPlg::~TmplPlg() {
        }
        static TreeConfigWidget *config = nullptr;
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

            // First, set true and create "plugin/Res/configurations/config.treeui"(Empty file with treeui suffix).
            // After, clicking the "create treeui" to load config.treeui and creat config ui in the settings window.
            // And then, clicking the save button will save the treeui (ui) and create config.json (config information).
            // Finally, set false to enter application mode.
            config = new TreeConfigWidget(tr("core.tmpl"), pluginSpec()->location() + "/configs/", false);

            // Add Setting Page
            auto jsonConfigPage = new TreeConfigPage();
            ICore::instance()->settingCatalog()->addPage(jsonConfigPage);
            // qDebug() << "TreeConfig:" << config->readConfig("edit/spinbox");

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