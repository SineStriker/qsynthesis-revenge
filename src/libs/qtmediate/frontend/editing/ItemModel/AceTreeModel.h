#ifndef ACETREEMODEL_H
#define ACETREEMODEL_H

#include <QIODevice>
#include <QObject>
#include <QVariant>

#include "QMEditingGlobal.h"

class AceTreeModel;

class AceTreeItemPrivate;

class QMEDITING_EXPORT AceTreeItem {
    Q_DECLARE_PRIVATE(AceTreeItem)
public:
    explicit AceTreeItem(const QString &name);
    virtual ~AceTreeItem();

    QString name() const;
    QVariant dynamicData(const QString &key) const;
    void setDynamicData(const QString &key, const QVariant &value);
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
    void setProperty(const QString &key, const QVariant &value);
    QVariantHash properties() const;

    // ByteArray
    void setBytes(int start, const QByteArray &bytes);
    void truncateBytes(int size);
    QByteArray bytes() const;
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
    AceTreeItem *row(int row) const;
    int rowIndexOf(AceTreeItem *item) const;
    int rowCount() const;

    // Hash - Records
    int addRecord(AceTreeItem *item);
    bool removeRecord(int seq);
    AceTreeItem *record(int seq);
    int recordIndexOf(AceTreeItem *item) const;
    QList<int> records() const;
    int recordCount() const;
    int maxRecordSeq() const;

    // Set - Nodes
    inline bool insertNode(AceTreeItem *item);
    bool addNode(AceTreeItem *item);
    bool containsNode(AceTreeItem *item);
    bool removeNode(AceTreeItem *item);
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
    void rowsMoved(AceTreeItem *parent, int index, int count, int dest);
    void rowsAboutToRemove(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items);
    void rowsRemoved(AceTreeItem *parent, int index, int count);

    void recordAdded(AceTreeItem *parent, int seq, AceTreeItem *item);
    void recordAboutToRemove(AceTreeItem *parent, int seq, AceTreeItem *item);
    void recordRemoved(AceTreeItem *parent, int seq);

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
