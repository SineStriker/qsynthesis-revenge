#include "JsLoaderObject.h"

#include <QUuid>

#include "QMDecoratorV2.h"

#include "JsUtils.h"
#include "ScriptLoader.h"


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
    QString JsLoaderObject::coreMsgBox(const QString &title, const QString &message, const QString &icon, const QStringList &buttons, const QString &defaultButton) {
        return jsMsgBox(nullptr, title, message, icon, buttons, defaultButton);
    }
} // Internal