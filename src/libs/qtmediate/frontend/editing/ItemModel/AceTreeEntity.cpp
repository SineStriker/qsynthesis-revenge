#include "AceTreeEntity.h"
#include "AceTreeEntity_p.h"

using ItemEntityHash = QHash<AceTreeItem *, AceTreeEntity *>;
Q_GLOBAL_STATIC(ItemEntityHash, globalItemIndexes)

AceTreeEntityPrivate::AceTreeEntityPrivate() {
    m_treeItem = nullptr;
    parent = nullptr;
}

AceTreeEntityPrivate::~AceTreeEntityPrivate() {
    if (m_treeItem)
        globalItemIndexes->remove(m_treeItem);
}

void AceTreeEntityPrivate::init() {
}

void AceTreeEntityPrivate::init_deferred() {
    Q_Q(AceTreeEntity);
    globalItemIndexes->insert(m_treeItem, q);
    m_treeItem->addSubscriber(this);
}

AceTreeEntity::AceTreeEntity(QObject *parent) : AceTreeEntity(*new AceTreeEntityPrivate(), parent) {
}

AceTreeEntity::~AceTreeEntity() {
    Q_D(AceTreeEntity);
    if (d->parent)
        d->parent->removeChild(this);

    if (d->m_treeItem->isFree())
        delete d->m_treeItem;

    for (const auto &child : qAsConst(d->children)) {
        if (!child->parent())
            delete child;
    }
}

QString AceTreeEntity::name() const {
    Q_D(const AceTreeEntity);
    return d->name;
}

void AceTreeEntity::initialize() {
    Q_D(AceTreeEntity);
    auto &treeItem = d->m_treeItem;
    if (treeItem) {
        qWarning() << "AceTreeEntity::setup(): entity has been initialized" << this;
        return;
    }

    treeItem = new AceTreeItem(name());
    d->init_deferred();
    doInitialize();
}

void AceTreeEntity::setup(AceTreeItem *item) {
    Q_D(AceTreeEntity);
    auto &treeItem = d->m_treeItem;
    if (treeItem) {
        qWarning() << "AceTreeEntity::setup(): entity has been setup" << this;
        return;
    }

    treeItem = item;
    d->init_deferred();
    doSetup();
}

QList<AceTreeEntity *> AceTreeEntity::childEntities() const {
    Q_D(const AceTreeEntity);
    return d->children.values();
}

bool AceTreeEntity::hasChildEntity(AceTreeEntity *child) const {
    Q_D(const AceTreeEntity);
    return d->children.contains(child);
}

AceTreeEntity *AceTreeEntity::parentEntity() const {
    Q_D(const AceTreeEntity);
    return d->parent;
}

const AceTreeItem *AceTreeEntity::treeItem() const {
    Q_D(const AceTreeEntity);
    return d->m_treeItem;
}

const AceTreeModel *AceTreeEntity::treeModel() const {
    Q_D(const AceTreeEntity);
    return d->m_treeItem ? d->m_treeItem->model() : nullptr;
}

bool AceTreeEntity::addChild(AceTreeEntity *child) {
    Q_D(AceTreeEntity);
    auto &parent = child->d_func()->parent;
    if (parent) {
        qWarning() << "AceTreeEntity::addChild(): entity" << child << "has parent" << parent;
        return false;
    }
    d->children.insert(child);
    parent = child;
    childAdded(child);
    return true;
}

bool AceTreeEntity::removeChild(AceTreeEntity *child) {
    Q_D(AceTreeEntity);
    auto it = d->children.find(child);
    if (it == d->children.end()) {
        qWarning() << "AceTreeEntity::removeChild(): entity" << child << "doesn't belong to" << this;
        return false;
    }
    childAboutToRemove(child);
    child->d_func()->parent = nullptr;
    d->children.erase(it);
    return true;
}

AceTreeEntity *AceTreeEntity::itemToEntity(AceTreeItem *item) {
    return globalItemIndexes->value(item, nullptr);
}

void AceTreeEntity::childAdded(AceTreeEntity *child) {
    Q_UNUSED(child) //
}

void AceTreeEntity::childAboutToRemove(AceTreeEntity *child){
    Q_UNUSED(child) //
}

AceTreeEntity::AceTreeEntity(AceTreeEntityPrivate &d, QObject *parent)
    : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
