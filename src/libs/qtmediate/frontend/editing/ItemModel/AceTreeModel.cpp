#include "AceTreeModel.h"
#include "AceTreeModel_p.h"

#include <QMBatch.h>

#include <QDataStream>
#include <QDebug>
#include <QStack>
#include <QTextCodec>

#include <iostream>

// #define ENABLE_DEBUG_COUNT

AceTreeModelPrivate::BaseOp::~BaseOp() {
#ifdef ENABLE_DEBUG_COUNT
    qDebug() << "AceTree Operation destroyed" << c;
#endif
}

template <typename ForwardIterator>
static void qCloneAll(ForwardIterator begin, ForwardIterator end, ForwardIterator dest) {
    while (begin != end) {
        *dest = (*begin)->clone();
        ++begin;
        ++dest;
    }
}

template <template <class> class Container1, template <class> class Container2, class T>
static Container2<T> qCloneAll(const Container1<T> &container) {
    Container2<T> res;
    res.reserve(container.size());
    for (const auto &item : container)
        res.push_back(item->clone());
    return res;
}

static bool validateArrayQueryArguments(int index, int size) {
    return index >= 0 && index <= size;
}

static bool validateArrayRemoveArguments(int index, int &count, int size) {
    return (index >= 0 && index <= size)                                 // index bound
           && ((count = qMin(count, size - index)) > 0 && count <= size) // count bound
        ;
}

static const char SIGN_TREE_MODEL[] = "ATM"
                                      "TX"
                                      "LOG";
static const char SIGN_TREE_ITEM[] = "item";
static const char SIGN_OPERATION[] = "SEG ";

namespace AceTreePrivate {

    static QDataStream &operator<<(QDataStream &stream, const QString &s) {
        return stream << s.toUtf8();
    }

    static QDataStream &operator>>(QDataStream &stream, QString &s) {
        QByteArray arr;
        stream >> arr;
        if (stream.status() == QDataStream::Ok) {
            s = QString::fromUtf8(arr);
        }
        return stream;
    }

    static QDataStream &operator<<(QDataStream &stream, const QVariantHash &s) {
        stream << s.size();
        for (auto it = s.begin(); it != s.end(); ++it) {
            AceTreePrivate::operator<<(stream, it.key());
            if (it->type() == QVariant::String) {
            }
            stream << it.value();
        }
        return stream;
    }

    static QDataStream &operator>>(QDataStream &stream, QVariantHash &s) {
        int size;
        stream >> size;
        s.reserve(size);
        for (int i = 0; i < size; ++i) {
            QString key;
            AceTreePrivate::operator>>(stream, key);
            if (stream.status() != QDataStream::Ok) {
                break;
            }
            QVariant val;
            stream >> val;
            if (stream.status() != QDataStream::Ok) {
                break;
            }
            s.insert(key, val);
        }
        return stream;
    }

}

#define myWarning(func)  (qWarning().nospace() << "AceTreeItem::" << (func) << "(): ").maybeSpace()
#define myWarning2(func) (qWarning().nospace() << "AceTreeModel::" << (func) << "(): ").maybeSpace()

void AceTreeItemPrivate::propagateItems(AceTreeItem *item, const std::function<void(AceTreeItem *)> &f) {
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
        for (const auto &child : qAsConst(d->records))
            stack.push(child);
    }
}

QByteArray AceTreeItemPrivate::itemToData(AceTreeItem *item) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    item->write(stream);
    return data;
}

AceTreeItem *AceTreeItemPrivate::itemFromStream(QDataStream &stream, QByteArray *out) {
    auto dev = stream.device();

    // Read item
    auto pos = dev->pos();
    auto item = AceTreeItem::read(stream);
    if (!item) {
        return nullptr;
    }

    // Restore device position and read bytes
    auto size = dev->pos() - pos;
    dev->seek(pos);
    *out = dev->read(size);

    return item;
}

void AceTreeItemPrivate::enterModel(AceTreeItem *item, AceTreeModel *model) {
    if (!item->d_func()->model) {
        item->propagateModel(model);
    }
}

void AceTreeItemPrivate::leaveModel(AceTreeItem *item) {
}

void AceTreeItemPrivate::forceDelete(AceTreeItem *item) {
    item->d_func()->m_forceDelete = true;
    delete item;
}

/**
 * User can only add child items whose index is zero into a model
 *
 * `index` field will be serialized in to binary data, but the index information in binary data deserialized to
 * `indexHint` field, because user may add an deserialized item to a model
 *
 * When a model is recovering from binary data itself, the `indexHint` really plays an important role in
 * rebuilding the original structure precisely
 */

#ifdef ENABLE_DEBUG_COUNT
static int item_count = 0;
#endif

// Item
AceTreeItemPrivate::AceTreeItemPrivate() {
#ifdef ENABLE_DEBUG_COUNT
    item_count++;
    qDebug() << "AceTreeItemPrivate construct, left" << item_count;
#endif

    parent = nullptr;
    model = nullptr;
    m_index = 0;
    m_indexHint = 0;
    m_forceDelete = false;
    maxRecordSeq = 0;
    status = AceTreeItem::Root;
}

AceTreeItemPrivate::~AceTreeItemPrivate() {
    Q_Q(AceTreeItem);

#ifdef ENABLE_DEBUG_COUNT
    item_count--;
    qDebug() << "AceTreeItemPrivate destroy, left" << item_count << q->name();
#endif

    // No need to detach from model if the model is in destruction
    if (model) {
        if (!model->d_func()->is_destruct) {
            if (!m_forceDelete)
                myWarning("~AceTreeItem") << "Deleting a managed item may cause crash";

            model->d_func()->removeIndex(m_index);
        }
    } else {
        switch (status) {
            case AceTreeItem::Row:
                parent->removeRow(q);
                break;
            case AceTreeItem::Record:
                parent->removeRecord(q);
                break;
            case AceTreeItem::Node:
                parent->removeNode(q);
                break;
            default:
                break;
        }
    }

    // Clear subscribers
    for (const auto &sub : subscribers) {
        sub->d->m_treeItem = nullptr;
    }
    subscribers.clear();

    if (m_forceDelete) {
        for (const auto &item : qAsConst(vector))
            forceDelete(item);
        for (const auto &item : qAsConst(set))
            forceDelete(item);
        for (const auto &item : qAsConst(records))
            forceDelete(item);
    } else {
        qDeleteAll(vector);
        qDeleteAll(set);
        qDeleteAll(records);
    }
}

void AceTreeItemPrivate::init() {
}

AceTreeItem::AceTreeItem(const QString &name) : AceTreeItem(*new AceTreeItemPrivate(), name) {
}

AceTreeItem::~AceTreeItem() {
}

bool AceTreeItem::addSubscriber(AceTreeItemSubscriber *sub) {
    Q_D(AceTreeItem);
    auto res = d->subscribers.append(sub).second;
    if (res) {
        sub->d->m_treeItem = this;
    }
    return res;
}

bool AceTreeItem::removeSubscriber(AceTreeItemSubscriber *sub) {
    Q_D(AceTreeItem);
    auto res = d->subscribers.remove(sub);
    if (res) {
        sub->d->m_treeItem = nullptr;
    }
    return res;
}

QList<AceTreeItemSubscriber *> AceTreeItem::subscribers() const {
    Q_D(const AceTreeItem);
    return d->subscribers.values();
}

int AceTreeItem::subscriberCount() const {
    Q_D(const AceTreeItem);
    return d->subscribers.size();
}

bool AceTreeItem::hasSubscriber(AceTreeItemSubscriber *sub) const {
    Q_D(const AceTreeItem);
    return d->subscribers.contains(sub);
}

QString AceTreeItem::name() const {
    Q_D(const AceTreeItem);
    return d->name;
}

AceTreeItem::Status AceTreeItem::status() const {
    Q_D(const AceTreeItem);
    return d->status;
}

QVariant AceTreeItem::dynamicData(const QString &key) const {
    Q_D(const AceTreeItem);
    return d->dynamicData.value(key);
}

void AceTreeItem::setDynamicData(const QString &key, const QVariant &value) {
    Q_D(AceTreeItem);

    QVariant oldValue;
    auto it = d->dynamicData.find(key);
    if (it == d->dynamicData.end()) {
        if (!value.isValid())
            return;
        d->dynamicData.insert(key, value);
    } else {
        oldValue = it.value();
        if (!value.isValid())
            d->dynamicData.erase(it);
        else if (oldValue == value)
            return;
        else
            it.value() = value;
    }

    // Propagate signal
    for (const auto &sub : qAsConst(d->subscribers))
        sub->dynamicDataChanged(key, value, oldValue);
    if (d->model)
        emit d->model->dynamicDataChanged(this, key, value, oldValue);
}

QStringList AceTreeItem::dynamicDataKeys() const {
    Q_D(const AceTreeItem);
    return d->dynamicData.keys();
}

QVariantHash AceTreeItem::dynamicDataMap() const {
    Q_D(const AceTreeItem);
    return d->dynamicData;
}

AceTreeItem *AceTreeItem::parent() const {
    Q_D(const AceTreeItem);
    return d->parent;
}

AceTreeModel *AceTreeItem::model() const {
    Q_D(const AceTreeItem);
    return d->model;
}

int AceTreeItem::index() const {
    Q_D(const AceTreeItem);
    return d->model ? d->m_index : 0;
}

bool AceTreeItem::isFree() const {
    Q_D(const AceTreeItem);

    // The item is not free if it has parent or model

    return !d->model && !d->parent;
}

bool AceTreeItem::isObsolete() const {
    auto item = this;
    while (auto parent = item->d_func()->parent)
        item = parent;
    return item->d_func()->status == ManagedRoot;
}

bool AceTreeItem::isWritable() const {
    Q_D(const AceTreeItem);
    return d->model ? d->model->isWritable() : true;
}

QVariant AceTreeItem::property(const QString &key) const {
    Q_D(const AceTreeItem);
    return d->properties.value(key, {});
}

bool AceTreeItem::setProperty(const QString &key, const QVariant &value) {
    Q_D(AceTreeItem);
    if (!d->testModifiable(__func__))
        return false;

    d->setProperty_helper(key, value);
    return true;
}

QStringList AceTreeItem::propertyKeys() const {
    Q_D(const AceTreeItem);
    return d->properties.keys();
}

QVariantHash AceTreeItem::propertyMap() const {
    Q_D(const AceTreeItem);
    return d->properties;
}

bool AceTreeItem::setBytes(int start, const QByteArray &bytes) {
    Q_D(AceTreeItem);
    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!validateArrayQueryArguments(start, d->byteArray.size())) {
        myWarning(__func__) << "invalid parameters";
        return false;
    }

    d->setBytes_helper(start, bytes);
    return true;
}

bool AceTreeItem::insertBytes(int start, const QByteArray &bytes) {
    Q_D(AceTreeItem);
    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!validateArrayQueryArguments(start, d->byteArray.size())) {
        myWarning(__func__) << "invalid parameters";
        return false;
    }

    d->insertBytes_helper(start, bytes);
    return true;
}

bool AceTreeItem::removeBytes(int start, int size) {
    Q_D(AceTreeItem);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!validateArrayRemoveArguments(start, size, d->byteArray.size())) {
        myWarning(__func__) << "invalid parameters";
        return false;
    }

    d->removeBytes_helper(start, size);
    return true;
}

QByteArray AceTreeItem::bytes() const {
    Q_D(const AceTreeItem);
    return d->byteArray;
}

const char *AceTreeItem::bytesData() const {
    Q_D(const AceTreeItem);
    return d->byteArray.constData();
}

QByteArray AceTreeItem::midBytes(int start, int len) const {
    Q_D(const AceTreeItem);
    return d->byteArray.mid(start, len);
}

int AceTreeItem::bytesIndexOf(const QByteArray &bytes, int start) const {
    Q_D(const AceTreeItem);
    return d->byteArray.indexOf(bytes, start);
}

int AceTreeItem::bytesSize() const {
    Q_D(const AceTreeItem);
    return d->byteArray.size();
}

bool AceTreeItem::insertRows(int index, const QVector<AceTreeItem *> &items) {
    Q_D(AceTreeItem);
    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!validateArrayQueryArguments(index, d->vector.size())) {
        myWarning(__func__) << "invalid parameters";
        return false;
    }
    for (const auto &item : items) {
        if (!d->testInsertable(__func__, item)) {
            return false;
        }
    }

    d->insertRows_helper(index, items);
    return true;
}

bool AceTreeItem::moveRows(int index, int count, int dest) {
    Q_D(AceTreeItem);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!validateArrayRemoveArguments(index, count, d->vector.size()) ||
        (dest >= index && dest <= index + count) // dest bound
    ) {
        myWarning(__func__) << "invalid parameters";
        return false;
    }

    d->moveRows_helper(index, count, dest);
    return true;
}

bool AceTreeItem::removeRows(int index, int count) {
    Q_D(AceTreeItem);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!validateArrayRemoveArguments(index, count, d->vector.size())) {
        myWarning(__func__) << "invalid parameters";
        return false;
    }

    d->removeRows_helper(index, count);
    return true;
}

bool AceTreeItem::removeRow(AceTreeItem *item) {
    Q_D(AceTreeItem);
    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!item) {
        myWarning(__func__) << "trying to remove a null row from" << this;
        return false;
    }
    int index;
    if (item->parent() != this || (index = d->vector.indexOf(item)) < 0) {
        myWarning(__func__) << "item" << item << "is not a row of" << this;
        return false;
    }

    d->removeRows_helper(index, 1);
    return true;
}

AceTreeItem *AceTreeItem::row(int index) const {
    Q_D(const AceTreeItem);
    return (index >= 0 && index < d->vector.size()) ? d->vector.at(index) : nullptr;
}

QVector<AceTreeItem *> AceTreeItem::rows() const {
    Q_D(const AceTreeItem);
    return d->vector;
}

int AceTreeItem::rowIndexOf(AceTreeItem *item) const {
    Q_D(const AceTreeItem);
    return (item && item->parent() == this) ? d->vector.indexOf(item) : -1;
}

int AceTreeItem::rowCount() const {
    Q_D(const AceTreeItem);
    return d->vector.size();
}

int AceTreeItem::addRecord(AceTreeItem *item) {
    Q_D(AceTreeItem);

    if (!d->testModifiable(__func__))
        return -1;

    // Validate
    if (!d->testInsertable(__func__, item))
        return -1;

    auto seq = ++d->maxRecordSeq;
    d->addRecord_helper(seq, item);
    return seq;
}

bool AceTreeItem::removeRecord(int seq) {
    Q_D(AceTreeItem);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!d->records.contains(seq)) {
        myWarning(__func__) << "seq num" << seq << "doesn't exists in" << this;
        return false;
    }

    d->removeRecord_helper(seq);
    return true;
}

bool AceTreeItem::removeRecord(AceTreeItem *item) {
    Q_D(AceTreeItem);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!item) {
        myWarning(__func__) << "trying to remove a null record from" << this;
        return false;
    }
    int seq = d->recordIndexes.value(item, -1);
    if (seq < 0) {
        myWarning(__func__) << "seq num" << seq << "doesn't exists in" << this;
        return false;
    }

    d->removeRecord_helper(seq);
    return true;
}

AceTreeItem *AceTreeItem::record(int seq) {
    Q_D(const AceTreeItem);
    return d->records.value(seq, nullptr);
}

int AceTreeItem::recordIndexOf(AceTreeItem *item) const {
    Q_D(const AceTreeItem);
    return d->recordIndexes.value(item, -1);
}

QList<int> AceTreeItem::records() const {
    Q_D(const AceTreeItem);
    auto seqs = d->records.keys();
    std::sort(seqs.begin(), seqs.end());
    return seqs;
}

int AceTreeItem::recordCount() const {
    Q_D(const AceTreeItem);
    return d->records.size();
}

int AceTreeItem::maxRecordSeq() const {
    Q_D(const AceTreeItem);
    return d->maxRecordSeq;
}

bool AceTreeItem::addNode(AceTreeItem *item) {
    Q_D(AceTreeItem);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!d->testInsertable(__func__, item))
        return false;

    d->addNode_helper(item);
    return true;
}

bool AceTreeItem::removeNode(AceTreeItem *item) {
    Q_D(AceTreeItem);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!item) {
        myWarning(__func__) << "trying to remove a null node from" << this;
        return false;
    }
    if (item->parent() != this || !d->set.contains(item)) {
        myWarning(__func__) << "item" << item << "is not a node of" << this;
        return false;
    }

    d->removeNode_helper(item);
    return true;
}

bool AceTreeItem::containsNode(AceTreeItem *item) {
    Q_D(const AceTreeItem);
    return d->set.contains(item);
}

QList<AceTreeItem *> AceTreeItem::nodes() const {
    Q_D(const AceTreeItem);
    return d->set.values();
}

int AceTreeItem::nodeCount() const {
    Q_D(const AceTreeItem);
    return d->set.size();
}

QStringList AceTreeItem::nodeNames() const {
    Q_D(const AceTreeItem);
    return d->setNameIndexes.keys();
}

AceTreeItem *AceTreeItem::nameToNode(const QString &name) const {
    Q_D(const AceTreeItem);
    auto it = d->setNameIndexes.find(name);
    if (it == d->setNameIndexes.end() || it->isEmpty())
        return nullptr;
    return *it->begin();
}

QList<AceTreeItem *> AceTreeItem::nameToNodes(const QString &name) const {
    Q_D(const AceTreeItem);
    return d->setNameIndexes.value(name).values();
}

AceTreeItem *AceTreeItem::read(QDataStream &in) {
    char sign[sizeof(SIGN_TREE_ITEM) - 1];
    in.readRawData(sign, sizeof(sign));
    if (memcmp(SIGN_TREE_ITEM, sign, sizeof(sign)) != 0) {
        return nullptr;
    }

    QString name;
    AceTreePrivate::operator>>(in, name);

    auto item = new AceTreeItem(name);
    auto d = item->d_func();

    in >> d->m_indexHint; // Read saved index as hint
    AceTreePrivate::operator>>(in, d->properties);
    in >> d->byteArray;

    if (in.status() != QDataStream::Ok) {
        myWarning(__func__) << "read item data failed";
        goto abort;
    } else {
        int size;
        in >> size;
        d->vector.reserve(size);
        for (int i = 0; i < size; ++i) {
            auto child = read(in);
            if (!child) {
                myWarning(__func__) << "read vector item failed";
                goto abort;
            }
            child->d_func()->parent = item;
            d->vector.append(child);
        }

        in >> size;
        d->set.reserve(size);
        d->setNameIndexes.reserve(size);
        for (int i = 0; i < size; ++i) {
            auto child = read(in);
            if (!child) {
                myWarning(__func__) << "read set item failed";
                goto abort;
            }
            child->d_func()->parent = item;
            d->set.insert(child);
            d->setNameIndexes[child->name()] += child;
        }

        in >> size;
        d->records.reserve(size);
        d->recordIndexes.reserve(size);
        for (int i = 0; i < size; ++i) {
            int seq;
            in >> seq;

            auto child = read(in);
            if (!child) {
                myWarning(__func__) << "read record item failed";
                goto abort;
            }

            child->d_func()->parent = item;
            d->records.insert(seq, child);
            d->recordIndexes.insert(child, seq);
        }
    }

    return item;

abort:
    delete item;
    return nullptr;
}

void AceTreeItem::write(QDataStream &out) const {
    Q_D(const AceTreeItem);

    out.writeRawData("item", 4);
    AceTreePrivate::operator<<(out, d->name);
    out << d->m_index;
    AceTreePrivate::operator<<(out, d->properties);
    out << d->byteArray;

    out << d->vector.size();
    for (const auto &item : d->vector) {
        item->write(out);
    }

    out << d->set.size();
    for (const auto &item : d->set) {
        item->write(out);
    }

    out << d->records.size();
    for (auto it = d->records.begin(); it != d->records.end(); ++it) {
        out << it.key();        // write seq
        it.value()->write(out); // write item
    }
}

AceTreeItem *AceTreeItem::clone(const QString &newName) const {
    Q_D(const AceTreeItem);
    auto item = new AceTreeItem(newName.isEmpty() ? d->name : newName);

    auto d2 = item->d_func();
    d2->dynamicData = d->dynamicData;
    d2->properties = d->properties;
    d2->byteArray = d->byteArray;

    d2->vector.reserve(d->vector.size());
    for (auto &child : d->vector) {
        auto newChild = child->clone();
        newChild->d_func()->parent = item;
        d2->vector.append(newChild);
    }

    d2->set.reserve(d->set.size());
    d2->setNameIndexes.reserve(d->set.size());
    for (auto &child : d->set) {
        auto newChild = child->clone();
        newChild->d_func()->parent = item;
        d2->set.insert(newChild);
        d2->setNameIndexes[newChild->name()] += newChild;
    }

    d2->maxRecordSeq = d->maxRecordSeq;
    d2->records.reserve(d->records.size());
    d2->recordIndexes.reserve(d->recordIndexes.size());
    for (auto it = d->records.begin(); it != d->records.end(); ++it) {
        auto newChild = it.value()->clone();
        newChild->d_func()->parent = item;
        d2->records.insert(it.key(), newChild);
        d2->recordIndexes.insert(newChild, it.key());
    }

    return item;
}

void AceTreeItem::propagateModel(AceTreeModel *model) {
    Q_D(AceTreeItem);
    auto d2 = model->d_func();
    d->m_index = d2->addIndex(this, (d2->internalChange && d->m_indexHint != 0) ? d->m_indexHint : d->m_index);
    d->m_indexHint = 0;
    d->model = model;

    for (auto item : qAsConst(d->vector))
        item->propagateModel(model);

    for (auto item : qAsConst(d->set))
        item->propagateModel(model);

    for (auto item : qAsConst(d->records))
        item->propagateModel(model);
}

AceTreeItem::AceTreeItem(AceTreeItemPrivate &d, const QString &name) : d_ptr(&d) {
    d.q_ptr = this;
    d.name = name;

    d.init();
}

// Model
AceTreeModelPrivate::AceTreeModelPrivate() {
    is_destruct = false;
    maxIndex = 1;
    rootItem = nullptr;
    stepIndex = 0;
    internalChange = false;
}

AceTreeModelPrivate::~AceTreeModelPrivate() {
    is_destruct = true;
    qDeleteAll(operations.rbegin(), operations.rend());
    delete rootItem;
}

void AceTreeModelPrivate::init() {
}

void AceTreeModelPrivate::setRootItem_helper(AceTreeItem *item) {
    Q_Q(AceTreeModel);

    auto org = rootItem;

    // Pre-Propagate
    emit q->rootAboutToChange(item, org);

    // Do change
    if (org) {
        org->d_func()->status = AceTreeItem::ManagedRoot;
        AceTreeItemPrivate::leaveModel(org);
    }
    if (item) {
        item->d_func()->status = AceTreeItem::Root;
        AceTreeItemPrivate::enterModel(item, q);
    }
    rootItem = item;

    // Propagate signal
    rootChanged(item, org);
}

AceTreeItem *AceTreeModelPrivate::reset_helper() {
    Q_Q(AceTreeModel);

    if (stepIndex != 0)
        truncate(0);

    auto func = [&](AceTreeItem *item) {
        auto d = item->d_func();
        d->model = nullptr;
        d->m_index = 0;
    };

    AceTreeItemPrivate::propagateItems(rootItem, func);
    indexes.clear();
    maxIndex = 1;

    auto org = rootItem;
    rootItem = nullptr;

    emit q->stepChanged(0);

    return org;
}

void AceTreeModelPrivate::setCurrentStep_helper(int step) {
    Q_Q(AceTreeModel);

    internalChange = true;

    // Undo
    while (stepIndex > step) {
        if (!execute(operations.at(stepIndex - 1), true)) {
            goto out;
        }
        stepIndex--;
    }

    // Redo
    while (stepIndex < step) {
        if (!execute(operations.at(stepIndex), false)) {
            goto out;
        }
        stepIndex++;
    }

out:
    internalChange = false;

    // Serialize
    if (m_dev) {
        // Change step in log
        if (!writeCurrentStep() || (m_fileDev && (!m_fileDev->flush()))) {
            logError();
        }
    }

    emit q->stepChanged(stepIndex);
}

int AceTreeModelPrivate::addIndex(AceTreeItem *item, int idx) {
    int index = idx > 0 ? (maxIndex = qMax(maxIndex, idx), idx) : maxIndex++;
    indexes.insert(index, item);
    return index;
}

void AceTreeModelPrivate::removeIndex(int index) {
    indexes.remove(index);
}

bool AceTreeItemPrivate::testModifiable(const char *func) const {
    Q_Q(const AceTreeItem);

    if (model && model->d_func()->internalChange) {
        myWarning(func) << "modifying data during model's internal state switching is prohibited" << q;
        return false;
    }

    if (!model && m_index > 0) {
        myWarning(func) << "the item is now obsolete" << q;
        return false;
    }

    return true;
}

bool AceTreeItemPrivate::testInsertable(const char *func, const AceTreeItem *item) const {
    Q_Q(const AceTreeItem);
    if (!item) {
        myWarning(func) << "item is null";
        return false;
    }

    if (item == q) {
        myWarning(func) << "item cannot be its child itself";
        return false;
    }

    // Validate
    if (!item->isFree()) {
        myWarning(func) << "item is not free" << item;
        return false;
    }
    return true;
}

void AceTreeItemPrivate::setProperty_helper(const QString &key, const QVariant &value) {
    Q_Q(AceTreeItem);

    QVariant oldValue;
    auto it = properties.find(key);
    if (it == properties.end()) {
        if (!value.isValid())
            return;
        properties.insert(key, value);
    } else {
        oldValue = it.value();
        if (!value.isValid())
            properties.erase(it);
        else if (oldValue == value)
            return;
        else
            it.value() = value;
    }

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->propertyChanged(key, value, oldValue);
    if (model)
        model->d_func()->propertyChanged(q, key, value, oldValue);
}

void AceTreeItemPrivate::setBytes_helper(int start, const QByteArray &bytes) {
    Q_Q(AceTreeItem);

    auto len = bytes.size();
    auto oldBytes = this->byteArray.mid(start, len);

    // Do change
    int newSize = start + len;
    if (newSize > this->byteArray.size())
        this->byteArray.resize(newSize);
    this->byteArray.replace(start, len, bytes);

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->bytesSet(start, bytes, oldBytes);
    if (model)
        model->d_func()->bytesSet(q, start, bytes, oldBytes);
}

void AceTreeItemPrivate::insertBytes_helper(int start, const QByteArray &bytes) {
    Q_Q(AceTreeItem);

    // Do change
    byteArray.insert(start, bytes);

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->bytesInserted(start, bytes);
    if (model)
        model->d_func()->bytesInserted(q, start, bytes);
}

void AceTreeItemPrivate::removeBytes_helper(int start, int size) {
    Q_Q(AceTreeItem);

    auto bytes = byteArray.mid(start, size);

    // Do change
    byteArray.remove(start, size);

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->bytesRemoved(start, bytes);
    if (model)
        model->d_func()->bytesRemoved(q, start, bytes);
}

void AceTreeItemPrivate::insertRows_helper(int index, const QVector<AceTreeItem *> &items) {
    Q_Q(AceTreeItem);

    // Do change
    vector.insert(vector.begin() + index, items.size(), nullptr);
    for (int i = 0; i < items.size(); ++i) {
        auto item = items[i];
        vector[index + i] = item;

        item->d_func()->parent = q;
        if (model)
            enterModel(item, model);
    }

    // Update status
    for (const auto &item : items)
        item->d_func()->status = AceTreeItem::Row;

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->rowsInserted(index, items);
    if (model)
        model->d_func()->rowsInserted(q, index, items);
}

void AceTreeItemPrivate::moveRows_helper(int index, int count, int dest) {
    Q_Q(AceTreeItem);

    // Pre-Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->rowsAboutToMove(index, count, dest);
    if (model)
        emit model->rowsAboutToMove(q, index, count, dest);

    // Do change
    QMBatch::arrayMove(vector, index, count, dest);

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->rowsMoved(index, count, dest);
    if (model)
        model->d_func()->rowsMoved(q, index, count, dest);
}

void AceTreeItemPrivate::removeRows_helper(int index, int count) {
    Q_Q(AceTreeItem);

    QVector<AceTreeItem *> tmp;
    tmp.resize(count);
    std::copy(vector.begin() + index, vector.begin() + index + count, tmp.begin());

    // Pre-Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->rowsAboutToRemove(index, tmp);
    if (model)
        emit model->rowsAboutToRemove(q, index, tmp);

    // Do change
    for (const auto &item : qAsConst(tmp)) {
        item->d_func()->parent = nullptr;
        if (model)
            leaveModel(item);
    }
    vector.erase(vector.begin() + index, vector.begin() + index + count);

    // Update status
    if (model)
        for (const auto &item : qAsConst(tmp))
            item->d_func()->status = AceTreeItem::ManagedRoot;
    else
        for (const auto &item : qAsConst(tmp))
            item->d_func()->status = AceTreeItem::Root;

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->rowsRemoved(index, tmp);
    if (model)
        model->d_func()->rowsRemoved(q, index, tmp);
}

void AceTreeItemPrivate::addNode_helper(AceTreeItem *item) {
    Q_Q(AceTreeItem);

    // Do change
    item->d_func()->parent = q;
    if (model)
        enterModel(item, model);
    set.insert(item);
    setNameIndexes[item->name()] += item;

    // Update status
    item->d_func()->status = AceTreeItem::Node;

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->nodeAdded(item);
    if (model)
        model->d_func()->nodeAdded(q, item);
}

void AceTreeItemPrivate::removeNode_helper(AceTreeItem *item) {
    Q_Q(AceTreeItem);

    // Pre-Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->nodeAboutToRemove(item);
    if (model)
        emit model->nodeAboutToRemove(q, item);

    // Do change
    item->d_func()->parent = nullptr;
    if (model)
        leaveModel(item);
    set.remove(item);
    {
        auto it = setNameIndexes.find(item->name());
        if (it != setNameIndexes.end()) {
            it.value().remove(item);
            if (it.value().isEmpty()) {
                setNameIndexes.erase(it);
            }
        }
    }

    // Update status
    item->d_func()->status = model ? AceTreeItem::ManagedRoot : AceTreeItem::Root;

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->nodeRemoved(item);
    if (model)
        model->d_func()->nodeRemoved(q, item);
}

void AceTreeItemPrivate::addRecord_helper(int seq, AceTreeItem *item) {
    Q_Q(AceTreeItem);

    // Do change
    item->d_func()->parent = q;
    if (model)
        enterModel(item, model);

    records.insert(seq, item);
    recordIndexes.insert(item, seq);

    // Update status
    item->d_func()->status = AceTreeItem::Record;

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->recordAdded(seq, item);
    if (model)
        model->d_func()->recordAdded(q, seq, item);
}

void AceTreeItemPrivate::removeRecord_helper(int seq) {
    Q_Q(AceTreeItem);

    auto it = records.find(seq);
    auto item = it.value();

    // Pre-Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->recordAboutToRemove(seq, item);
    if (model)
        emit model->recordAboutToRemove(q, seq, item);

    // Do change
    item->d_func()->parent = nullptr;
    if (model)
        leaveModel(item);
    records.erase(it);
    recordIndexes.remove(item);

    // Update status
    item->d_func()->status = model ? AceTreeItem::ManagedRoot : AceTreeItem::Root;

    // Propagate signal
    for (const auto &sub : qAsConst(subscribers))
        sub->recordRemoved(seq, item);
    if (model)
        model->d_func()->recordRemoved(q, seq, item);
}

void AceTreeModelPrivate::serializeOperation(QDataStream &stream, BaseOp *baseOp) {
    // Write begin sign
    stream.writeRawData(SIGN_OPERATION, sizeof(SIGN_OPERATION) - 1);

    stream << (int) baseOp->c;
    switch (baseOp->c) {
        case PropertyChange: {
            auto op = static_cast<PropertyChangeOp *>(baseOp);
            stream << op->id;
            AceTreePrivate::operator<<(stream, op->key);
            stream << op->oldValue << op->newValue;
            break;
        }
        case BytesSet: {
            auto op = static_cast<BytesSetOp *>(baseOp);
            stream << op->id << op->start << op->oldBytes << op->newBytes;
            break;
        }
        case BytesInsert:
        case BytesRemove: {
            auto op = static_cast<BytesInsertRemoveOp *>(baseOp);
            stream << op->id << op->start << op->bytes;
            break;
        }
        case RowsInsert:
        case RowsRemove: {
            auto op = static_cast<RowsInsertRemoveOp *>(baseOp);
            stream << op->id << op->index << op->items.size();
            if (op->c == RowsInsert) {
                for (const auto &item : qAsConst(op->serialized)) {
                    stream.writeRawData(item.data(), item.size());
                }
            } else {
                // Write id only
                for (const auto &item : qAsConst(op->items)) {
                    stream << item->d_func()->m_index;
                }
            }
            break;
        }
        case RowsMove: {
            auto op = static_cast<RowsMoveOp *>(baseOp);
            stream << op->id << op->index << op->count << op->dest;
            break;
        }
        case RecordAdd:
        case RecordRemove: {
            auto op = static_cast<RecordAddRemoveOp *>(baseOp);
            stream << op->id << op->seq;
            if (op->c == RecordAdd) {
                stream.writeRawData(op->serialized, op->serialized.size());
            } else {
                // Write id only
                stream << op->item->d_func()->m_index;
            }
            break;
        }
        case NodeAdd:
        case NodeRemove: {
            auto op = static_cast<NodeAddRemoveOp *>(baseOp);
            stream << op->id;
            if (op->c == NodeAdd) {
                stream.writeRawData(op->serialized, op->serialized.size());
            } else {
                // Write id only
                stream << op->item->d_func()->m_index;
            }
            break;
        }
        case RootChange: {
            auto op = static_cast<RootChangeOp *>(baseOp);

            // Write old root id
            stream << (op->oldRoot ? op->oldRoot->d_func()->m_index : 0);

            // Write new root id
            if (op->newRoot) {
                stream << op->newRoot->d_func()->m_index;

                // Write new root
                stream.writeRawData(op->serialized, op->serialized.size());
            } else {
                stream << int(0);
            }
            break;
        }
        default:
            break;
    }
}

AceTreeModelPrivate::BaseOp *AceTreeModelPrivate::deserializeOperation(QDataStream &stream, QList<int> *ids) {
    // Read begin sign
    char sign[sizeof(SIGN_OPERATION) - 1];
    stream.readRawData(sign, sizeof(sign));
    if (memcmp(sign, SIGN_OPERATION, sizeof(sign)) != 0) {
        return nullptr;
    }

    int c;
    stream >> c;

    BaseOp *res = nullptr;
    switch (c) {
        case PropertyChange: {
            auto op = new PropertyChangeOp();
            stream >> op->id;
            AceTreePrivate::operator>>(stream, op->key);
            stream >> op->oldValue >> op->newValue;
            res = op;
            break;
        }
        case BytesSet: {
            auto op = new BytesSetOp();
            stream >> op->id >> op->start >> op->oldBytes >> op->newBytes;
            res = op;
            break;
        }
        case BytesInsert:
        case BytesRemove: {
            auto op = new BytesInsertRemoveOp(c == BytesInsert);
            stream >> op->id >> op->start >> op->bytes;
            res = op;
            break;
        }
        case RowsInsert:
        case RowsRemove: {
            auto op = new RowsInsertRemoveOp(c == RowsInsert);
            int size;
            stream >> op->id >> op->index >> size;
            op->items.reserve(size);
            if (c == RowsInsert) {
                for (int i = 0; i < size; ++i) {
                    QByteArray data;
                    auto item = AceTreeItemPrivate::itemFromStream(stream, &data);
                    if (!item) {
                        stream.setStatus(QDataStream::ReadCorruptData);
                        break;
                    }
                    op->items.append(item);

                    // Restore serialized data
                    op->serialized.append(data);
                }
            } else {
                int id;
                for (int i = 0; i < size; ++i) {
                    stream >> id;
                    if (ids)
                        ids->append(id);
                }
            }
            res = op;
            break;
        }
        case RowsMove: {
            auto op = new RowsMoveOp();
            stream >> op->id >> op->index >> op->count >> op->dest;
            res = op;
            break;
        }
        case RecordAdd:
        case RecordRemove: {
            auto op = new RecordAddRemoveOp(c == RecordAdd);
            stream >> op->id >> op->seq;
            if (c == RecordAdd) {
                QByteArray data;
                auto item = AceTreeItemPrivate::itemFromStream(stream, &data);
                if (!item) {
                    stream.setStatus(QDataStream::ReadCorruptData);
                } else {
                    op->item = item;

                    // Restore serialized data
                    op->serialized = std::move(data);
                }
            } else {
                int id;
                stream >> id;
                if (ids)
                    ids->append(id);
            }
            res = op;
            break;
        }
        case NodeAdd:
        case NodeRemove: {
            auto op = new NodeAddRemoveOp(c == NodeAdd);
            int size;
            stream >> op->id;
            if (c == NodeAdd) {
                QByteArray data;
                auto item = AceTreeItemPrivate::itemFromStream(stream, &data);
                if (!item) {
                    stream.setStatus(QDataStream::ReadCorruptData);
                } else {
                    op->item = item;

                    // Restore serialized data
                    op->serialized = std::move(data);
                }
            } else {
                int id;
                stream >> id;
                if (ids)
                    ids->append(id);
            }
            res = op;
            break;
        }
        case RootChange: {
            auto op = new RootChangeOp();

            // Read old root id
            int id;
            stream >> id;
            if (ids)
                ids->append(id);

            // Read new root id
            stream >> id;
            if (id != 0) {
                QByteArray data;
                auto item = AceTreeItemPrivate::itemFromStream(stream, &data);
                if (!item) {
                    stream.setStatus(QDataStream::ReadCorruptData);
                } else {
                    op->newRoot = item;

                    // Restore serialized data
                    op->serialized = std::move(data);
                }
            }
            res = op;
            break;
        }
        default:
            break;
    }

    if (stream.status() != QDataStream::Ok) {
        delete res;
        return nullptr;
    }

    return res;
}


void AceTreeModelPrivate::logStart() {
    auto &dev = m_dev;

    QDataStream stream(dev);

    offsets.startPos = dev->pos();
    stream.writeRawData(SIGN_TREE_MODEL, sizeof(SIGN_TREE_MODEL) - 1);

    offsets.countPos = dev->pos();
    stream << stepIndex;
    offsets.totalPos = dev->pos();
    stream << operations.size();

    offsets.dataPos = dev->pos();

    // Write all existing operations
    for (auto operation : operations) {
        AceTreeModelPrivate::serializeOperation(stream, operation);

        if (stream.status() != QDataStream::Ok) {
            logError();
            return;
        }

        offsets.begs.append(dev->pos());
    }

    if (m_fileDev)
        m_fileDev->flush();
}

void AceTreeModelPrivate::logStop() {
    offsets = {};
}

void AceTreeModelPrivate::propertyChanged(AceTreeItem *item, const QString &key, const QVariant &newValue,
                                          const QVariant &oldValue) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new PropertyChangeOp();
        op->id = item->d_func()->m_index;
        op->key = key;
        op->oldValue = oldValue;
        op->newValue = newValue;
        push(op);
    }
    emit q->propertyChanged(item, key, newValue, oldValue);
}

void AceTreeModelPrivate::bytesSet(AceTreeItem *item, int start, const QByteArray &newBytes,
                                   const QByteArray &oldBytes) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new BytesSetOp();
        op->id = item->d_func()->m_index;
        op->start = start;
        op->oldBytes = oldBytes;
        op->newBytes = newBytes;
        push(op);
    }
    emit q->bytesSet(item, start, newBytes, oldBytes);
}

void AceTreeModelPrivate::bytesInserted(AceTreeItem *item, int start, const QByteArray &bytes) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new BytesInsertRemoveOp(true);
        op->id = item->d_func()->m_index;
        op->start = start;
        op->bytes = bytes;
        push(op);
    }
    emit q->bytesInserted(item, start, bytes);
}

void AceTreeModelPrivate::bytesRemoved(AceTreeItem *item, int start, const QByteArray &bytes) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new BytesInsertRemoveOp(false);
        op->id = item->d_func()->m_index;
        op->start = start;
        op->bytes = bytes;
        push(op);
    }
    emit q->bytesRemoved(item, start, bytes);
}

void AceTreeModelPrivate::rowsInserted(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new RowsInsertRemoveOp(true);
        op->id = parent->d_func()->m_index;
        op->index = index;
        op->items = items;

        // Serialize all items for recovery
        op->serialized.reserve(items.size());
        for (const auto &item : items) {
            op->serialized.append(AceTreeItemPrivate::itemToData(item));
        }

        push(op);
    }
    emit q->rowsInserted(parent, index, items);
}

void AceTreeModelPrivate::rowsMoved(AceTreeItem *parent, int index, int count, int dest) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new RowsMoveOp();
        op->id = parent->d_func()->m_index;
        op->index = index;
        op->count = count;
        op->dest = dest;
        push(op);
    }
    emit q->rowsMoved(parent, index, count, dest);
}

void AceTreeModelPrivate::rowsRemoved(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new RowsInsertRemoveOp(false);
        op->id = parent->d_func()->m_index;
        op->index = index;
        op->items = items;
        push(op);
    }
    emit q->rowsRemoved(parent, index, items);
}

void AceTreeModelPrivate::recordAdded(AceTreeItem *parent, int seq, AceTreeItem *item) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new RecordAddRemoveOp(true);
        op->id = parent->d_func()->m_index;
        op->seq = seq;
        op->item = item;

        // Serialize item for recovery
        op->serialized = AceTreeItemPrivate::itemToData(item);

        push(op);
    }
    emit q->recordAdded(parent, seq, item);
}

void AceTreeModelPrivate::recordRemoved(AceTreeItem *parent, int seq, AceTreeItem *item) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new RecordAddRemoveOp(false);
        op->id = parent->d_func()->m_index;
        op->seq = seq;
        op->item = item;
        push(op);
    }

    emit q->recordRemoved(parent, seq, item);
}

void AceTreeModelPrivate::nodeAdded(AceTreeItem *parent, AceTreeItem *item) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new NodeAddRemoveOp(true);
        op->id = parent->d_func()->m_index;
        op->item = item;

        // Serialize item for recovery
        op->serialized = AceTreeItemPrivate::itemToData(item);

        push(op);
    }
    emit q->nodeAdded(parent, item);
}

void AceTreeModelPrivate::nodeRemoved(AceTreeItem *parent, AceTreeItem *item) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new NodeAddRemoveOp(false);
        op->id = parent->d_func()->m_index;
        op->item = item;
        push(op);
    }
    emit q->nodeRemoved(parent, item);
}

void AceTreeModelPrivate::rootChanged(AceTreeItem *newRoot, AceTreeItem *oldRoot) {
    Q_Q(AceTreeModel);
    if (!internalChange) {
        auto op = new RootChangeOp();
        op->oldRoot = oldRoot;
        op->newRoot = newRoot;

        // Serialize item for recovery
        if (newRoot) {
            op->serialized = AceTreeItemPrivate::itemToData(newRoot);
        }

        push(op);
    }
    emit q->rootChanged(newRoot, oldRoot);
}

bool AceTreeModelPrivate::execute(BaseOp *baseOp, bool undo) {
    Q_Q(AceTreeModel);
    switch (baseOp->c) {
        case PropertyChange: {
            auto op = static_cast<PropertyChangeOp *>(baseOp);
            auto item = q->itemFromIndex(op->id);
            if (!item)
                goto obsolete;
            item->d_func()->setProperty_helper(op->key, undo ? op->oldValue : op->newValue);
            break;
        }
        case BytesSet: {
            auto op = static_cast<BytesSetOp *>(baseOp);
            auto item = q->itemFromIndex(op->id);
            if (!item)
                goto obsolete;
            if (undo) {
                item->d_func()->setBytes_helper(op->start, op->oldBytes);

                // Need truncate
                int delta = op->newBytes.size() - op->oldBytes.size();
                if (delta > 0) {
                    item->d_func()->removeBytes_helper(item->bytesSize() - delta, delta);
                }
            } else {
                item->d_func()->setBytes_helper(op->start, op->newBytes);
            }
            break;
        }
        case BytesInsert:
        case BytesRemove: {
            auto op = static_cast<BytesInsertRemoveOp *>(baseOp);
            auto item = q->itemFromIndex(op->id);
            if (!item)
                goto obsolete;
            ((op->c == BytesRemove) ^ undo) ? item->d_func()->removeBytes_helper(op->start, op->bytes.size())
                                            : item->d_func()->insertBytes_helper(op->start, op->bytes);
            break;
        }
        case RowsInsert:
        case RowsRemove: {
            auto op = static_cast<RowsInsertRemoveOp *>(baseOp);
            auto item = q->itemFromIndex(op->id);
            if (!item)
                goto obsolete;
            ((op->c == RowsRemove) ^ undo) ? item->d_func()->removeRows_helper(op->index, op->items.size())
                                           : item->d_func()->insertRows_helper(op->index, op->items);
            break;
        }
        case RowsMove: {
            auto op = static_cast<RowsMoveOp *>(baseOp);
            auto item = q->itemFromIndex(op->id);
            if (!item)
                goto obsolete;
            if (undo) {
                int r_index;
                int r_dest;
                if (op->dest > op->index) {
                    r_index = op->dest - op->count;
                    r_dest = op->index;
                } else {
                    r_index = op->dest;
                    r_dest = op->index + op->count;
                }
                item->d_func()->moveRows_helper(r_index, op->count, r_dest);
            } else
                item->d_func()->moveRows_helper(op->index, op->count, op->dest);
            break;
        }
        case RecordAdd:
        case RecordRemove: {
            auto c = static_cast<RecordAddRemoveOp *>(baseOp);
            auto item = q->itemFromIndex(c->id);
            if (!item)
                goto obsolete;
            ((c->c == RecordRemove) ^ undo) ? item->d_func()->removeRecord_helper(c->seq)
                                            : item->d_func()->addRecord_helper(c->seq, c->item);
            break;
        }
        case NodeAdd:
        case NodeRemove: {
            auto c = static_cast<NodeAddRemoveOp *>(baseOp);
            auto item = q->itemFromIndex(c->id);
            if (!item)
                goto obsolete;
            ((c->c == NodeRemove) ^ undo) ? item->d_func()->removeNode_helper(c->item)
                                          : item->d_func()->addNode_helper(c->item);
            break;
        }
        case RootChange: {
            auto op = static_cast<RootChangeOp *>(baseOp);
            setRootItem_helper(undo ? op->oldRoot : op->newRoot);
            break;
        }
        default:
            break;
    }

    return true;

obsolete:
    myWarning2(__func__) << "Encountering obsolete operation" << baseOp << baseOp->c;
    return false;
}

void AceTreeModelPrivate::push(BaseOp *op) {
    Q_Q(AceTreeModel);
    truncate(stepIndex);

    operations.append(op);
    stepIndex++;

    // Serialize
    if (m_dev) {
        // Change step in log
        writeCurrentStep();

        QDataStream stream(m_dev);
        serializeOperation(stream, op);
        offsets.begs.append(m_dev->pos());

        if (stream.status() != QDataStream::Ok || (m_fileDev && (!m_fileDev->flush()))) {
            logError();
        }
    }

    emit q->stepChanged(stepIndex);
}

void AceTreeModelPrivate::truncate(int step) {
    Q_Q(AceTreeModel);

    while (operations.size() > step) {
        delete operations.takeLast();
    }
    offsets.begs.resize(step);
    stepIndex = step;

    // Serialize
    if (m_dev) {
        // Restore pos
        qint64 pos = offsets.begs.isEmpty() ? offsets.dataPos : offsets.begs.back();
        m_dev->seek(pos);
        if (!writeCurrentStep() || (m_fileDev && (!m_fileDev->resize(pos) || !m_fileDev->flush()))) {
            logError();
        }
    }
}

bool AceTreeModelPrivate::writeCurrentStep() const {
    qint64 pos = m_dev->pos();
    m_dev->seek(offsets.countPos);

    QDataStream stream(m_dev);
    stream << stepIndex;
    stream << operations.size();

    m_dev->seek(pos);

    return stream.status() == QDataStream::Ok;
}

AceTreeModel::AceTreeModel(QObject *parent) : AceTreeModel(*new AceTreeModelPrivate(), parent) {
}

AceTreeModel::~AceTreeModel() {
}

int AceTreeModel::steps() const {
    Q_D(const AceTreeModel);
    return d->operations.size();
}

int AceTreeModel::currentStep() const {
    Q_D(const AceTreeModel);
    return d->stepIndex;
}

void AceTreeModel::setCurrentStep(int step) {
    Q_D(AceTreeModel);
    if (step < 0 || step > d->operations.size() || step == d->stepIndex || d->internalChange)
        return;

    d->setCurrentStep_helper(step);
}

void AceTreeModel::truncateForwardSteps() {
    Q_D(AceTreeModel);
    if (d->internalChange)
        return;

    d->truncate(d->stepIndex);
}

bool AceTreeModel::isWritable() const {
    Q_D(const AceTreeModel);
    return !d->internalChange;
}

bool AceTreeModel::recover(const QByteArray &data) {
    Q_D(AceTreeModel);
    if (d->maxIndex > 1) {
        myWarning2(__func__) << "cannot recover when model has changed";
        return false;
    }

    QDataStream stream(data);

    char sign[sizeof(SIGN_TREE_MODEL) - 1];
    stream.readRawData(sign, sizeof(sign));
    if (memcmp(sign, SIGN_TREE_MODEL, sizeof(sign)) != 0) {
        myWarning2(__func__) << "read header sign failed";
        return false;
    }

    QHash<int, AceTreeItem *> insertedItems;
    auto collectItems = [&](AceTreeItem *item) {
        if (!item)
            return;

        QStack<AceTreeItem *> stack;
        stack.push(item);
        while (!stack.isEmpty()) {
            auto top = stack.pop();
            auto d = top->d_func();
            insertedItems.insert(d->m_indexHint, top);

            for (const auto &child : qAsConst(d->vector))
                stack.push(child);
            for (const auto &child : qAsConst(d->set))
                stack.push(child);
            for (const auto &child : qAsConst(d->records))
                stack.push(child);
        }
    };

    // Deserialize all operations, and collect all items
    QVector<AceTreeModelPrivate::BaseOp *> operations;
    int step, size;

    stream >> step; // stepIndex
    stream >> size; // operation size

    operations.reserve(size);
    for (int i = 0; i < size; ++i) {
        QList<int> ids;
        auto baseOp = AceTreeModelPrivate::deserializeOperation(stream, &ids);
        if (!baseOp) {
            myWarning2(__func__) << "read operation failed";
            goto op_abort;
        }

        switch (baseOp->c) {
            case AceTreeModelPrivate::RowsInsert: {
                auto op = static_cast<AceTreeModelPrivate::RowsInsertRemoveOp *>(baseOp);
                for (const auto &item : qAsConst(op->items))
                    collectItems(item);
                break;
            }
            case AceTreeModelPrivate::RowsRemove: {
                auto op = static_cast<AceTreeModelPrivate::RowsInsertRemoveOp *>(baseOp);
                op->items.reserve(ids.size());
                for (const auto &id : qAsConst(ids)) {
                    auto removedItem = insertedItems.value(id, nullptr);
                    if (!removedItem) {
                        myWarning2(__func__) << "removed row not found";
                        goto op_abort;
                    }
                    op->items.append(removedItem);
                }
                break;
            }
            case AceTreeModelPrivate::RecordAdd: {
                auto op = static_cast<AceTreeModelPrivate::RecordAddRemoveOp *>(baseOp);
                collectItems(op->item);
                break;
            }
            case AceTreeModelPrivate::RecordRemove: {
                auto op = static_cast<AceTreeModelPrivate::RecordAddRemoveOp *>(baseOp);
                auto removedItem = insertedItems.value(ids.front(), nullptr);
                if (!removedItem) {
                    myWarning2(__func__) << "removed node not found";
                    goto op_abort;
                }
                op->item = removedItem;
                break;
            }
            case AceTreeModelPrivate::NodeAdd: {
                auto op = static_cast<AceTreeModelPrivate::NodeAddRemoveOp *>(baseOp);
                collectItems(op->item);
                break;
            }
            case AceTreeModelPrivate::NodeRemove: {
                auto op = static_cast<AceTreeModelPrivate::NodeAddRemoveOp *>(baseOp);
                auto removedItem = insertedItems.value(ids.front(), nullptr);
                if (!removedItem) {
                    myWarning2(__func__) << "removed node not found";
                    goto op_abort;
                }
                op->item = removedItem;
                break;
            }
            case AceTreeModelPrivate::RootChange: {
                auto op = static_cast<AceTreeModelPrivate::RootChangeOp *>(baseOp);

                auto id = ids.front();
                if (id != 0) {
                    op->oldRoot = insertedItems.value(id, nullptr);
                    if (!op->oldRoot) {
                        myWarning2(__func__) << "old root not found";
                        goto op_abort;
                    }
                }

                collectItems(op->newRoot);
                break;
            }
            default:
                break;
        }

        operations.append(baseOp);
        continue;

    op_abort:
        qDeleteAll(operations.rbegin(), operations.rend());
        return false;
    }

    // Rebuild
    d->operations = std::move(operations);
    setCurrentStep(size);

    // Cannot happen if the log is not illegally modified
    //    if (currentStep() != size) {
    //        myWarning2(__func__) << "failed to reach given step";
    //        delete reset();
    //        return false;
    //    }

    // Go back to current step
    setCurrentStep(step);

    return true;
}

AceTreeItem *AceTreeModel::itemFromIndex(int index) const {
    Q_D(const AceTreeModel);
    return index > 0 ? d->indexes.value(index, nullptr) : nullptr;
}

AceTreeItem *AceTreeModel::rootItem() const {
    Q_D(const AceTreeModel);
    return d->rootItem;
}

void AceTreeModel::setRootItem(AceTreeItem *item) {
    Q_D(AceTreeModel);
    if (d->internalChange) {
        myWarning2(__func__) << "modifying root item during model's internal state switching is prohibited" << this;
        return;
    }

    if (item == d->rootItem)
        return;

    if (item && !item->isFree()) {
        myWarning2(__func__) << "item is not free" << item;
        return;
    }

    d->setRootItem_helper(item);
}

AceTreeItem *AceTreeModel::reset() {
    Q_D(AceTreeModel);
    if (d->internalChange) {
        myWarning2(__func__) << "reset model during model's internal state switching is prohibited" << this;
        return nullptr;
    }

    if (!d->rootItem)
        return nullptr;

    return d->reset_helper();
}

AceTreeModel::AceTreeModel(AceTreeModelPrivate &d, QObject *parent) : AceTreeRecoverable(d, parent) {
    d.init();
}

AceTreeItemSubscriberPrivate::AceTreeItemSubscriberPrivate(AceTreeItemSubscriber *q) : q(q) {
    m_treeItem = nullptr;
}

AceTreeItemSubscriberPrivate::~AceTreeItemSubscriberPrivate() {
    if (m_treeItem)
        m_treeItem->removeSubscriber(q);
}

AceTreeItemSubscriber::AceTreeItemSubscriber() : d(new AceTreeItemSubscriberPrivate(this)) {
}

AceTreeItemSubscriber::~AceTreeItemSubscriber() {
}

AceTreeItem *AceTreeItemSubscriber::treeItem() const {
    return d->m_treeItem;
}

void AceTreeItemSubscriber::dynamicDataChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue) {
}
void AceTreeItemSubscriber::propertyChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue) {
}
void AceTreeItemSubscriber::bytesSet(int start, const QByteArray &newBytes, const QByteArray &oldBytes) {
}
void AceTreeItemSubscriber::bytesInserted(int start, const QByteArray &bytes) {
}
void AceTreeItemSubscriber::bytesRemoved(int start, const QByteArray &bytes) {
}
void AceTreeItemSubscriber::rowsInserted(int index, const QVector<AceTreeItem *> &items) {
}
void AceTreeItemSubscriber::rowsAboutToMove(int index, int count, int dest) {
}
void AceTreeItemSubscriber::rowsMoved(int index, int count, int dest) {
}
void AceTreeItemSubscriber::rowsAboutToRemove(int index, const QVector<AceTreeItem *> &items) {
}
void AceTreeItemSubscriber::rowsRemoved(int index, const QVector<AceTreeItem *> &items) {
}
void AceTreeItemSubscriber::recordAdded(int seq, AceTreeItem *item) {
}
void AceTreeItemSubscriber::recordAboutToRemove(int seq, AceTreeItem *item) {
}
void AceTreeItemSubscriber::recordRemoved(int seq, AceTreeItem *item) {
}
void AceTreeItemSubscriber::nodeAdded(AceTreeItem *item) {
}
void AceTreeItemSubscriber::nodeAboutToRemove(AceTreeItem *item) {
}
void AceTreeItemSubscriber::nodeRemoved(AceTreeItem *item) {
}