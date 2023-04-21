#ifndef ADDITIVETREEMODEL_H
#define ADDITIVETREEMODEL_H

#include <QFile>
#include <QObject>
#include <QVariant>

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class AdditiveTreeItemPrivate;

    class QSFRAMEWORK_API AdditiveTreeItem {
        Q_DECLARE_PRIVATE(AdditiveTreeItem)
    public:
        explicit AdditiveTreeItem(const QString &name);
        virtual ~AdditiveTreeItem();

        friend class AdditiveTreeModel;
        friend class AdditiveTreeModelPrivate;

    public:
        QString name() const;

        QJsonObject workspaceData() const;

        AdditiveTreeItem *parent() const;
        AdditiveTreeModel *model() const;
        int index() const;

        // Properties
        QVariant property(const QString &key) const;
        void setProperty(const QString &key, const QVariant &value);
        QVariantHash properties() const;

        // ByteArray
        void setBytes(int start, const QByteArray &bytes);
        void truncateBytes(int size);
        QByteArray bytes() const;
        int bytesSize() const;

        // Vector
        void insertRows(int index, const QVector<AdditiveTreeItem *> &items);
        void moveRows(int index, int count, int dest);
        void removeRows(int index, int count);
        AdditiveTreeItem *itemAtRow(int row) const;

        // Map
        void addUnique(AdditiveTreeItem *item);
        bool containsUnique(AdditiveTreeItem *item);
        void removeUnique(AdditiveTreeItem *item);
        QList<AdditiveTreeItem *> uniqueItems() const;

    public:
        static AdditiveTreeItem *read(QDataStream &in);

        void write(QDataStream &out) const;

        AdditiveTreeItem *clone() const;

    protected:
        void propagateModel(AdditiveTreeModel *model);

        AdditiveTreeItem(AdditiveTreeItemPrivate &d, const QString &name);

        QScopedPointer<AdditiveTreeItemPrivate> d_ptr;
    };

    class AdditiveTreeModelPrivate;

    class QSFRAMEWORK_API AdditiveTreeModel : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AdditiveTreeModel)
    public:
        explicit AdditiveTreeModel(QObject *parent = nullptr);
        ~AdditiveTreeModel();

        friend class AdditiveTreeItem;
        friend class AdditiveTreeItemPrivate;
        friend class AdditiveTreeTransaction;

    public:
        AdditiveTreeItem *itemFromIndex(int index) const;

        AdditiveTreeItem *rootItem() const;
        void setRootItem(AdditiveTreeItem *item);

        AdditiveTreeTransaction *transaction() const;

    signals:
        void propertyChanged(AdditiveTreeItem *item, const QString &key, const QVariant &oldValue,
                             const QVariant &newValue);

        void bytesSet(AdditiveTreeItem *item, int start, const QByteArray &oldBytes, const QByteArray &newBytes);
        void bytesTruncated(AdditiveTreeItem *item, int size, const QByteArray &oldBytes, int delta);

        void rowsInserted(AdditiveTreeItem *parent, int index, const QVector<AdditiveTreeItem *> &items);
        void rowsMoved(AdditiveTreeItem *parent, int index, int count, int dest);
        void rowsAboutToRemove(AdditiveTreeItem *parent, int index, const QVector<AdditiveTreeItem *> &items);
        void rowsRemoved(AdditiveTreeItem *parent, int index, int count);

        void uniqueAdded(AdditiveTreeItem *parent, AdditiveTreeItem *item);
        void uniqueAboutToRemove(AdditiveTreeItem *parent, AdditiveTreeItem *item);
        void uniqueRemoved(AdditiveTreeItem *parent, AdditiveTreeItem *item);

        void rootAboutToChange(AdditiveTreeItem *oldRoot, AdditiveTreeItem *newRoot);
        void rootChanged();

    protected:
        AdditiveTreeModel(AdditiveTreeModelPrivate &d, QObject *parent = nullptr);

        QScopedPointer<AdditiveTreeModelPrivate> d_ptr;
    };

}

#endif // ADDITIVETREEMODEL_H