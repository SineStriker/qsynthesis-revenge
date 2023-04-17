//
// Created by Crs_1 on 2023/4/17.
//

#ifndef CHORUSKIT_SCRIPTLOADER_H
#define CHORUSKIT_SCRIPTLOADER_H

#include "AddOn/ScriptMgrAddOn.h"
#include "Collections/QMChronMap.h"
#include "Collections/QMChronSet.h"
#include <QSet>

namespace ScriptMgr::Internal {

    class ScriptLoader: public QObject {
        Q_OBJECT
    public:
        static ScriptLoader *instance();

        void registerAddon(ScriptMgrAddOn *addon);
        QString userScriptDir();

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
