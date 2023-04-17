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

            // d->engine.globalObject().setProperty("$", d->engine.newQObject(new TestObject));
            // qDebug() << d->engine.evaluate("$.f(['114514', '1919810'])").toString().toStdString().c_str();

            // Add basic windows and add-ons
            auto winMgr = ICore::instance()->windowSystem();
            winMgr->addAddOn(new ScriptMgrAddOnFactory());

            return true;
        }

        void BatchProcess::extensionsInitialized() {
        }

        bool BatchProcess::delayedInitialize() {
            return true;
        }

    }

}