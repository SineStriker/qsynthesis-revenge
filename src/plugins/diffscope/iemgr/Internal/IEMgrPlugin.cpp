#include "IEMgrPlugin.h"

#include <QApplication>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

#include "AddOn/IEMgrAddOn.h"

#include "iemgr/IManager.h"

#include "Internal/Wizards/MidiWizard.h"
#include "Internal/Wizards/OpenSvipWizard.h"
#include "Internal/Wizards/SvipWizard.h"
#include "Internal/Wizards/UstWizard.h"

namespace IEMgr {

    using namespace Core;

    namespace Internal {

        static IManager *imgr = nullptr;

        IEMgrPlugin::IEMgrPlugin() {
        }

        IEMgrPlugin::~IEMgrPlugin() {
        }

        bool IEMgrPlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(pluginSpec()->location() + "/translations");
            qIDec->addThemePath(pluginSpec()->location() + "/themes");

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing import/export manager..."));
            }
            // QThread::msleep(2000);

            // Init IManager instance
            imgr = new IManager(this);

            auto icore = ICore::instance();

            // Add basic actions
            auto actionMgr = icore->actionSystem();
            if (actionMgr->loadContexts(":/iemgr_actions.xml").isEmpty()) {
                *errorMessage = tr("Failed to load action configuration!");
                return false;
            }

            // Add basic windows and add-ons
            auto winMgr = icore->windowSystem();
            winMgr->addAddOn({"home","project"}, &IEMgrAddOn::staticMetaObject);

            // Add wizards
            imgr->addWizard(new MidiWizard());
            imgr->addWizard(new UstWizard());
            imgr->addWizard(new OpenSvipWizard());
            imgr->addWizard(new SvipWizard());

            return true;
        }

        void IEMgrPlugin::extensionsInitialized() {
        }

        bool IEMgrPlugin::delayedInitialize() {
            return false;
        }

        // This scope is only to expose strings to Qt translation tool
        static void _qt_translation_CommandCategory() {
            QApplication::translate("Core::CommandCategory", "Import");
            QApplication::translate("Core::CommandCategory", "Export");
        }

    }

}
