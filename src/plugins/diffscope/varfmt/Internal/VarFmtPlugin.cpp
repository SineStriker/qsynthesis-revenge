#include "VarFmtPlugin.h"

#include <QApplication>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <extensionsystem/pluginspec.h>

#include "iemgr/IManager.h"

#include "AcepWizard.h"
#include "DsWizard.h"

namespace VarFmt {

    using namespace Core;

    namespace Internal {

        VarFmtPlugin::VarFmtPlugin() {
        }

        VarFmtPlugin::~VarFmtPlugin() {
        }

        bool VarFmtPlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(pluginSpec()->location() + "/translations");
            // qIDec->addThemePath(pluginSpec()->location() +"/themes");

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing variable formats converter..."));
            }
            // QThread::msleep(2000);

            auto imgr = IEMgr::IManager::instance();

            // Add wizards
            imgr->addWizard(new DsWizard());
            imgr->addWizard(new AcepWizard());

            return true;
        }

        void VarFmtPlugin::extensionsInitialized() {
        }

        bool VarFmtPlugin::delayedInitialize() {
            return false;
        }

    }

}