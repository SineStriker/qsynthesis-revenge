#ifndef ADDITIVETREEMODEL_H
#define ADDITIVETREEMODEL_H

#include <QFile>
#include <QObject>
#include <QVariant>

#include "QsSVSGlobal.h"

namespace QsApi {

    class AdditiveTreeItemPrivate;

    class QSSVS_API AdditiveTreeItem {
        Q_DECLARE_PRIVATE(AdditiveTreeItem)
    public:
        explicit AdditiveTreeItem(const QString &name);
        virtual ~AdditiveTreeItem();

        friend class AdditiveTreeModel;
        friend class AdditiveTreeModelPrivate;

    public:
        QString name() const;

        AdditiveTreeItem *parent() const;
        AdditiveTreeModel *model() const;
        int index() const;

        // Properties
        QVariant property(const QString &key) const;
        void setProperty(const QString &key, const QVariant &value);
        QVariantHash properties() const;

        // ByteArray
        void setBytes(int start, int len, const QByteArray &bytes);
        void truncateBytes(int from);
        QByteArray bytes() const;
        int bytesSize() const;

        // Vector
        void insertRows(int index, const QList<AdditiveTreeItem *> &items);
        void moveRows(int index, int count, int dest);
        QList<AdditiveTreeItem *> takeRows(int index, int count);
        void removeRows(int index, int count);
        AdditiveTreeItem *itemAtRow(int row) const;

        // Map
        void addUnique(AdditiveTreeItem *item);
        bool containsUnique(AdditiveTreeItem *item);
        void takeUnique(AdditiveTreeItem *item);
        void removeUnique(AdditiveTreeItem *item);
        QList<AdditiveTreeItem *> uniqueItems() const;

    public:
        virtual void read(QDataStream &in);
        virtual void write(QDataStream &out) const;

    protected:
        void propagateModel(AdditiveTreeModel *model);

        AdditiveTreeItem(AdditiveTreeItemPrivate &d, const QString &name);

        QScopedPointer<AdditiveTreeItemPrivate> d_ptr;
    };

    class AdditiveTreeModelPrivate;

    class QSSVS_API AdditiveTreeModel : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AdditiveTreeModel)
    public:
        explicit AdditiveTreeModel(QObject *parent = nullptr);
        ~AdditiveTreeModel();

        friend class AdditiveTreeItem;
        friend class AdditiveTreeItemPrivate;

    public:
        void record(QIODevice *dev);
        static AdditiveTreeModel *recover(QIODevice *dev);

    public:
        void beginTransaction();
        void endTransaction(const QVariant &desc);

        bool canUndo() const;
        bool canRedo() const;

        void undo();
        void redo();

        QVariant undoDesc() const;
        QVariant redoDesc() const;

    public:
        QString indexFromItem(AdditiveTreeItem *item) const;
        AdditiveTreeItem *itemFromIndex(int index) const;

        AdditiveTreeItem *rootItem() const;
        void setRootItem(AdditiveTreeItem *item);

    signals:
        void propertyChanged(AdditiveTreeItem *item, const QString &key, const QVariant &oldValue,
                             const QVariant &newValue);

        void bytesSet(AdditiveTreeItem *item, int start, int len, const QByteArray &oldBytes,
                      const QByteArray &newBytes);
        void bytesTruncated(AdditiveTreeItem *item, int from, const QByteArray &oldBytes);

        void rowInserted(AdditiveTreeItem *parent, int index, const QList<AdditiveTreeItem *> &items);
        void rowMoved(AdditiveTreeItem *parent, int index, int count, int dest);
        void rowRemoved(AdditiveTreeItem *parent, int index, const QList<AdditiveTreeItem *> &items);

        void uniqueAdded(AdditiveTreeItem *parent, AdditiveTreeItem *item);
        void uniqueRemoved(AdditiveTreeItem *parent, AdditiveTreeItem *item);

    protected:
        AdditiveTreeModel(AdditiveTreeModelPrivate &d, QObject *parent = nullptr);

        QScopedPointer<AdditiveTreeModelPrivate> d_ptr;
    };

}

#endif // ADDITIVETREEMODEL_H