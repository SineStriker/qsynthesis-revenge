#ifndef ACETREEMODEL_H
#define ACETREEMODEL_H

#include <QIODevice>
#include <QObject>
#include <QVariant>

#include "QMEditingGlobal.h"

class AceTreeModel;

class AceTreeItemSubscriber;

class AceTreeItemPrivate;

class QMEDITING_EXPORT AceTreeItem {
    Q_DECLARE_PRIVATE(AceTreeItem)
public:
    explicit AceTreeItem(const QString &name);
    virtual ~AceTreeItem();

    enum Status : qint8 {
        Root,        // Set as root or newly created
        Row,         // Added to vector
        Record,      // Added to record table
        Node,        // Added to set
        ManagedRoot, // Removed from an item on a model
    };

    inline bool isRoot() const;
    inline bool isRow() const;
    inline bool isRecord() const;
    inline bool isNode() const;
    inline bool isManagedRoot() const;

    QString name() const;
    Status status() const;

    bool addSubscriber(AceTreeItemSubscriber *sub);
    bool removeSubscriber(AceTreeItemSubscriber *sub);
    QList<AceTreeItemSubscriber *> subscribers() const;
    int subscriberCount() const;
    bool hasSubscriber(AceTreeItemSubscriber *sub) const;

    QVariant dynamicData(const QString &key) const;
    void setDynamicData(const QString &key, const QVariant &value);
    inline void clearDynamicData(const QString &key);
    QStringList dynamicDataKeys() const;
    QVariantHash dynamicDataMap() const;

public:
    AceTreeItem *parent() const;
    AceTreeModel *model() const;
    int index() const;

    bool isFree() const;
    bool isObsolete() const;
    bool isWritable() const;

    // Properties
    QVariant property(const QString &key) const;
    bool setProperty(const QString &key, const QVariant &value);
    inline bool clearProperty(const QString &key);
    QStringList propertyKeys() const;
    QVariantHash propertyMap() const;
    inline QVariantHash properties() const;

    // ByteArray
    void setBytes(int start, const QByteArray &bytes);
    void truncateBytes(int size);
    QByteArray bytes() const;
    QByteArray midBytes(int start, int len) const;
    int bytesSize() const;

    // Vector - Rows
    inline bool prependRow(AceTreeItem *item);
    inline bool prependRows(const QVector<AceTreeItem *> &items);
    inline bool appendRow(AceTreeItem *item);
    inline bool appendRows(const QVector<AceTreeItem *> &items);
    inline bool insertRow(int index, AceTreeItem *item);
    inline bool removeRow(int index);
    bool insertRows(int index, const QVector<AceTreeItem *> &items);
    bool moveRows(int index, int count, int dest);         // `dest`: destination index before move
    inline bool moveRows2(int index, int count, int dest); // `dest`: destination index after move
    bool removeRows(int index, int count);
    AceTreeItem *row(int index) const;
    QVector<AceTreeItem *> rows() const;
    int rowIndexOf(AceTreeItem *item) const;
    int rowCount() const;

    // Sheet - Records
    int addRecord(AceTreeItem *item);
    bool removeRecord(int seq);
    bool removeRecord(AceTreeItem *item);
    AceTreeItem *record(int seq);
    int recordIndexOf(AceTreeItem *item) const;
    QList<int> records() const;
    int recordCount() const;
    int maxRecordSeq() const;

    // Set - Nodes
    inline bool insertNode(AceTreeItem *item);
    bool addNode(AceTreeItem *item);
    bool removeNode(AceTreeItem *item);
    bool containsNode(AceTreeItem *item);
    QList<AceTreeItem *> nodes() const;
    int nodeCount() const;
    QStringList nodeNames() const;
    AceTreeItem *nameToNode(const QString &name) const;
    QList<AceTreeItem *> nameToNodes(const QString &name) const;

public:
    static AceTreeItem *read(QDataStream &in);
    void write(QDataStream &out) const;
    AceTreeItem *clone(const QString &newName = {}) const;

protected:
    void propagateModel(AceTreeModel *model);

    AceTreeItem(AceTreeItemPrivate &d, const QString &name);

    QScopedPointer<AceTreeItemPrivate> d_ptr;

    friend class AceTreeModel;
    friend class AceTreeModelPrivate;
};

class AceTreeModelPrivate;

class QMEDITING_EXPORT AceTreeModel : public QObject {
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

    void startLogging(QIODevice *dev);
    void stopLogging();
    static AceTreeModel *recover(const QByteArray &data);

    AceTreeItem *itemFromIndex(int index) const;

    AceTreeItem *rootItem() const;
    void setRootItem(AceTreeItem *item);

    AceTreeItem *reset();

signals:
    void dynamicDataChanged(AceTreeItem *item, const QString &key, const QVariant &newValue, const QVariant &oldValue);
    void propertyChanged(AceTreeItem *item, const QString &key, const QVariant &newValue, const QVariant &oldValue);

    void bytesSet(AceTreeItem *item, int start, const QByteArray &newBytes, const QByteArray &oldBytes);
    void bytesTruncated(AceTreeItem *item, int size, const QByteArray &oldBytes, int delta);

    void rowsInserted(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);
    void rowsAboutToMove(AceTreeItem *parent, int index, int count, int dest);
    void rowsMoved(AceTreeItem *parent, int index, int count, int dest);
    void rowsAboutToRemove(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);
    void rowsRemoved(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);

    void recordAdded(AceTreeItem *parent, int seq, AceTreeItem *item);
    void recordAboutToRemove(AceTreeItem *parent, int seq, AceTreeItem *item);
    void recordRemoved(AceTreeItem *parent, int seq, AceTreeItem *item);

    void nodeAdded(AceTreeItem *parent, AceTreeItem *item);
    void nodeAboutToRemove(AceTreeItem *parent, AceTreeItem *item);
    void nodeRemoved(AceTreeItem *parent, AceTreeItem *item);

    void rootAboutToChange(AceTreeItem *newRoot, AceTreeItem *oldRoot);
    void rootChanged(AceTreeItem *newRoot, AceTreeItem *oldRoot);

    void stepChanged(int step);
    void loggingError();

protected:
    AceTreeModel(AceTreeModelPrivate &d, QObject *parent = nullptr);

    QScopedPointer<AceTreeModelPrivate> d_ptr;

    friend class AceTreeItem;
    friend class AceTreeItemPrivate;
};

class AceTreeItemSubscriberPrivate;

class QMEDITING_EXPORT AceTreeItemSubscriber {
    Q_DISABLE_COPY_MOVE(AceTreeItemSubscriber)
public:
    AceTreeItemSubscriber();
    virtual ~AceTreeItemSubscriber();

    AceTreeItem *treeItem() const;

    virtual void dynamicDataChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue);
    virtual void propertyChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue);

    virtual void bytesSet(int start, const QByteArray &newBytes, const QByteArray &oldBytes);
    virtual void bytesTruncated(int size, const QByteArray &oldBytes, int delta);

    virtual void rowsInserted(int index, const QVector<AceTreeItem *> &items);
    virtual void rowsAboutToMove(int index, int count, int dest);
    virtual void rowsMoved(int index, int count, int dest);
    virtual void rowsAboutToRemove(int index, const QVector<AceTreeItem *> &items);
    virtual void rowsRemoved(int index, const QVector<AceTreeItem *> &items);

    virtual void recordAdded(int seq, AceTreeItem *item);
    virtual void recordAboutToRemove(int seq, AceTreeItem *item);
    virtual void recordRemoved(int seq, AceTreeItem *item);

    virtual void nodeAdded(AceTreeItem *item);
    virtual void nodeAboutToRemove(AceTreeItem *item);
    virtual void nodeRemoved(AceTreeItem *item);

private:
    AceTreeItemSubscriberPrivate *d;

    friend class AceTreeItem;
    friend class AceTreeItemPrivate;
    friend class AceTreeModel;
    friend class AceTreeModelPrivate;
    friend class AceTreeItemSubscriberPrivate;
};

bool AceTreeItem::isRoot() const {
    return status() == Root;
}

bool AceTreeItem::isRow() const {
    return status() == Row;
}

bool AceTreeItem::isRecord() const {
    return status() == Record;
}

bool AceTreeItem::isNode() const {
    return status() == Node;
}

bool AceTreeItem::isManagedRoot() const {
    return status() == ManagedRoot;
}

inline void AceTreeItem::clearDynamicData(const QString &key) {
    setDynamicData(key, {});
}

inline bool AceTreeItem::clearProperty(const QString &key) {
    return setProperty(key, {});
}

inline QVariantHash AceTreeItem::properties() const {
    return propertyMap();
}

inline bool AceTreeItem::prependRow(AceTreeItem *item) {
    return insertRows(0, {item});
}

inline bool AceTreeItem::prependRows(const QVector<AceTreeItem *> &items) {
    return insertRows(0, items);
}

inline bool AceTreeItem::appendRow(AceTreeItem *item) {
    return insertRows(rowCount(), {item});
}

inline bool AceTreeItem::appendRows(const QVector<AceTreeItem *> &items) {
    return insertRows(rowCount(), items);
}

inline bool AceTreeItem::insertRow(int index, AceTreeItem *item) {
    return insertRows(index, {item});
}

inline bool AceTreeItem::moveRows2(int index, int count, int dest) {
    return moveRows(index, count, (dest <= index) ? dest : (dest + count));
}

inline bool AceTreeItem::removeRow(int index) {
    return removeRows(index, 1);
}

inline bool AceTreeItem::insertNode(AceTreeItem *item) {
    return addNode(item);
}

inline void AceTreeModel::nextStep() {
    setCurrentStep(currentStep() + 1);
}

inline void AceTreeModel::previousStep() {
    setCurrentStep(currentStep() - 1);
}

#endif // ACETREEMODEL_H
