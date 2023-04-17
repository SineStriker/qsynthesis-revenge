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
        ~ScriptLoader();
        void registerAddon(ScriptMgrAddOn *addon);
        static ScriptLoader *instance();
        QString userScriptDir();
    public slots:
        void reloadScripts();
    protected:
        friend class BatchProcess;
        explicit ScriptLoader(QObject *parent);
        QSet<ScriptMgrAddOn *> addonRegistry;
    };

} // Internal

#endif // CHORUSKIT_SCRIPTLOADER_H
