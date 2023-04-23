#ifndef CHORUSKIT_SCRIPTLOADER_H
#define CHORUSKIT_SCRIPTLOADER_H

#include <QSet>

#include "AddOn/ScriptMgrAddOn.h"

namespace ScriptMgr::Internal {

    class ScriptLoader: public QObject {
        Q_OBJECT
    public:
        static ScriptLoader *instance();

        void registerAddon(ScriptMgrAddOn *addon);
        QString userScriptDir() const;

    public slots:
        void reloadScripts();

    protected:
        explicit ScriptLoader(QObject *parent);
        ~ScriptLoader();

        QSet<ScriptMgrAddOn *> addonRegistry;

        friend class ScriptMgrPlugin;
    };

} // Internal

#endif // CHORUSKIT_SCRIPTLOADER_H
