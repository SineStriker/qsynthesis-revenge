#include "AceTreeEntityVector.h"
#include "AceTreeEntityVector_p.h"

AceTreeEntityVectorPrivate::AceTreeEntityVectorPrivate() {
}

AceTreeEntityVectorPrivate::~AceTreeEntityVectorPrivate() {
}

AceTreeEntityVector::AceTreeEntityVector(const QString &name, QObject *parent) : AceTreeEntity(name, parent) {
}

AceTreeEntityVector::~AceTreeEntityVector() {
    Q_D(AceTreeEntityVector);
    qDeleteAll(d->vector);
    qDeleteAll(d->lastInsertOnes);
}

bool AceTreeEntityVector::setup(AceTreeItem *treeItem) {
    if (!AceTreeEntity::setup(treeItem)) {
        return false;
    }

    return true;
}

void AceTreeEntityVector::move(int from, int to, int count) {
    Q_D(AceTreeEntityVector);
    d->treeItem->moveRows(from, to, count);
}

void AceTreeEntityVector::remove(int index, int count) {
    Q_D(AceTreeEntityVector);
    d->treeItem->removeRows(index, count);
}

int AceTreeEntityVector::size() const {
    Q_D(const AceTreeEntityVector);
    return d->vector.size();
}

void AceTreeEntityVector::insertImpl(int index, const QVector<AceTreeEntity *> &items) {
    Q_D(AceTreeEntityVector);

    QVector<AceTreeItem *> treeItems;
    treeItems.reserve(items.size());
    for (const auto &item : items) {
        treeItems.append(item->treeItem());
    }

    d->lastInsertOnes = items;
    d->treeItem->insertRows(index, treeItems);
}

AceTreeEntity *AceTreeEntityVector::atImpl(int index) const {
    Q_D(const AceTreeEntityVector);
    return d->vector.at(index);
}

int AceTreeEntityVector::indexOfImpl(AceTreeEntity *item) const {
    Q_D(const AceTreeEntityVector);
    return d->vector.indexOf(item);
}

AceTreeEntityVector::AceTreeEntityVector(AceTreeEntityVectorPrivate &d, const QString &name, QObject *parent)
    : AceTreeEntity(d, name, parent) {
}