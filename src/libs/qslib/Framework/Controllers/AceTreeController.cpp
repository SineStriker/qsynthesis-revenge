#include "AceTreeController.h"
#include "AceTreeController_p.h"

#include <Collections/QMChronSet.h>

#include "ItemModel/AceTreeModel.h"

#include <QDebug>
#include <QMetaType>

#include <utility>

namespace QsApi {

    AceTreeControllerPrivate::AceTreeControllerPrivate() {
        treeItem = nullptr;
        builder = nullptr;
        operational = false;
    }

    AceTreeControllerPrivate::~AceTreeControllerPrivate() {
    }

    void AceTreeControllerPrivate::init() {
    }

    AceTreeController::AceTreeController(const QString &name, QObject *parent)
        : AceTreeController(*new AceTreeControllerPrivate(), name, parent) {
    }

    AceTreeController::~AceTreeController() {
    }

    QString AceTreeController::controllerName() const {
        Q_D(const AceTreeController);
        return d->name;
    }

    AceTreeItem *AceTreeController::treeItem() const {
        Q_D(const AceTreeController);
        return d->treeItem;
    }

    AceTreeControllerBuilder *AceTreeController::builder() const {
        Q_D(const AceTreeController);
        return d->builder;
    }

    bool AceTreeController::addChild(AceTreeController *c) {
        Q_D(AceTreeController);
        if (!c) {
            qWarning() << "QsApi::AceTreeController::addChild(): trying to add null child";
            return false;
        }
        if (d->children.contains(c->controllerName())) {
            qWarning() << "QsApi::AceTreeController::addChild(): trying to add duplicated factory:"
                       << c->controllerName();
            return false;
        }
        c->setParent(this);
        d->children.insert(c->controllerName(), c);
        return true;
    }

    bool AceTreeController::removeChild(AceTreeController *c) {
        if (c == nullptr) {
            qWarning() << "QsApi::AceTreeController::removeChild(): trying to remove null child";
            return false;
        }
        return removeChild(c->controllerName());
    }

    bool AceTreeController::removeChild(const QString &name) {
        Q_D(AceTreeController);
        auto it = d->children.find(name);
        if (it == d->children.end()) {
            qWarning() << "QsApi::AceTreeController::removeChild(): child does not exist:" << name;
            return false;
        }
        it.value()->setParent(nullptr);
        d->children.erase(it);
        return true;
    }

    AceTreeController *AceTreeController::child(const QString &name) const {
        Q_D(const AceTreeController);
        return d->children.value(name, nullptr);
    }

    void AceTreeController::setup(AceTreeItem *item) {
        Q_D(AceTreeController);

        clean();

        d->operational = true;
        setupImpl(item);
    }

    void AceTreeController::clean() {
        Q_D(AceTreeController);

        if (!d->operational)
            return;

        d->operational = false;

        cleanImpl();
    }

    bool AceTreeController::isOperational() const {
        Q_D(const AceTreeController);
        return d->operational;
    }

    void AceTreeController::preBuild() {
        // Do nothing
    }

    void AceTreeController::postBuild() {
        // Do nothing
    }

    void AceTreeController::setupImpl(AceTreeItem *item) {
        Q_D(AceTreeController);

        d->treeItem = item;
        for (const auto &child : qAsConst(d->children)) {
            auto childItem = item->nameToNode(child->controllerName());
            if (!childItem)
                continue;
            child->setup(childItem);
        }
    }

    void AceTreeController::cleanImpl() {
        Q_D(AceTreeController);

        for (const auto &child : qAsConst(d->children)) {
            child->clean();
        }
        d->treeItem = nullptr;
    }

    AceTreeController::AceTreeController(AceTreeControllerPrivate &d, const QString &name, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.name = name;

        d.init();
    }

    class AceTreeControllerBuilderPrivate {
    public:
        AceTreeControllerBuilderPrivate(AceTreeControllerBuilder *q) : q(q){};

        void init() {
        }

        AceTreeControllerBuilder *q;

        struct BuilderData {
            BuilderData(const QMetaObject *metaObject = nullptr) : metaObject(metaObject), is_meta_object(true){};
            BuilderData(AceTreeControllerBuilder::Builder builder)
                : builder(std::move(builder)), metaObject(nullptr), is_meta_object(false){};

            AceTreeController *build() const {
                if (is_meta_object) {
                    auto obj = metaObject->newInstance();
                    return qobject_cast<AceTreeController *>(obj);
                }
                return builder();
            }

            AceTreeControllerBuilder::Builder builder;
            const QMetaObject *metaObject;
            bool is_meta_object;
        };

        QHash<QString, BuilderData> builders;
        QHash<QString, QMChronSet<QString>> subBuilders;
        QString rootId;

        AceTreeController *build_helper(const QString &id) {
            auto it = builders.find(id);
            if (it == builders.end())
                return nullptr;

            auto ctl = it->build();
            if (!ctl)
                return nullptr;
            ctl->d_func()->builder = q;

            ctl->preBuild();
            for (const auto &sub : subBuilders.value(id)) {
                auto child = build_helper(sub);
                if (!child)
                    continue;

                ctl->addChild(child);
            }
            ctl->postBuild();

            return ctl;
        }
    };

    AceTreeControllerBuilder::AceTreeControllerBuilder(QObject *parent) : AceTreeControllerBuilder({}, parent) {
    }

    AceTreeControllerBuilder::AceTreeControllerBuilder(const QString &rootBuilder, QObject *parent)
        : QObject(parent), d(new AceTreeControllerBuilderPrivate(this)) {
        d->init();
        d->rootId = rootBuilder;
    }

    AceTreeControllerBuilder::~AceTreeControllerBuilder() {
        delete d;
    }

    QString AceTreeControllerBuilder::rootBuilder() const {
        return d->rootId;
    }

    void AceTreeControllerBuilder::setRootBuilder(const QString &id) {
        d->rootId = id;
    }

    AceTreeController *AceTreeControllerBuilder::build(const QString &id) const {
        return d->build_helper(id);
    }

    bool AceTreeControllerBuilder::addBuilder(const QString &id, const AceTreeControllerBuilder::Builder &builder) {
        if (!builder) {
            qWarning() << "QsApi::AceTreeController::addChild(): trying to add null builder";
            return false;
        }
        if (d->builders.contains(id)) {
            qWarning() << "QsApi::AceTreeController::addChild(): trying to add duplicated builder:" << id;
            return false;
        }
        d->builders.insert(id, builder);
        return true;
    }

    bool AceTreeControllerBuilder::addBuilder(const QString &id, const QMetaObject *metaObject) {
        if (!metaObject) {
            qWarning() << "QsApi::AceTreeControllerBuilder::addBuilder(): trying to add null builder";
            return false;
        }
        if (d->builders.contains(id)) {
            qWarning() << "QsApi::AceTreeControllerBuilder::addBuilder(): trying to add duplicated builder:" << id;
            return false;
        }
        d->builders.insert(id, metaObject);
        return true;
    }

    bool AceTreeControllerBuilder::removeBuilder(const QString &id) {
        auto it = d->builders.find(id);
        if (it == d->builders.end()) {
            qWarning() << "QsApi::AceTreeControllerBuilder::removeBuilder(): builder does not exist:" << id;
            return false;
        }
        d->builders.erase(it);
        return true;
    }

    bool AceTreeControllerBuilder::hasBuilder(const QString &id) const {
        return d->builders.contains(id);
    }

    bool AceTreeControllerBuilder::addSubBuilder(const QString &parentId, const QString &childId) {
        auto &set = d->subBuilders[parentId];
        if (set.contains(childId)) {
            return false;
        }
        set.append(childId);
        return true;
    }

    bool AceTreeControllerBuilder::removeSubBuilder(const QString &parentId, const QString &childId) {
        auto it = d->subBuilders.find(parentId);
        if (it == d->subBuilders.end())
            return false;

        auto &set = it.value();
        bool res = set.remove(childId);

        if (set.isEmpty())
            d->subBuilders.erase(it);

        return res;
    }

    bool AceTreeControllerBuilder::hasSubBuilder(const QString &parentId, const QString &childId) const {
        auto it = d->subBuilders.find(parentId);
        if (it == d->subBuilders.end())
            return false;
        return it->contains(childId);
    }

}
