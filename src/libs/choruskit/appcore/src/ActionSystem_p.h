#ifndef ACTIONSYSTEM_P_H
#define ACTIONSYSTEM_P_H

#include "ActionSystem.h"
#include "Collections/QMChronMap.h"

#include <Serialization/QMXmlAdaptor.h>

namespace Core {

    class ActionSystemPrivate {
        Q_DECLARE_PUBLIC(ActionSystem)
    public:
        ActionSystemPrivate();
        virtual ~ActionSystemPrivate();

        void init();

        void readSettings();
        void saveSettings() const;

        void loadContexts_dfs(const QString &prefix, const QString &parentId, const QMXmlAdaptorElement *ele,
                              ActionContext *context);

        ActionSystem *q_ptr;

        QMChronMap<QString, ActionSpec *> actions;
        QMChronMap<QString, ActionContext *> contexts;

        QHash<QString, QMap<QString, QStringList>> stateCaches;
    };
}

#endif // ACTIONSYSTEM_P_H
