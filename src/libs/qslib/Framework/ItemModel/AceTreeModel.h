#ifndef ACETREEMODEL_H
#define ACETREEMODEL_H

#include <QIODevice>
#include <QObject>
#include <QVariant>

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class AceTreeModel;

    class AceTreeItemPrivate;

    class QSFRAMEWORK_API AceTreeItem {
        Q_DECLARE_PRIVATE(AceTreeItem)
    public:
        explicit AceTreeItem(const QString &name);
        virtual ~AceTreeItem();

    public:
        QString name() const;

        AceTreeItem *parent() const;
        AceTreeModel *model() const;
        int index() const;

        bool isFree() const;
        bool isObsolete() const;
        bool isWritable() const;

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
        inline void prependRow(AceTreeItem *item);
        inline void prependRows(const QVector<AceTreeItem *> &items);
        inline void appendRow(AceTreeItem *item);
        inline void appendRows(const QVector<AceTreeItem *> &items);
        inline void insertRow(int index, AceTreeItem *item);
        inline void removeRow(int index);
        void insertRows(int index, const QVector<AceTreeItem *> &items);
        void moveRows(int index, int count, int dest);         // `dest`: destination index before move
        inline void moveRows2(int index, int count, int dest); // `dest`: destination index after move
        void removeRows(int index, int count);
        AceTreeItem *row(int row) const;
        int rowIndexOf(AceTreeItem *item) const;
        int rowCount() const;

        // Set
        inline void insertNode(AceTreeItem *item);
        void addNode(AceTreeItem *item);
        bool containsNode(AceTreeItem *item);
        void removeNode(AceTreeItem *item);
        QList<AceTreeItem *> nodes() const;
        int nodeCount() const;

    public:
        static AceTreeItem *read(QDataStream &in);
        void write(QDataStream &out) const;
        AceTreeItem *clone() const;

    protected:
        void propagateModel(AceTreeModel *model);

        AceTreeItem(AceTreeItemPrivate &d, const QString &name);

        QScopedPointer<AceTreeItemPrivate> d_ptr;

        friend class AceTreeModel;
        friend class AceTreeModelPrivate;
    };

    class AceTreeModelPrivate;

    class QSFRAMEWORK_API AceTreeModel : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AceTreeModel)
    public:
        explicit AceTreeModel(QObject *parent = nullptr);
        ~AceTreeModel();

    public:
        int steps() const;
        int currentStep() const;
        void setCurrentStep(int step);
        inline void nextStep();
        inline void previousStep();

        bool isWritable() const;

        void startRecord(QIODevice *dev);
        void stopRecord();
        static AceTreeModel *recover(const QByteArray &data);

        AceTreeItem *itemFromIndex(int index) const;

        AceTreeItem *rootItem() const;
        void setRootItem(AceTreeItem *item);

        AceTreeItem *reset();

    signals:
        void propertyChanged(AceTreeItem *item, const QString &key, const QVariant &oldValue, const QVariant &newValue);

        void bytesSet(AceTreeItem *item, int start, const QByteArray &oldBytes, const QByteArray &newBytes);
        void bytesTruncated(AceTreeItem *item, int size, const QByteArray &oldBytes, int delta);

        void rowsInserted(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);
        void rowsMoved(AceTreeItem *parent, int index, int count, int dest);
        void rowsAboutToRemove(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);
        void rowsRemoved(AceTreeItem *parent, int index, int count);

        void nodeAdded(AceTreeItem *parent, AceTreeItem *item);
        void nodeAboutToRemove(AceTreeItem *parent, AceTreeItem *item);
        void nodeRemoved(AceTreeItem *parent, AceTreeItem *item);

        void rootAboutToChange(AceTreeItem *oldRoot, AceTreeItem *newRoot);
        void rootChanged();

        void stepChanged(int step);
        void recordError();

    protected:
        AceTreeModel(AceTreeModelPrivate &d, QObject *parent = nullptr);

        QScopedPointer<AceTreeModelPrivate> d_ptr;

        friend class AceTreeItem;
        friend class AceTreeItemPrivate;
    };

    inline void AceTreeItem::prependRow(AceTreeItem *item) {
        insertRows(0, {item});
    }

    inline void AceTreeItem::prependRows(const QVector<AceTreeItem *> &items) {
        insertRows(0, items);
    }

    inline void AceTreeItem::appendRow(AceTreeItem *item) {
        insertRows(rowCount(), {item});
    }

    inline void AceTreeItem::appendRows(const QVector<AceTreeItem *> &items) {
        insertRows(rowCount(), items);
    }

    inline void AceTreeItem::insertRow(int index, AceTreeItem *item) {
        insertRows(index, {item});
    }

    inline void AceTreeItem::moveRows2(int index, int count, int dest) {
        return moveRows(index, count, (dest <= index) ? dest : (dest + count));
    }

    inline void AceTreeItem::removeRow(int index) {
        removeRows(index, 1);
    }

    inline void AceTreeItem::insertNode(AceTreeItem *item) {
        addNode(item);
    }

    inline void AceTreeModel::nextStep() {
        setCurrentStep(currentStep() + 1);
    }

    inline void AceTreeModel::previousStep() {
        setCurrentStep(currentStep() - 1);
    }

}

#endif // ACETREEMODEL_H