#include "ImportManager.h"

#include <QApplication>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMDecorateDir.h>
#include <QMSystem.h>

#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

#include "AddOn/IEMgrAddOn.h"

namespace IEMgr {

    using namespace Core;

    namespace Internal {

        class ImportManagerPrivate {
        public:
            QMDecorateDir dec;
        };

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

            // Add basic actions
            auto actionMgr = ICore::instance()->actionSystem();

            if (actionMgr->loadContexts(":/iemgr_actions.xml").isEmpty()) {
                *errorMessage = tr("Failed to load action configuration!");
                return false;
            }

            // Add basic windows and add-ons
            auto winMgr = ICore::instance()->windowSystem();

            winMgr->addAddOn(new IEMgrAddOnFactory());

            return true;
        }

        void ImportManager::extensionsInitialized() {
            qDebug() << "extensions initialized ImportManager Plugin";
        }

        bool ImportManager::delayedInitialize() {
            qDebug() << "delayed initialize ImportManager Plugin";
            return true;
        }

    }

}