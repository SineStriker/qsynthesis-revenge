#include "ParseDs.h"

#include <QApplication>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMDecorateDir.h>
#include <QMDecorator.h>
#include <QMSystem.h>

#include <extensionsystem/pluginspec.h>

#include "iemgr/IManager.h"

#include "DsWizard.h"

namespace IEMgr {

    using namespace Core;

    namespace Internal {

        class ParseDsPrivate {
        public:
            QMDecorateDir dec;
        };

        static ParseDsPrivate *d = nullptr;

        ParseDs::ParseDs() {
        }

        ParseDs::~ParseDs() {
        }

        bool ParseDs::initialize(const QStringList &arguments, QString *errorMessage) {
            auto &d = Internal::d;
            d = new ParseDsPrivate();

            d->dec.load(
                QString("%1/share/%2.res.json").arg(QMFs::PathFindDirPath(pluginSpec()->filePath()), "ParseDs"));

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