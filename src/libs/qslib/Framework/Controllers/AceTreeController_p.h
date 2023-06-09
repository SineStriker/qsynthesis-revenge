#ifndef ACETREECONTROLLERPRIVATE_H
#define ACETREECONTROLLERPRIVATE_H

#include <QHash>

#include "AceTreeController.h"

namespace QsApi {

    class QSFRAMEWORK_API AceTreeControllerPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(AceTreeController)
    public:
        AceTreeControllerPrivate();
        virtual ~AceTreeControllerPrivate();

        void init();

        AceTreeController *q_ptr;

        QString name;
        AceTreeItem *treeItem;
        AceTreeControllerBuilder *builder;
        bool operational;

        QHash<QString, AceTreeController *> children;
    };

}

#endif // ACETREECONTROLLERPRIVATE_H