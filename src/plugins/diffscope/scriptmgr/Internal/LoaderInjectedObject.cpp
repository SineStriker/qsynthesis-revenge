#include "LoaderInjectedObject.h"

#include <QUuid>
#include "ScriptLoader.h"

namespace ScriptMgr::Internal {
    void LoaderInjectedObject::registerScript(const QString &id, const QStringList &role, const QString &shortcut) const {
        m_loader->m_scriptEntries.append({ id, ScriptEntry::Script, role, shortcut});
    }
    void LoaderInjectedObject::registerScriptSet(const QString &id, const QStringList &childrenId, const QStringList &role, const QStringList &childrenShortcut) const {
        m_loader->m_scriptEntries.append({id, ScriptEntry::ScriptSet, role, {}, childrenId, childrenShortcut});
    }
    QString LoaderInjectedObject::uuid() const {
        return QUuid::createUuid().toString();
    }
    LoaderInjectedObject::LoaderInjectedObject(ScriptLoader *loader): m_loader(loader) {
    }
} // Internal