#include "ScriptLoader.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>

#include <QMDecoratorV2.h>

#include "JsInternalObject.h"
#include "JsLoaderObject.h"
#include "ScriptSettingsPage.h"

#define CHECK_ENGINE(ret) \
if(m_engine.isNull()) { \
    qWarning() << "ScriptLoader: Engine not initialized."; \
    return (ret); \
}

namespace ScriptMgr::Internal {

    static ScriptLoader *m_instance = nullptr;

    ScriptLoader *ScriptLoader::instance() {
        return m_instance;
    }

    bool ScriptLoader::loadFile(const QString &path) {
        QFile f(path);
        f.open(QFile::ReadOnly);
        if(!f.isOpen()) {
            return false;
        }
        auto ret = m_engine->evaluate(f.readAll(), path);
        if(ret.isError()) {
            alertJsUncaughtError(ret);
            return false;
        }
        return true;
    }

    bool ScriptLoader::reloadEngine() {
        emit engineWillReload();
        m_engine.reset(new QJSEngine);
        m_scriptEntries.clear();
        m_builtInScriptEntries.clear();
        m_cachedCustomShortcuts = ScriptSettingsPage::storedCustomShortcuts();

        //load internal.js
        m_engine->globalObject().setProperty("__q_tr_ext", m_engine->newObject());
        m_engine->installExtensions(QJSEngine::TranslationExtension, m_engine->globalObject().property("__q_tr_ext"));
        m_engine->installExtensions(QJSEngine::ConsoleExtension);
        m_engine->globalObject().setProperty("__q_loader", m_engine->newQObject(new JsLoaderObject(this)));
        m_engine->globalObject().setProperty("__q_internal", m_engine->newQObject(new JsInternalObject(nullptr)));
        auto internalModule = m_engine->importModule(":/scripts/internal/index.js");
        if(internalModule.isError()) {
            alertJsUncaughtError(internalModule);
            criticalCannotInitializeEngine();
            return false;
        }
        m_jsCallbacks = internalModule.property("__q_callbacks");

        m_engine->globalObject().setProperty("__q_tr", m_engine->globalObject().property("__q_tr_ext").property("qsTranslate"));
        m_engine->globalObject().setProperty("$register", internalModule.property("register"));
        m_engine->globalObject().setProperty("$Script", internalModule.property("Script"));
        m_engine->globalObject().setProperty("$ScriptSet", internalModule.property("ScriptSet"));

        //load built-in scripts
        for(const auto& filename: QDir(":/scripts/").entryList(QDir::Files)) {
            if(filename == "internal.js") continue;
            if(!loadFile(":/scripts/" + filename)){
                warningCannotLoadFile(":/scripts/" + filename);
            }
        }
        m_builtInScriptEntries = m_scriptEntries;
        m_scriptEntries.clear();

        if(ScriptSettingsPage::storedEnableUserScripts()) {
            QDir dir(userScriptDir());
            for(const auto& filename: dir.entryList(QDir::Files)) {
                if(!loadFile(dir.filePath(filename))){
                    warningCannotLoadFile(dir.filePath(filename));
                }
            }
        }
        reloadStrings();
        emit engineReloaded();
        return true;
    }

    QString ScriptLoader::createHandles(JsInternalObject *internalObject) const {
        CHECK_ENGINE("")
        auto jsInternal = m_engine->newQObject(internalObject);
        return m_jsCallbacks.property("createHandles").call({jsInternal}).toString();
    }

    void ScriptLoader::invoke(const QString &windowKey, const QString &id) const {
        CHECK_ENGINE(void())
        auto ret = m_jsCallbacks.property("invoke").call({windowKey, id});
        if(ret.isError()) {
            alertJsUncaughtError(ret);
            criticalScriptExecutionFailed(getName(id));
        }
    }

    void ScriptLoader::invoke(const QString &windowKey, const QString &id, int index) const {
        CHECK_ENGINE(void())
        auto ret = m_jsCallbacks.property("invoke").call({windowKey, id, index});
        if(ret.isError()) {
            alertJsUncaughtError(ret);
            criticalScriptExecutionFailed(getName(id + "@" + QString::number(index)));
        }
    }

    QString ScriptLoader::getNameFromJs(const QString &id) const {
        CHECK_ENGINE("")
        auto ret = m_jsCallbacks.property("getNameFromJs").call({id});
        if(ret.isError()) {
            alertJsUncaughtError(ret);
            warningCannotGetName(id);
            return id;
        }
        return ret.toString();
    }

    QString ScriptLoader::getNameFromJs(const QString &id, const QString &childId, int index) const {
        CHECK_ENGINE("")
        auto ret = m_jsCallbacks.property("getNameFromJs").call({id, index});
        if(ret.isError()) {
            alertJsUncaughtError(ret);
            warningCannotGetName(id);
        }
        if(!ret.isString()) return id + '.' + childId;
        return ret.toString();
    }

    QJSValue ScriptLoader::getInfo(const QString &id) const {
        CHECK_ENGINE("")
        auto ret = m_jsCallbacks.property("getInfo").call({id});
        if(ret.isError()) {
            alertJsUncaughtError(ret);
            return QJSValue::NullValue;
        }
        return ret;
    }

    void ScriptLoader::removeHandles(const QString &windowKey) const {
        CHECK_ENGINE(void())
        auto ret = m_jsCallbacks.property("removeHandles").call({windowKey});
        if(ret.isError()) {
            alertJsUncaughtError(ret);
        }
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
        qIDec->installLocale(this, _LOC(ScriptLoader, this));
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
        QMessageBox::warning(nullptr, tr("Warning"), tr("Cannot get name of script '%1'. Its name will be displayed as its id.").arg(id));
    }

    void ScriptLoader::criticalCannotInitializeEngine() {
        QMessageBox::critical(nullptr, tr("Error"), tr("Cannot initialize JavaScript engine."));
    }

    void ScriptLoader::criticalScriptExecutionFailed(const QString &name) {
        QMessageBox::critical(nullptr, tr("Error"), tr("Script '%1' execution failed.").arg(name));
    }
    QJSEngine *ScriptLoader::engine() const {
        return m_engine.data();
    }
    void ScriptLoader::setVersion(const QString &version) {
        m_version = version;
    }
    QString ScriptLoader::version() const {
        return m_version;
    }
    void ScriptLoader::reloadStrings() {
        scriptNameDict.clear();
        for(const auto &entry: m_builtInScriptEntries) {
            scriptNameDict.insert(entry.id, getNameFromJs(entry.id));
            if(entry.type == ScriptEntry::ScriptSet) {
                for(int i = 0; i < entry.childrenId.size(); i++) {
                    auto childId = entry.childrenId[i];
                    auto childName = getNameFromJs(entry.id, childId, i);
                    scriptNameDict.insert(entry.id + "." + childId, childName);
                    scriptNameDict.insert(entry.id + "@" + QString::number(i), childName);
                }
            }
        }
        emit scriptNameReloaded();
    }

    QString ScriptLoader::getName(const QString &id) const {
        return scriptNameDict.value(id, id);
    }
    QMap<QString, QKeySequence> ScriptLoader::cachedCustomShortcuts() const {
        return m_cachedCustomShortcuts;
    }
    bool ScriptLoader::setHandleExtension(const QString &name, QObject *extFactory) {
        if(m_extFactoryDict.contains(name)) return false;
        m_extFactoryDict.insert(name, extFactory);
        qDebug() << "ScriptLoader: Handle extension added" << name;
        return true;
    }

}