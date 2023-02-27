#ifndef CMENUBUILDER_P_H
#define CMENUBUILDER_P_H

#include "../CMenuBuilder.h"

#include <QAction>
#include <QMenu>

class CMenuBuilderPrivate {
    Q_DECLARE_PUBLIC(CMenuBuilder)
public:
    CMenuBuilderPrivate();
    virtual ~CMenuBuilderPrivate();

    void init();

    struct ActionSpec {
        QString id;
        bool separator;
        QList<QSharedPointer<ActionSpec>> children;

        ActionSpec() : separator(false){};
        ActionSpec(const QString &id, bool separator = false) : id(id), separator(separator){};
    };

    using ActionSpecRef = ActionSpec;

    struct Data {
        QString ns;
        QMap<QString, ActionSpec> menus;
    };

    Data data;

    CMenuBuilder *q_ptr;
};

#endif // CMENUBUILDER_P_H
