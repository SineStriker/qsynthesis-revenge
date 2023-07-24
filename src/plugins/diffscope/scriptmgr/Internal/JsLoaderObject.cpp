#include "JsLoaderObject.h"

#include <QUuid>

#include "QMDecoratorV2.h"

#include "JsUtils.h"
#include "ScriptLoader.h"
#include "JsInternalObject.h"
#include "AddOn/ScriptMgrAddOn.h"


namespace ScriptMgr::Internal {
    void JsLoaderObject::registerScript(const QString &id, const QStringList &role, const QString &shortcut) const {
        m_loader->m_scriptEntries.append({ id, ScriptEntry::Script, role, shortcut});
    }
    void JsLoaderObject::registerScriptSet(const QString &id, const QStringList &childrenId, const QStringList &role, const QStringList &childrenShortcut) const {
        m_loader->m_scriptEntries.append({id, ScriptEntry::ScriptSet, role, {}, childrenId, childrenShortcut});
    }
    QString JsLoaderObject::uuid() const {
        return QUuid::createUuid().toString();
    }
    void JsLoaderObject::assertFalse() const {
        Q_ASSERT(false);
    }
    QString JsLoaderObject::version() const {
        return ScriptLoader::instance()->version();
    }
    QString JsLoaderObject::getLang() const {
        return qIDec->locale();
    }
    JsLoaderObject::JsLoaderObject(ScriptLoader *loader): m_loader(loader) {
    }
    QString JsLoaderObject::coreMsgBox(const QString &title, const QString &message, const QString &icon, const QStringList &buttons, const QString &defaultButton) const {
        return jsMsgBox(nullptr, title, message, icon, buttons, defaultButton);
    }
    bool JsLoaderObject::versionMismatchWarning(const QString &id, const QString &requiredVersion) const {
        return coreMsgBox(
            tr("Version Mismatch"),
            tr("The version of DiffScope Script Manager (%1) does not satisfy the requirement in script '%2' (%3).\n\nContinue loading anyway?")
                .arg(version(), id, requiredVersion),
            "Warning",
            {"Yes", "No"},
            "No") == "No";
    }
    QStringList JsLoaderObject::handleExtList() const {
        return m_loader->m_extFactoryDict.keys();
    }
    QJSValue JsLoaderObject::getHandleExt(const QString &name, const QJSValue &internalObjectJSValue) const {
        auto win = qobject_cast<JsInternalObject *>(internalObjectJSValue.toQObject())->addOn->windowHandle()->cast<Core::IProjectWindow>();
        QJSValue handleExt;
        QMetaObject::invokeMethod(m_loader->m_extFactoryDict[name], "create", Q_RETURN_ARG(QJSValue, handleExt), Q_ARG(QObject *, win));
        return handleExt;
    }
} // Internal