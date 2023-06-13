#include "AceTreeEntity.h"
#include "AceTreeEntity_p.h"

AceTreeEntityPrivate::AceTreeEntityPrivate() {
}

AceTreeEntityPrivate::~AceTreeEntityPrivate() {
}

void AceTreeEntityPrivate::init() {
}

AceTreeEntity::AceTreeEntity(QObject *parent) : AceTreeEntity(*new AceTreeEntityPrivate(), parent) {
}

AceTreeEntity::~AceTreeEntity() {
}

void AceTreeEntity::setup(AceTreeItem *item) {
}

AceTreeEntity::AceTreeEntity(AceTreeEntityPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}

AceTreeEntityFactory::AceTreeEntityFactory(QObject *parent)
    : QObject(parent), d(new AceTreeEntityFactoryPrivate(this)) {
}

AceTreeEntityFactory::~AceTreeEntityFactory() {
    delete d;
}

bool AceTreeEntityFactory::addChild(const QMetaObject *parent, const QString &key, const QMetaObject *child) {
    return false;
}

bool AceTreeEntityFactory::removeChild(const QMetaObject *parent, const QString &key) {
    return false;
}

const QMetaObject *AceTreeEntityFactory::child(const QMetaObject *parent, const QString &key) {
    return nullptr;
}

AceTreeEntity *AceTreeEntityFactory::createImpl(const QMetaObject *metaObject, AceTreeItem *treeItem) {
    return nullptr;
}
