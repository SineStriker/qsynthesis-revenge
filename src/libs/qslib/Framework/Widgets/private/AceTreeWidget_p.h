#ifndef ADDITIVETREEWIDGETPRIVATE_H
#define ADDITIVETREEWIDGETPRIVATE_H

#include <QHash>

#include "AceTreeWidget.h"

namespace QsApi {

    class AceTreeWidgetPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(AceTreeWidget)
    public:
        AceTreeWidgetPrivate();
        virtual ~AceTreeWidgetPrivate();

        void init();

        AceTreeWidget *q_ptr;
        AceTreeModel *m_model;
        QHash<AceTreeItem *, QTreeWidgetItem *> itemIndexes;

        QTreeWidgetItem *buildItem(AceTreeItem *item);
        void removeItem(AceTreeItem *item);

    private:
        void _q_propertyChanged(AceTreeItem *item, const QString &key, const QVariant &oldValue,
                                const QVariant &newValue);

        void _q_bytesSet(AceTreeItem *item, int start, const QByteArray &oldBytes, const QByteArray &newBytes);
        void _q_bytesTruncated(AceTreeItem *item, int size, const QByteArray &oldBytes, int delta);

        void _q_rowsInserted(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);
        void _q_rowsMoved(AceTreeItem *parent, int index, int count, int dest);
        void _q_rowsAboutToRemove(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);
        void _q_rowsRemoved(AceTreeItem *parent, int index, int count);

        void _q_nodeAdded(AceTreeItem *parent, AceTreeItem *item);
        void _q_nodeAboutToRemove(AceTreeItem *parent, AceTreeItem *item);
        void _q_nodeRemoved(AceTreeItem *parent, AceTreeItem *item);

        void _q_rootAboutToChange(AceTreeItem *oldRoot, AceTreeItem *newRoot);
        void _q_rootChanged();
    };

}

#endif // ADDITIVETREEWIDGETPRIVATE_H