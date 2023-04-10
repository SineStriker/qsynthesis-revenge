#include "ImportManager.h"

#include <QApplication>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMDecorateDir.h>
#include <QMDecorator.h>
#include <QMSystem.h>

#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

#include "AddOn/IEMgrAddOn.h"

#include "iemgr/IManager.h"

#include "Internal/Wizards/MidiWizard.h"
#include "Internal/Wizards/UstWizard.h"

namespace IEMgr {

    using namespace Core;

    namespace Internal {

        class ImportManagerPrivate {
        public:
            QMDecorateDir dec;
        };

        static IManager *imgr = nullptr;

        static ImportManagerPrivate *d = nullptr;

        ImportManager::ImportManager() {
        }

        ImportManager::~ImportManager() {
        }

        bool ImportManager::initialize(const QStringList &arguments, QString *errorMessage) {
            auto &d = Internal::d;
            d = new ImportManagerPrivate();

            d->dec.load(QString("%1/share/%2.res.json").arg(QMFs::PathFindDirPath(pluginSpec()->filePath()), "IEMgr"));

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
            winMgr->addAddOn(new IEMgrAddOnFactory());

            // Add wizards
            imgr->addWizard(new Internal::MidiWizard());
            imgr->addWizard(new Internal::UstWizard());

            qIDec->addThemeTemplate("IEMgr_ImportDialog", ":/themes/import-dialog.qss.in");

            return true;
        }

        void ImportManager::extensionsInitialized() {
        }

        bool ImportManager::delayedInitialize() {
            return true;
        }

    }

}