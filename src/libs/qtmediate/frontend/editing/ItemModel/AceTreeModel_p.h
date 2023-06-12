#ifndef ACETREEMODEL_P_H
#define ACETREEMODEL_P_H

#include <QFileDevice>
#include <QSet>
#include <QStack>

#include "AceTreeModel.h"

class AceTreeItemPrivate {
    Q_DECLARE_PUBLIC(AceTreeItem)
public:
    AceTreeItemPrivate();
    virtual ~AceTreeItemPrivate();

    void init();

    AceTreeItem *q_ptr;
    QString name;
    QString serializerId;
    QHash<QString, QVariant> dynamicData;

    AceTreeItem *parent;
    AceTreeModel *model;
    int m_index;
    int m_indexHint; // Only for deserialization

    QHash<QString, QVariant> properties;
    QByteArray byteArray;
    QVector<AceTreeItem *> vector;

    QSet<AceTreeItem *> set;
    QHash<QString, QSet<AceTreeItem *>> setNameIndexes;

    bool allowModify() const;

    void setProperty_helper(const QString &key, const QVariant &value);
    void setBytes_helper(int start, const QByteArray &bytes);
    void truncateBytes_helper(int size);
    void insertRows_helper(int index, const QVector<AceTreeItem *> &items);
    void moveRows_helper(int index, int count, int dest);
    void removeRows_helper(int index, int count);
    void addNode_helper(AceTreeItem *item);
    void removeNode_helper(AceTreeItem *item);

    template <class Func>
    static void propagateItems(AceTreeItem *item, Func f) {
        QStack<AceTreeItem *> stack;
        stack.push(item);
        while (!stack.isEmpty()) {
            auto top = stack.pop();
            auto d = top->d_func();
            f(top);

            for (const auto &child : qAsConst(d->vector))
                stack.push(child);
            for (const auto &child : qAsConst(d->set))
                stack.push(child);
        }
    }
};

class AceTreeModelPrivate {
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
        BytesTruncate,
        RowsInsert,
        RowsMove,
        RowsRemove,
        NodeAdd,
        NodeRemove,
        RootChange,
    };

    struct BaseOp {
        Change c;
        explicit BaseOp(Change c) : c(c){};
        virtual ~BaseOp() = default;
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

    struct BytesTruncateOp : public BaseOp {
        int id;
        int size;
        QByteArray oldBytes;
        int delta;
        BytesTruncateOp() : BaseOp(BytesTruncate), id(0), size(0), delta(0) {
        }
    };

    struct RowsInsertRemoveOp : public BaseOp {
        int id;
        int index;
        QVector<AceTreeItem *> items;
        QByteArrayList serialized;
        RowsInsertRemoveOp(bool isInsert = false) : BaseOp(isInsert ? RowsInsert : RowsRemove), id(0), index(0) {
        }
        ~RowsInsertRemoveOp() {
            if (c == RowsInsert)
                for (const auto &item : qAsConst(items))
                    if (item && !item->model() && !item->parent())
                        delete item;
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

    struct NodeAddRemoveOp : public BaseOp {
        int id;
        AceTreeItem *item;
        QByteArray serialized;
        NodeAddRemoveOp(bool isInsert = false) : BaseOp(isInsert ? NodeAdd : NodeRemove), id(0), item(nullptr) {
        }
        ~NodeAddRemoveOp() {
            if (c == NodeAdd)
                if (item && !item->model() && !item->parent())
                    delete item;
        }
    };

    struct RootChangeOp : public BaseOp {
        AceTreeItem *oldRoot;
        AceTreeItem *newRoot;
        RootChangeOp() : BaseOp(RootChange), oldRoot(nullptr), newRoot(nullptr) {
        }
        ~RootChangeOp() {
            // if (oldRoot && !oldRoot->model())
            //    delete oldRoot;
            if (newRoot && !newRoot->model())
                delete newRoot;
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
    void propertyChanged(AceTreeItem *item, const QString &key, const QVariant &oldValue, const QVariant &newValue);

    void bytesSet(AceTreeItem *item, int start, const QByteArray &oldBytes, const QByteArray &newBytes);
    void bytesTruncated(AceTreeItem *item, int size, const QByteArray &oldBytes, int delta);

    void rowsInserted(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);
    void rowsMoved(AceTreeItem *parent, int index, int count, int dest);
    void rowsRemoved(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);

    void nodeAdded(AceTreeItem *parent, AceTreeItem *item);
    void nodeRemoved(AceTreeItem *parent, AceTreeItem *item);

    void rootChanged(AceTreeItem *oldRoot, AceTreeItem *newRoot);
};

#endif // ACETREEMODEL_P_H
