#include "ScriptMgrAddOn.h"
#include "JsInternalObject.h"
#include "ScriptLoader.h"

#include <QDebug>

#include <CoreApi/IWindow.h>
#include <QDir>
#include <QFile>
#include <QMDecoratorV2.h>
#include <QMessageBox>

namespace ScriptMgr {

    using namespace Core;

    namespace Internal {

        bool ScriptMgrAddOnFactory::predicate(Core::IWindow *handle) const {
            return handle->id() == "project" || handle->id() == "home"; // TODO temporarily add to home
        }

        Core::IWindowAddOn *ScriptMgrAddOnFactory::create(QObject *parent) {
            return new ScriptMgrAddOn();
        }

        ScriptMgrAddOn::ScriptMgrAddOn(QObject *parent) : IWindowAddOn(parent) {
        }

        ScriptMgrAddOn::~ScriptMgrAddOn() {
        }

        void ScriptMgrAddOn::initialize() {
            qIDec->installLocale(this, _LOC(ScriptMgrAddOn, this));
            if(!initializeEngine()) {
                criticalCannotInitializeEngine();
                return;
            }
        }

        void ScriptMgrAddOn::extensionsInitialized() {
        }

        void ScriptMgrAddOn::reloadStrings() {
        }

        bool ScriptMgrAddOn::initializeEngine() {
            engine.globalObject().setProperty("_internal", engine.newQObject(new JsInternalObject(&engine, this)));
            engine.globalObject().setProperty("_q_tr_ext", engine.newObject());
            engine.installExtensions(QJSEngine::TranslationExtension, engine.globalObject().property("_q_tr_ext"));
            if(!loadScriptFile(":/scripts/internal.js")) {
                return false;
            }
            loadScripts();
            invoke("transpose", 2); //TODO this is a test
            return true;
        }

        void ScriptMgrAddOn::loadScripts() {
            QDir dir(":/scripts/");
            for(const auto& filename: dir.entryList(QDir::Files)) {
                if(filename == "internal.js") continue;
                if(!loadScriptFile(":/scripts/" + filename)){
                    warningCannotLoadFile(":/scripts/" + filename);
                }
            }
            //TODO load user scripts
        }

        void ScriptMgrAddOn::registerScript(const QString &id, int role) {

        }

        void ScriptMgrAddOn::registerScript(const QString &id, const QStringList &children, int role) {
        }

        QString ScriptMgrAddOn::getName(const QString &id) const {
            auto ret = engine.globalObject().property("_getName").call({id});
            if(ret.isString()) {
                return ret.toString();
            } else {
                if(ret.isError()) {
                    alertJsUncaughtError(ret);
                }
                warningCannotGetName(id);
                return id;
            }
        }

        QString ScriptMgrAddOn::getName(const QString &id, int index) const {
            auto ret = engine.globalObject().property("_getName").call({id, index});
            if(ret.isString()) {
                return ret.toString();
            } else {
                if(ret.isError()) {
                    alertJsUncaughtError(ret);
                }
                warningCannotGetName(id);
                return id; //TODO child id
            }
        }

        void ScriptMgrAddOn::invoke(const QString &id) const {
            auto ret = engine.globalObject().property("_invoke").call({id});
            if(ret.isError()) {
                alertJsUncaughtError(ret);
                criticalScriptExecutionFailed(id);
            }
        }

        void ScriptMgrAddOn::invoke(const QString &id, int index) const {
            auto ret = engine.globalObject().property("_invoke").call({id, index});
            if(ret.isError()) {
                alertJsUncaughtError(ret);
                criticalScriptExecutionFailed(id, index);
            }
        }

        bool ScriptMgrAddOn::loadScriptFile(const QString &path) {
            QFile scriptFile(path);
            if(!scriptFile.open(QIODevice::ReadOnly)) {
                alertJsUncaughtError(engine.evaluate(QString("new Error(`Cannot open file '%1'.`)").arg(path)));
                return false;
            }
            QTextStream stream(&scriptFile);
            stream.setCodec("UTF-8");
            auto ret =  engine.evaluate(scriptFile.readAll(), path);
            if(ret.isError()) {
                alertJsUncaughtError(ret);
                return false;
            }
            return true;
        }

        void ScriptMgrAddOn::alertJsUncaughtError(const QJSValue& error) const {
            QString msg = tr("Uncaught error.") + "\n\n" + QString("%1:%2").arg(error.property("fileName").toString(), error.property("lineNumber").toString()) + "\n\n" + error.toString() + "\n\n" + error.property("stack").toString();
            QMessageBox::warning(windowHandle()->window(), tr("JavaScript Error"), msg);
        }

        void ScriptMgrAddOn::warningCannotLoadFile(const QString &path) const {
            QMessageBox::warning(windowHandle()->window(), tr("Warning"), tr("Cannot load script file '%1'. The file is ignored.").arg(path));
        }

        void ScriptMgrAddOn::warningCannotGetName(const QString &id) const {
            QMessageBox::warning(windowHandle()->window(), tr("Warning"), tr("Cannot get name of script '%1'. Its name will be displayed as its id."));
        }

        void ScriptMgrAddOn::criticalCannotInitializeEngine() const {
            QMessageBox::critical(windowHandle()->window(), tr("Error"), tr("Cannot initialize JavaScript engine."));
        }

        void ScriptMgrAddOn::criticalScriptExecutionFailed(const QString &id) const {
            QMessageBox::critical(windowHandle()->window(), tr("Error"), tr("Script '%1' execution failed.").arg(getName(id)));
        }

        void ScriptMgrAddOn::criticalScriptExecutionFailed(const QString &id, int index) const {
            QMessageBox::critical(windowHandle()->window(), tr("Error"), tr("Script '%1' execution failed.").arg(getName(id, index)));
        }
    }
}