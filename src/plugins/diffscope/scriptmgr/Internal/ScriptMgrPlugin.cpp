#include "ScriptMgrPlugin.h"

#include <QApplication>
#include <QJsEngine>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

#include "AddOn/ScriptMgrAddOn.h"
#include "JsInternalObject.h"

namespace ScriptMgr {

    using namespace Core;

    namespace Internal {

        class BatchProcessPrivate {
        public:
            QJSEngine engine;
        };

        static BatchProcessPrivate *d = nullptr;

        BatchProcess::BatchProcess() {
        }

        BatchProcess::~BatchProcess() {
            delete Internal::d;
        }

        bool BatchProcess::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(QMFs::PathFindDirPath(pluginSpec()->filePath()) + "/translations");

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing script manager..."));
            }
            // QThread::msleep(2000);

            auto &d = Internal::d;
            d = new BatchProcessPrivate();

            d->engine.globalObject().setProperty("_internal", d->engine.newQObject(new JsInternalObject(&d->engine)));

            // Add basic windows and add-ons
            auto winMgr = ICore::instance()->windowSystem();
            winMgr->addAddOn(new ScriptMgrAddOnFactory(&d->engine));

            return true;
        }

        void BatchProcess::extensionsInitialized() {
        }

        bool BatchProcess::delayedInitialize() {
            return true;
        }

    }

}