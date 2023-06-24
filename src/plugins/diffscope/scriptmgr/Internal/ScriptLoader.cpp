#include "ScriptLoader.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>

#include "LoaderInjectedObject.h"
#include "JsInternalObject.h"
#include "ScriptSettingsPage.h"

namespace ScriptMgr::Internal {

    static ScriptLoader *m_instance = nullptr;

    ScriptLoader *ScriptLoader::instance() {
        return m_instance;
    }

    bool ScriptLoader::reloadEngine() {
        emit engineWillReload();
        m_engine.reset(new QJSEngine);
        m_scriptEntries.clear();
        m_builtInScriptEntries.clear();

        //load internal.js
        m_engine->globalObject().setProperty("__q_tr_ext", m_engine->newObject());
        m_engine->installExtensions(QJSEngine::TranslationExtension, m_engine->globalObject().property("__q_tr_ext"));
        m_engine->globalObject().setProperty("__q_loader", m_engine->newQObject(new LoaderInjectedObject(this)));
        auto internalModule = m_engine->importModule(":/scripts/internal.js");
        if(internalModule.isError()) {
            alertJsUncaughtError(internalModule);
            criticalCannotInitializeEngine();
            return false;
        }
        m_jsCallbacks = internalModule.property("__q_callbacks");

        m_engine->globalObject().setProperty("$tr", m_engine->globalObject().property("__q_tr_ext").property("qsTranslate"));
        m_engine->globalObject().setProperty("$register", internalModule.property("register"));
        m_engine->globalObject().setProperty("$Script", internalModule.property("Script"));
        m_engine->globalObject().setProperty("$ScriptSet", internalModule.property("ScriptSet"));

        //load built-in scripts
        for(const auto& filename: QDir(":/scripts/").entryList(QDir::Files)) {
            if(filename == "internal.js") continue;
            auto ret = m_engine->importModule(":/scripts/" + filename);
            if(ret.isError()){
                alertJsUncaughtError(ret);
                warningCannotLoadFile(":/scripts/" + filename);
            }
        }
        m_builtInScriptEntries = m_scriptEntries;
        m_scriptEntries.clear();

        if(ScriptSettingsPage::storedEnableUserScripts()) {
            QDir dir(userScriptDir());
            for(const auto& filename: dir.entryList(QDir::Files)) {
                auto ret = m_engine->importModule(dir.filePath(filename));
                if(ret.isError()){
                    alertJsUncaughtError(ret);
                    warningCannotLoadFile(dir.filePath(filename));
                }
            }
        }
        emit engineReloaded();
        return true;
    }

    QString ScriptLoader::createHandles(JsInternalObject *internalObject) {
        if(m_engine.isNull()) {
            qWarning() << "ScriptLoader: Engine not initialized.";
            return {};
        }
        auto jsInternal = m_engine->newQObject(internalObject);
        return m_jsCallbacks.property("createHandles").call({jsInternal}).toString();
    }

    void ScriptLoader::invoke(const QString &windowKey, const QString &id) {
        if(m_engine.isNull()) {
            qWarning() << "ScriptLoader: Engine not initialized.";
            return;
        }
        m_jsCallbacks.property("invoke").call({windowKey, id});
    }

    void ScriptLoader::invoke(const QString &windowKey, const QString &id, int index) {
        if(m_engine.isNull()) {
            qWarning() << "ScriptLoader: Engine not initialized.";
            return;
        }
        m_jsCallbacks.property("invoke").call({windowKey, id, index});
    }

    QString ScriptLoader::getName(const QString &id) {
        if(m_engine.isNull()) {
            qWarning() << "ScriptLoader: Engine not initialized.";
            return {};
        }
        return m_jsCallbacks.property("getName").call({id}).toString();
    }

    QString ScriptLoader::getName(const QString &id, int index) {
        if(m_engine.isNull()) {
            qWarning() << "ScriptLoader: Engine not initialized.";
            return {};
        }
        return m_jsCallbacks.property("getName").call({id, index}).toString();
    }

    QJSValue ScriptLoader::getInfo(const QString &id) {
        if(m_engine.isNull()) {
            qWarning() << "ScriptLoader: Engine not initialized.";
            return {};
        }
        return m_jsCallbacks.property("getInfo").call({id});
    }

    void ScriptLoader::removeHandles(const QString &windowKey) {
        if(m_engine.isNull()) {
            qWarning() << "ScriptLoader: Engine not initialized.";
            return;
        }
        m_jsCallbacks.property("removeHandles").call({windowKey});
    }

    QList<ScriptEntry> ScriptLoader::builtInScriptEntries() const {
        return m_builtInScriptEntries;
    }
    QList<ScriptEntry> ScriptLoader::scriptEntries() const {
        return m_scriptEntries;
    }

    QString ScriptLoader::userScriptDir() {
        return ScriptSettingsPage::storedUserScriptsDirectory();
    }

    ScriptLoader::ScriptLoader(QObject *parent): QObject(parent) {
        m_instance = this;
    }

    ScriptLoader::~ScriptLoader() {
        m_instance = nullptr;
    }

    void ScriptLoader::alertJsUncaughtError(const QJSValue& error) {
        QString msg = tr("Uncaught error.") + "\n\n" + QString("%1:%2").arg(error.property("fileName").toString(), error.property("lineNumber").toString()) + "\n\n" + error.toString() + "\n\n" + error.property("stack").toString();
        QMessageBox::warning(nullptr, tr("JavaScript Error"), msg);
    }

    void ScriptLoader::warningCannotLoadFile(const QString &path) {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Cannot load script file '%1'. The file is ignored.").arg(path));
    }

    void ScriptLoader::warningCannotGetName(const QString &id) {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Cannot get name of script '%1'. Its name will be displayed as its id."));
    }

    void ScriptLoader::criticalCannotInitializeEngine() {
        QMessageBox::critical(nullptr, tr("Error"), tr("Cannot initialize JavaScript engine."));
    }

    void ScriptLoader::criticalScriptExecutionFailed(const QString &name) {
        QMessageBox::critical(nullptr, tr("Error"), tr("Script '%1' execution failed.").arg(name));
    }
    QJSEngine *ScriptLoader::engine() {
        return m_engine.data();
    }

}