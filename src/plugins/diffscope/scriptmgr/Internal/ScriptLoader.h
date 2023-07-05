#ifndef CHORUSKIT_SCRIPTLOADER_H
#define CHORUSKIT_SCRIPTLOADER_H

#include <QJSEngine>

namespace ScriptMgr::Internal {

    class ScriptMgrPlugin;
    class JsLoaderObject;
    class JsInternalObject;
    class ScriptSettingsPage;

    struct ScriptEntry {
        enum Type { Script, ScriptSet };
        QString id;
        Type type;
        QStringList role;
        QString shortcut;
        QStringList childrenId;
        QStringList childrenShortcut;
    };

    class ScriptLoader: public QObject {
        Q_OBJECT
    public:
        static ScriptLoader *instance();

        bool reloadEngine();

        [[nodiscard]] QString createHandles(JsInternalObject *internalObject) const;

        void invoke(const QString &windowKey, const QString &id) const;
        void invoke(const QString &windowKey, const QString &id, int index) const;

        QString getName(const QString &id) const;

        QJSValue getInfo(const QString &id) const;

        void removeHandles(const QString &windowKey) const;

        QList<ScriptEntry> builtInScriptEntries() const;
        QList<ScriptEntry> scriptEntries() const;

        static QString userScriptDir();

        QJSEngine *engine() const;

        QString version() const;

        QMap<QString, QKeySequence> cachedCustomShortcuts() const;

    signals:
        void engineWillReload();
        void engineReloaded();
        void scriptNameReloaded();
    private:
        friend class ScriptMgrPlugin;
        friend class JsLoaderObject;
        friend class ScriptSettingsPage;

        ScriptLoader(QObject *parent = nullptr);
        ~ScriptLoader();

        QString getNameFromJs(const QString &id) const;
        QString getNameFromJs(const QString &id, const QString &childId, int index) const;
        void reloadStrings();

        bool loadFile(const QString &path);

        void setVersion(const QString &version);

        static void alertJsUncaughtError(const QJSValue &error);
        static void warningCannotLoadFile(const QString &path);
        static void warningCannotGetName(const QString &id);
        static void criticalCannotInitializeEngine();
        static void criticalScriptExecutionFailed(const QString &name);

        struct EngineDeleter {
            static inline void cleanup(QJSEngine *pointer) { if (pointer) { pointer->setInterrupted(true); pointer->deleteLater(); } }
        };

        QScopedPointer<QJSEngine, EngineDeleter> m_engine;
        QJSValue m_jsCallbacks;
        QList<ScriptEntry> m_builtInScriptEntries;
        QList<ScriptEntry> m_scriptEntries;
        QString m_version;
        QMap<QString, QString> scriptNameDict;

        QMap<QString, QKeySequence> m_cachedCustomShortcuts;
    };

} // Internal

#endif // CHORUSKIT_SCRIPTLOADER_H
