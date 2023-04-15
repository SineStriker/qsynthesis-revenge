#include "ParseDs.h"

#include <QApplication>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <extensionsystem/pluginspec.h>

#include "iemgr/IManager.h"

#include "DsWizard.h"

namespace IEMgr {

    using namespace Core;

    namespace Internal {

        ParseDs::ParseDs() {
        }

        ParseDs::~ParseDs() {
        }

        bool ParseDs::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(QMFs::PathFindDirPath(pluginSpec()->filePath()) + "/translations");
            // qIDec->addThemePath(QMFs::PathFindDirPath(pluginSpec()->filePath()) +"/themes");

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing OpenVPI Ds parser..."));
            }
            // QThread::msleep(2000);

            auto imgr = IEMgr::IManager::instance();

            // Add wizards
            imgr->addWizard(new Internal::DsWizard());

            return true;
        }

        void ParseDs::extensionsInitialized() {
        }

        bool ParseDs::delayedInitialize() {
            return true;
        }

    }

}