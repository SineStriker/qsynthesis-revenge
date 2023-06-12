#include "AceTreeEntity.h"
#include "AceTreeEntity_p.h"

#include <QMChronSet.h>

#include "AceTreeModel.h"

#include <QDebug>
#include <QMetaType>

#include <utility>

AceTreeEntityPrivate::AceTreeEntityPrivate() {
    treeItem = nullptr;
    builder = nullptr;
}

AceTreeEntityPrivate::~AceTreeEntityPrivate() {
}

void AceTreeEntityPrivate::init() {
}

AceTreeEntity::AceTreeEntity(const QString &name, QObject *parent)
    : AceTreeEntity(*new AceTreeEntityPrivate(), name, parent) {
}

AceTreeEntity::~AceTreeEntity() {
}

AceTreeEntityBuilder *AceTreeEntity::builder() const {
    Q_D(const AceTreeEntity);
    return d->builder;
}

void AceTreeEntity::setBuilder(AceTreeEntityBuilder *builder) {
    Q_D(AceTreeEntity);
    d->builder = builder;
    for (const auto &child : qAsConst(d->children)) {
        child->setBuilder(builder);
    }
}

bool AceTreeEntity::setup(AceTreeItem *treeItem) {
    Q_D(AceTreeEntity);
    if (d->treeItem) {
        qWarning() << "AceTreeEntity::setup(): trying to setup an entity that has been setup";
        return false;
    }
    if (!treeItem) {
        qWarning() << "AceTreeEntity::setup(): trying to setup with null tree item";
        return false;
    }

    d->treeItem = treeItem;
    for (auto it = d->children.begin(); it != d->children.end(); ++it) {
        it.value()->setup(treeItem->nameToNode(it.key()));
    }

    return true;
}

QString AceTreeEntity::entityName() const {
    Q_D(const AceTreeEntity);
    return d->name;
}

AceTreeModel *AceTreeEntity::treeModel() const {
    Q_D(const AceTreeEntity);
    return d->treeItem ? d->treeItem->model() : nullptr;
}

AceTreeItem *AceTreeEntity::treeItem() const {
    Q_D(const AceTreeEntity);
    return d->treeItem;
}

AceTreeEntity *AceTreeEntity::child(const QString &name) const {
    Q_D(const AceTreeEntity);
    return d->children.value(name, nullptr);
}

AceTreeEntity *AceTreeEntity::parentEntity() const {
    return qobject_cast<AceTreeEntity *>(parent());
}

bool AceTreeEntity::addChild(AceTreeEntity *c) {
    Q_D(AceTreeEntity);
    if (!c) {
        qWarning() << "AceTreeEntity::addChild(): trying to add null child";
        return false;
    }
    if (d->children.contains(c->entityName())) {
        qWarning() << "AceTreeEntity::addChild(): trying to add duplicated factory:" << c->entityName();
        return false;
    }
    d->children.insert(c->entityName(), c);
    c->setParent(this);
    return true;
}

bool AceTreeEntity::removeChild(AceTreeEntity *c) {
    if (c == nullptr) {
        qWarning() << "AceTreeEntity::removeChild(): trying to remove null child";
        return false;
    }
    return removeChild(c->entityName());
}

bool AceTreeEntity::removeChild(const QString &name) {
    Q_D(AceTreeEntity);
    auto it = d->children.find(name);
    if (it == d->children.end()) {
        qWarning() << "AceTreeEntity::removeChild(): child does not exist:" << name;
        return false;
    }
    auto c = it.value();
    c->setParent(nullptr);
    d->children.erase(it);
    return true;
}

AceTreeEntity::AceTreeEntity(AceTreeEntityPrivate &d, const QString &name, QObject *parent)
    : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.name = name;

    d.init();
}

class AceTreeEntityBuilderPrivate {
public:
    AceTreeEntityBuilderPrivate(AceTreeEntityBuilder *q) : q(q){};

    void init() {
    }

    AceTreeEntityBuilder *q;

    struct BuilderData {
        BuilderData(const QMetaObject *metaObject = nullptr, const QList<QGenericArgument> &args = {})
            : metaObject(metaObject), args(args), is_meta_object(true){};
        BuilderData(AceTreeEntityBuilder::Builder builder)
            : builder(std::move(builder)), metaObject(nullptr), is_meta_object(false){};

        AceTreeEntity *build() const {
            if (is_meta_object) {
                QObject *obj;
                switch (args.size()) {
                    case 0:
                        obj = metaObject->newInstance();
                        break;
                    case 1:
                        obj = metaObject->newInstance(args.at(0));
                        break;
                    case 2:
                        obj = metaObject->newInstance(args.at(0), args.at(1));
                        break;
                    case 3:
                        obj = metaObject->newInstance(args.at(0), args.at(1), args.at(2));
                        break;
                    case 4:
                        obj = metaObject->newInstance(args.at(0), args.at(1), args.at(2), args.at(3));
                        break;
                    case 5:
                        obj = metaObject->newInstance(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4));
                        break;
                    case 6:
                        obj = metaObject->newInstance(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4),
                                                      args.at(5));
                        break;
                    case 7:
                        obj = metaObject->newInstance(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4),
                                                      args.at(5), args.at(6));
                        break;
                    case 8:
                        obj = metaObject->newInstance(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4),
                                                      args.at(5), args.at(6), args.at(7));
                        break;
                    case 9:
                        obj = metaObject->newInstance(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4),
                                                      args.at(5), args.at(6), args.at(7), args.at(8));
                        break;
                    default:
                        obj = metaObject->newInstance(args.at(0), args.at(1), args.at(2), args.at(3), args.at(4),
                                                      args.at(5), args.at(6), args.at(7), args.at(8), args.at(9));
                        break;
                }
                return qobject_cast<AceTreeEntity *>(obj);
            }
            return builder();
        }

        AceTreeEntityBuilder::Builder builder;
        const QMetaObject *metaObject;
        QList<QGenericArgument> args;

        bool is_meta_object;
    };

    QHash<QString, BuilderData> builders;
    QHash<QString, QMChronSet<QString>> subBuilders;
    QString rootId;
};

AceTreeEntityBuilder::AceTreeEntityBuilder(QObject *parent) : AceTreeEntityBuilder({}, parent) {
}

AceTreeEntityBuilder::AceTreeEntityBuilder(const QString &rootBuilder, QObject *parent)
    : QObject(parent), d(new AceTreeEntityBuilderPrivate(this)) {
    d->init();
    d->rootId = rootBuilder;
}

AceTreeEntityBuilder::~AceTreeEntityBuilder() {
    delete d;
}

QString AceTreeEntityBuilder::rootBuilder() const {
    return d->rootId;
}

void AceTreeEntityBuilder::setRootBuilder(const QString &id) {
    d->rootId = id;
}

AceTreeEntity *AceTreeEntityBuilder::build(const QString &id) const {
    auto it = d->builders.find(id);
    if (it == d->builders.end())
        return nullptr;

    auto entity = it->build();
    if (!entity)
        return nullptr;

    // Add children
    for (const auto &childId : d->subBuilders.value(id)) {
        auto childEntity = build(childId);
        if (!childEntity)
            continue;
        entity->addChild(childEntity);
    }

    entity->setBuilder(const_cast<AceTreeEntityBuilder *>(this));
    return entity;
}

bool AceTreeEntityBuilder::addBuilder(const QString &id, const AceTreeEntityBuilder::Builder &builder) {
    if (!builder) {
        qWarning() << "AceTreeEntity::addChild(): trying to add null builder";
        return false;
    }
    if (d->builders.contains(id)) {
        qWarning() << "AceTreeEntity::addChild(): trying to add duplicated builder:" << id;
        return false;
    }
    d->builders.insert(id, builder);
    return true;
}

bool AceTreeEntityBuilder::addBuilder(const QString &id, const QMetaObject *metaObject) {
    if (!metaObject) {
        qWarning() << "AceTreeEntityBuilder::addBuilder(): trying to add null builder";
        return false;
    }
    if (d->builders.contains(id)) {
        qWarning() << "AceTreeEntityBuilder::addBuilder(): trying to add duplicated builder:" << id;
        return false;
    }
    d->builders.insert(id, {metaObject, {}});
    return true;
}

bool AceTreeEntityBuilder::removeBuilder(const QString &id) {
    auto it = d->builders.find(id);
    if (it == d->builders.end()) {
        qWarning() << "AceTreeEntityBuilder::removeBuilder(): builder does not exist:" << id;
        return false;
    }
    d->builders.erase(it);
    return true;
}

bool AceTreeEntityBuilder::hasBuilder(const QString &id) const {
    return d->builders.contains(id);
}

bool AceTreeEntityBuilder::addSubBuilder(const QString &parentId, const QString &childId) {
    auto &set = d->subBuilders[parentId];
    if (set.contains(childId)) {
        return false;
    }
    set.append(childId);
    return true;
}

bool AceTreeEntityBuilder::removeSubBuilder(const QString &parentId, const QString &childId) {
    auto it = d->subBuilders.find(parentId);
    if (it == d->subBuilders.end())
        return false;

    auto &set = it.value();
    bool res = set.remove(childId);

    if (set.isEmpty())
        d->subBuilders.erase(it);

    return res;
}

bool AceTreeEntityBuilder::hasSubBuilder(const QString &parentId, const QString &childId) const {
    auto it = d->subBuilders.find(parentId);
    if (it == d->subBuilders.end())
        return false;
    return it->contains(childId);
}
