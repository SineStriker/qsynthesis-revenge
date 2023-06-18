#ifndef ACETREEMODEL_P_H
#define ACETREEMODEL_P_H

#include <QDebug>
#include <QFileDevice>
#include <QSet>
#include <QStack>

#include <QMChronSet.h>

#include "AceTreeModel.h"

class AceTreeItemPrivate {
    Q_DECLARE_PUBLIC(AceTreeItem)
public:
    AceTreeItemPrivate();
    virtual ~AceTreeItemPrivate();

    void init();

    AceTreeItem *q_ptr;
    QString name;
    AceTreeItem::Status status;
    QHash<QString, QVariant> dynamicData;
    QMChronSet<AceTreeItemSubscriber *> subscribers;

    AceTreeItem *parent;
    AceTreeModel *model;
    int m_index;
    int m_indexHint; // Only for deserialization
    bool m_forceDelete;

    QHash<QString, QVariant> properties;
    QByteArray byteArray;
    QVector<AceTreeItem *> vector;

    QHash<int, AceTreeItem *> records;
    QHash<AceTreeItem *, int> recordIndexes;
    int maxRecordSeq;

    QSet<AceTreeItem *> set;
    QHash<QString, QSet<AceTreeItem *>> setNameIndexes;

    bool testModifiable(const char *func) const;
    bool testInsertable(const char *func, const AceTreeItem *item) const;

    void setProperty_helper(const QString &key, const QVariant &value);
    void setBytes_helper(int start, const QByteArray &bytes);
    void insertBytes_helper(int start, const QByteArray &bytes);
    void removeBytes_helper(int start, int size);
    void insertRows_helper(int index, const QVector<AceTreeItem *> &items);
    void moveRows_helper(int index, int count, int dest);
    void removeRows_helper(int index, int count);
    void addNode_helper(AceTreeItem *item);
    void removeNode_helper(AceTreeItem *item);

    void addRecord_helper(int seq, AceTreeItem *item);
    void removeRecord_helper(int seq);

    static void propagateItems(AceTreeItem *item, const std::function<void(AceTreeItem *)> &f);
    static QByteArray itemToData(AceTreeItem *item);
    static AceTreeItem *itemFromStream(QDataStream &stream, QByteArray *out);

    static void enterModel(AceTreeItem *item, AceTreeModel *model);
    static void leaveModel(AceTreeItem *item);

public:
    static void forceDelete(AceTreeItem *item);
};

class AceTreeModelPrivate {
    Q_GADGET
    Q_DECLARE_PUBLIC(AceTreeModel)
public:
    AceTreeModelPrivate();
    virtual ~AceTreeModelPrivate();

    void init();

    AceTreeModel *q_ptr;

    QIODevice *m_dev;
    QFileDevice *m_fileDev;

    bool is_destruct;

    QHash<int, AceTreeItem *> indexes;
    int maxIndex;

    AceTreeItem *rootItem;

    void setRootItem_helper(AceTreeItem *item);
    AceTreeItem *reset_helper();
    void setCurrentStep_helper(int step);

    int addIndex(AceTreeItem *item, int idx = -1);
    void removeIndex(int index);

    // Operations
    enum Change {
        PropertyChange = 0xC1,
        BytesSet,
        BytesInsert,
        BytesRemove,
        RowsInsert,
        RowsMove,
        RowsRemove,
        RecordAdd,
        RecordRemove,
        NodeAdd,
        NodeRemove,
        RootChange,
    };

    Q_ENUM(Change)

    struct BaseOp {
        Change c;
        explicit BaseOp(Change c) : c(c){};
        virtual ~BaseOp();
    };

    struct PropertyChangeOp : public BaseOp {
        int id;
        QString key;
        QVariant oldValue;
        QVariant newValue;
        PropertyChangeOp() : BaseOp(PropertyChange), id(0) {
        }
    };

    struct BytesSetOp : public BaseOp {
        int id;
        int start;
        QByteArray oldBytes;
        QByteArray newBytes;
        BytesSetOp() : BaseOp(BytesSet), id(0), start(0) {
        }
    };

    struct BytesInsertRemoveOp : public BaseOp {
        int id;
        int start;
        QByteArray bytes;
        BytesInsertRemoveOp(bool isInsert = false) : BaseOp(isInsert ? BytesInsert : BytesRemove), id(0), start(0) {
        }
    };

    struct RowsInsertRemoveOp : public BaseOp {
        int id;
        int index;
        QVector<AceTreeItem *> items;
        QByteArrayList serialized; // Insert operation needs
        RowsInsertRemoveOp(bool isInsert = false) : BaseOp(isInsert ? RowsInsert : RowsRemove), id(0), index(0) {
        }
        ~RowsInsertRemoveOp() {
            if (c == RowsInsert)
                for (const auto &item : qAsConst(items))
                    if (item && item->isManagedRoot())
                        AceTreeItemPrivate::forceDelete(item);

            // If this is a remove operation, there must be an insert operation ahead
            // which holds the reference of `item`, we don't need to delete
        }
    };

    struct RowsMoveOp : public BaseOp {
        int id;
        int index;
        int count;
        int dest;
        RowsMoveOp() : BaseOp(RowsMove), id(0), index(0), count(0), dest(0) {
        }
    };

    struct RecordAddRemoveOp : public BaseOp {
        int id;
        int seq;
        AceTreeItem *item;
        QByteArray serialized; // Insert operation needs
        RecordAddRemoveOp(bool isInsert) : BaseOp(isInsert ? RecordAdd : RecordRemove), id(0), seq(-1), item(nullptr) {
        }
        ~RecordAddRemoveOp() {
            if (c == RecordAdd)
                if (item && item->isManagedRoot())
                    AceTreeItemPrivate::forceDelete(item);

            // If this is a remove operation, there must be an insert operation ahead
            // which holds the reference of `item`, we don't need to delete
        }
    };

    struct NodeAddRemoveOp : public BaseOp {
        int id;
        AceTreeItem *item;
        QByteArray serialized; // Insert operation needs
        NodeAddRemoveOp(bool isInsert = false) : BaseOp(isInsert ? NodeAdd : NodeRemove), id(0), item(nullptr) {
        }
        ~NodeAddRemoveOp() {
            if (c == NodeAdd)
                if (item && item->isManagedRoot())
                    AceTreeItemPrivate::forceDelete(item);

            // If this is a remove operation, there must be an insert operation ahead
            // which holds the reference of `item`, we don't need to delete
        }
    };

    struct RootChangeOp : public BaseOp {
        AceTreeItem *oldRoot;
        AceTreeItem *newRoot;
        QByteArray serialized;
        RootChangeOp() : BaseOp(RootChange), oldRoot(nullptr), newRoot(nullptr) {
        }
        ~RootChangeOp() {
            if (newRoot && newRoot->isManagedRoot())
                AceTreeItemPrivate::forceDelete(newRoot);

            // The `oldRoot` must be hold by the previous `RootChangeOp`
            // we don't need to delete
        }
    };

    int stepIndex;
    QVector<BaseOp *> operations;
    bool internalChange;

    struct Offsets {
        qint64 startPos;
        qint64 countPos;
        qint64 dataPos;
        QVector<qint64> begs;
        Offsets() : startPos(0), countPos(0), dataPos(0) {
        }
    };
    Offsets offsets;

    bool execute(BaseOp *baseOp, bool undo);
    void push(BaseOp *op);
    void truncate(int step);

    void writeCurrentStep(int step);

    static void serializeOperation(QDataStream &stream, BaseOp *baseOp);
    static BaseOp *deserializeOperation(QDataStream &stream, QList<int> *ids);

public:
    void propertyChanged(AceTreeItem *item, const QString &key, const QVariant &newValue, const QVariant &oldValue);

    void bytesSet(AceTreeItem *item, int start, const QByteArray &newBytes, const QByteArray &oldBytes);
    void bytesInserted(AceTreeItem *item, int start, const QByteArray &bytes);
    void bytesRemoved(AceTreeItem *item, int start, const QByteArray &bytes);

    void rowsInserted(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);
    void rowsMoved(AceTreeItem *parent, int index, int count, int dest);
    void rowsRemoved(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);

    void recordAdded(AceTreeItem *parent, int seq, AceTreeItem *item);
    void recordRemoved(AceTreeItem *parent, int seq, AceTreeItem *item);

    void nodeAdded(AceTreeItem *parent, AceTreeItem *item);
    void nodeRemoved(AceTreeItem *parent, AceTreeItem *item);

    void rootChanged(AceTreeItem *newRoot, AceTreeItem *oldRoot);
};

class AceTreeItemSubscriberPrivate {
public:
    AceTreeItemSubscriberPrivate(AceTreeItemSubscriber *q);
    ~AceTreeItemSubscriberPrivate();

    AceTreeItemSubscriber *q;
    AceTreeItem *m_treeItem;
};

#endif // ACETREEMODEL_P_H
