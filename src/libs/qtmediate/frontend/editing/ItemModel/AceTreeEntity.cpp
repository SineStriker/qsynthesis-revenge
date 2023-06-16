#include "AceTreeEntity.h"
#include "AceTreeEntity_p.h"

AceTreeEntityPrivate::AceTreeEntityPrivate() {
    m_treeItem = nullptr;
    parent = nullptr;
}

AceTreeEntityPrivate::~AceTreeEntityPrivate() {
}

void AceTreeEntityPrivate::init() {
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

void AceTreeEntity::initialize() {
    Q_D(AceTreeEntity);
    if (d->m_treeItem) {
        qWarning() << "AceTreeEntity::setup(): entity has been initialized" << this;
        return;
    }

    d->m_treeItem = new AceTreeItem(name());
    doInitialize();
}

void AceTreeEntity::setup(AceTreeItem *item) {
    Q_D(AceTreeEntity);
    if (d->m_treeItem) {
        qWarning() << "AceTreeEntity::setup(): entity has been setup" << this;
        return;
    }

    d->m_treeItem = item;
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

void AceTreeEntity::doInitialize() {
    // Do nothing
}

void AceTreeEntity::doSetup() {
    // Do nothing
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