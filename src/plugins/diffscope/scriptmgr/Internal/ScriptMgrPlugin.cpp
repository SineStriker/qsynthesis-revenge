#include "ScriptMgrPlugin.h"

#include <QApplication>
#include <QJSEngine>
#include <QThread>

#include "CoreApi/ILoader.h"

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

#include "AddOn/ScriptMgrAddOn.h"
#include "JsInternalObject.h"
#include "ScriptLoader.h"

namespace ScriptMgr {

    using namespace Core;

    namespace Internal {

        BatchProcess::BatchProcess() {
        }

        BatchProcess::~BatchProcess() {
        }

        static ScriptLoader *scriptLoader = nullptr;

        bool BatchProcess::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(QMFs::PathFindDirPath(pluginSpec()->filePath()) + "/translations");

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing script manager..."));
            }

            // Add basic windows and add-ons
            scriptLoader = new ScriptLoader(this);
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