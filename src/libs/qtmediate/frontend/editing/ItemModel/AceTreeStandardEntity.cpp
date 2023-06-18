#include "AceTreeStandardEntity.h"
#include "AceTreeStandardEntity_p.h"

static const char KEY_NAME_CHILD_TYPE[] = "ace_tree_child_type";

static int indent = 0;

using GlobalSpecMap = QHash<const QMetaObject *, AceTreeStandardSchema>;
Q_GLOBAL_STATIC(GlobalSpecMap, globalSpecMap)
Q_GLOBAL_STATIC(GlobalSpecMap, globalSpecMapComplete)

static inline void setTypeValueToItem(AceTreeItem *item, const QString &typeValue) {
    item->setProperty(KEY_NAME_CHILD_TYPE, typeValue);
}

static inline QString getTypeValueFromItem(const AceTreeItem *item) {
    return item->property(KEY_NAME_CHILD_TYPE).toString();
}

#define myWarning(func) (qWarning().nospace() << "AceTreeStandardEntity::" << (func) << "(): ").maybeSpace()

AceTreeStandardEntityPrivate::AceTreeStandardEntityPrivate(AceTreeStandardEntity::Type type) : type(type) {
    m_external = false;
}

AceTreeStandardEntityPrivate::~AceTreeStandardEntityPrivate() {
}

bool AceTreeStandardEntityPrivate::testModifiable(const char *func) const {
    Q_Q(const AceTreeStandardEntity);
    if (!m_treeItem->isWritable()) {
        myWarning(func) << "the entity isn't writable now" << q;
        return false;
    }
    return true;
}

bool AceTreeStandardEntityPrivate::testInsertable(const char *func, const AceTreeEntity *entity) const {
    Q_Q(const AceTreeStandardEntity);
    if (!entity) {
        myWarning(func) << "entity is null";
        return false;
    }

    if (entity == q) {
        myWarning(func) << "entity cannot be its child itself";
        return false;
    }

    // Validate
    if (!entity->isFree()) {
        myWarning(func) << "entity is not free" << entity;
        return false;
    }

    return true;
}

void AceTreeStandardEntityPrivate::rowsInserted(int index, const QVector<AceTreeItem *> &items) {
    Q_Q(AceTreeStandardEntity);
    QVector<AceTreeEntity *> entities;
    entities.reserve(items.size());
    if (m_external) {
        // Triggered by user
        for (const auto &item : items) {
            entities.append(AceTreeEntity::itemToEntity(item));
        }
    } else {
        // Triggered by undo/redo
        setupVector_helper(q->schema(), &AceTreeStandardSchema::rowTypeKey, &AceTreeStandardSchema::rowDefaultBuilder,
                           &AceTreeStandardSchema::rowBuilder, items, entities);
    }

    // Add children
    for (const auto &entity : qAsConst(entities)) {
        q->addChild(entity);
    }
}

void AceTreeStandardEntityPrivate::rowsRemoved(int index, const QVector<AceTreeItem *> &items) {
    Q_Q(AceTreeStandardEntity);

    Q_UNUSED(m_external);

    for (const auto &item : items) {
        auto entity = AceTreeEntity::itemToEntity(item);
        q->removeChild(entity);
        if (!entity->isFree())
            delete entity;
    }
}

void AceTreeStandardEntityPrivate::recordAdded(int seq, AceTreeItem *item) {
    Q_Q(AceTreeStandardEntity);

    QVector<AceTreeEntity *> entities;
    entities.reserve(1);
    if (m_external) {
        // Triggered by user
        entities.append(AceTreeEntity::itemToEntity(item));
    } else {
        // Triggered by undo/redo
        setupVector_helper(q->schema(), &AceTreeStandardSchema::recordTypeKey,
                           &AceTreeStandardSchema::recordDefaultBuilder, &AceTreeStandardSchema::recordBuilder, {item},
                           entities);
    }

    // Add children
    for (const auto &entity : qAsConst(entities)) {
        q->addChild(entity);
    }
}

void AceTreeStandardEntityPrivate::recordRemoved(int seq, AceTreeItem *item) {
    Q_Q(AceTreeStandardEntity);

    auto entity = AceTreeEntity::itemToEntity(item);
    q->removeChild(entity);
    if (!entity->isFree())
        delete entity;
}

void AceTreeStandardEntityPrivate::nodeAdded(AceTreeItem *item) {
    Q_Q(AceTreeStandardEntity);

    AceTreeEntity *entity;
    const auto &typeValue = getTypeValueFromItem(item);
    if (m_external) {
        // Triggered by user
        entity = AceTreeEntity::itemToEntity(item);
    } else {
        // Triggered by undo/redo
        // Find type value to determine the derived entity class builder
        auto builder = q->schema().nodeBuilder(typeValue);
        if (!builder.isValid())
            return; // No builder, skip

        auto child = builder();
        if (!child)
            return;

        child->setup(item);
        entity = child;
    }

    q->addChild(entity);
    childNodeMap.insert(typeValue, entity);
    addNode_assign(entity);
}

void AceTreeStandardEntityPrivate::nodeRemoved(AceTreeItem *item) {
    Q_Q(AceTreeStandardEntity);

    auto entity = AceTreeEntity::itemToEntity(item);
    const auto &typeValue = getTypeValueFromItem(item);

    removeNode_assigns(entity);
    childNodeMap.remove(typeValue);
    q->removeChild(entity);
    if (!entity->isFree())
        delete entity;
}

bool AceTreeStandardEntityPrivate::readVector_helper(const AceTreeStandardSchema &schema,
                                                     AceTreeStandardEntityPrivate::GetTypeKey getTypeKey,
                                                     AceTreeStandardEntityPrivate::GetDefaultBuilder getDefaultBuilder,
                                                     AceTreeStandardEntityPrivate::GetBuilder getBuilder,
                                                     const QJsonValue &value,
                                                     QVector<QPair<QString, AceTreeEntity *>> &childrenToAdd) {
    Q_Q(AceTreeStandardEntity);
    if (!value.isArray()) {
        return false;
    }

    const auto &typeKey = (schema.*getTypeKey)();
    if (typeKey.isEmpty()) {
        // If there's no type key
        // Consider it's an array storing non-polymorphic class, use default builder
        auto builder = (schema.*getDefaultBuilder)();
        if (!builder.isValid())
            return true; // No default builder, skip

        auto arr = value.toArray();
        for (const auto &item : qAsConst(arr)) {
            auto child = builder();
            if (!child)
                continue;

            child->initialize();

            qDebug().noquote() << std::string(indent, ' ').c_str() << "[read child]" << child->name() << child;
            if (!child->read(item)) {
                // Ignore error
                delete child;
                continue;
            }
            qDebug() << std::string(indent, ' ').c_str() << "[success]";

            childrenToAdd.append({QString(), child});
        }
    } else {
        auto arr = value.toArray();
        for (const auto &item : qAsConst(arr)) {
            const auto &typeValue = item[typeKey].toString();

            // Find type value to determine the derived entity class builder
            auto builder = (schema.*getBuilder)(typeValue);
            if (!builder.isValid())
                continue; // No builder, skip

            auto child = builder();
            if (!child)
                continue;

            child->initialize();
            qDebug().noquote() << std::string(indent, ' ').c_str() << "[read child]" << typeValue << child;
            if (!child->read(item)) {
                // Ignore error
                delete child;
                continue;
            }
            qDebug() << std::string(indent, ' ').c_str() << "[success]";

            childrenToAdd.append({typeValue, child});
        }
    }
    return true;
}

bool AceTreeStandardEntityPrivate::readSet_helper(const AceTreeStandardSchema &schema, const QJsonValue &value) {
    Q_Q(AceTreeStandardEntity);

    if (!value.isObject()) {
        return false;
    }

    auto obj = value.toObject();

    // Read dynamic data
    auto hash = schema.dynamicDataSpecHash();
    QVector<QPair<QString, QVariant>> dynamicDataToSet;
    if (!hash.isEmpty())
        qDebug().noquote() << std::string(indent, ' ').c_str() << "[collect dynamic-data]" << hash.keys();
    for (auto it = hash.begin(); it != hash.end(); ++it) {
        auto val = obj.value(it.key());
        if (val.isUndefined())
            continue;

        // Type checking (Important)
        if (val.type() != it->type())
            return false;

        dynamicDataToSet.append({it.key(), val.toVariant()});
    }
    for (const auto &pair : qAsConst(dynamicDataToSet)) {
        qDebug().noquote() << std::string(indent, ' ').c_str() << "[read dynamic-data]" << pair.first << pair.second;
        m_treeItem->setDynamicData(pair.first, pair.second);
    }

    // Read properties
    hash = schema.propertySpecHash();
    QVector<QPair<QString, QVariant>> propertiesToSet;
    if (!hash.isEmpty())
        qDebug().noquote() << std::string(indent, ' ').c_str() << "[collect properties]" << hash.keys();
    for (auto it = hash.begin(); it != hash.end(); ++it) {
        auto val = obj.value(it.key());
        if (val.isUndefined())
            continue;

        // Type checking (Important)
        if (val.type() != it->type())
            return false;

        propertiesToSet.append({it.key(), val.toVariant()});
    }
    for (const auto &pair : qAsConst(propertiesToSet)) {
        qDebug().noquote() << std::string(indent, ' ').c_str() << "[read property]" << pair.first << pair.second;
        m_treeItem->setProperty(pair.first, pair.second);
    }

    // Read node children
    for (const auto &key : schema.nodeKeys()) {
        auto val = obj.value(key);
        if (val.isUndefined())
            continue;

        auto child = childNodeMap.value(key);
        if (!child)
            continue;

        qDebug().noquote() << std::string(indent, ' ').c_str() << "[read child]" << key << child;
        if (!child->read(val))
            // Ignore
            continue;
        qDebug() << std::string(indent, ' ').c_str() << "[success]";
    }
    return true;
}

QJsonArray AceTreeStandardEntityPrivate::writeVector_helper(const AceTreeStandardSchema &schema,
                                                            const QVector<AceTreeItem *> &childItems) const {
    Q_Q(const AceTreeStandardEntity);
    Q_UNUSED(q);

    QJsonArray arr;
    auto typeKey = schema.rowTypeKey();
    if (typeKey.isEmpty()) {
        for (const auto &item : childItems) {
            auto child = AceTreeEntity::itemToEntity(item);
            if (!child)
                continue;

            QJsonValue value = child->write();
            arr.append(value);
        }
    } else {
        for (const auto &item : childItems) {
            auto child = AceTreeEntity::itemToEntity(item);
            if (!child)
                continue;

            QJsonValue value = child->write();
            auto childItem = AceTreeEntityPrivate::getItem(child);

            // Insert type value
            auto typeValue = getTypeValueFromItem(childItem);
            if (!typeValue.isEmpty() && value.isObject()) {
                // Add type value to json object
                auto obj = value.toObject();
                obj.insert(typeKey, typeValue);
                value = obj;
            }

            arr.append(value);
        }
    }
    return arr;
}

QJsonObject AceTreeStandardEntityPrivate::writeSet_helper(const AceTreeStandardSchema &schema) const {
    Q_UNUSED(schema);

    QJsonObject obj;

    // Write dynamic data
    auto hash = schema.dynamicDataSpecHash();
    for (auto it = hash.begin(); it != hash.end(); ++it) {
        auto val = m_treeItem->dynamicData(it.key());
        if (val.isValid())
            obj.insert(it.key(), QJsonValue::fromVariant(val));
        else
            obj.insert(it.key(), it.value());
    }

    // Write properties
    hash = schema.propertySpecHash();
    for (auto it = hash.begin(); it != hash.end(); ++it) {
        auto val = m_treeItem->property(it.key());
        if (val.isValid())
            obj.insert(it.key(), QJsonValue::fromVariant(val));
        else
            obj.insert(it.key(), it.value());
    }

    // Write node children
    for (auto it = childNodeMap.begin(); it != childNodeMap.end(); ++it) {
        obj.insert(it.key(), it.value()->write());
    }

    return obj;
}

void AceTreeStandardEntityPrivate::setupVector_helper(const AceTreeStandardSchema &schema, GetTypeKey getTypeKey,
                                                      GetDefaultBuilder getDefaultBuilder, GetBuilder getBuilder,
                                                      const QVector<AceTreeItem *> &items,
                                                      QVector<AceTreeEntity *> &childrenToAdd) {
    Q_Q(AceTreeStandardEntity);
    const auto &typeKey = (schema.*getTypeKey)();
    if (typeKey.isEmpty()) {
        // If there's no type key
        // Consider it's an array storing non-polymorphic class, use default builder
        auto builder = (schema.*getDefaultBuilder)();
        if (!builder.isValid())
            return; // No default builder, skip

        for (const auto &item : qAsConst(items)) {
            auto child = builder();
            if (!child)
                continue;

            child->setup(item);
            childrenToAdd.append(child);
        }
    } else {
        for (const auto &item : qAsConst(items)) {
            const auto &typeValue = getTypeValueFromItem(item);

            // Find type value to determine the derived entity class builder
            auto builder = (schema.*getBuilder)(typeValue);
            if (!builder.isValid())
                continue; // No builder, skip

            auto child = builder();
            if (!child)
                continue;

            child->setup(item);
            childrenToAdd.append(child);
        }
    }
}

void AceTreeStandardEntityPrivate::addNode_assign(AceTreeEntity *child) {
    auto it = childPostAssignRefs.find(getTypeValueFromItem(AceTreeEntityPrivate::getItem(child)));
    if (it == childPostAssignRefs.end())
        return;
    it->get() = child;
}

void AceTreeStandardEntityPrivate::removeNode_assigns(AceTreeEntity *child) {
    auto it = childPostAssignRefs.find(getTypeValueFromItem(AceTreeEntityPrivate::getItem(child)));
    if (it == childPostAssignRefs.end())
        return;
    it->get() = nullptr;
}

//===========================================================================
// AceTreeStandardEntity

AceTreeStandardEntity::AceTreeStandardEntity(Type type, QObject *parent)
    : AceTreeStandardEntity(*new AceTreeStandardEntityPrivate(type), parent) {
}

AceTreeStandardEntity::~AceTreeStandardEntity() {
}

AceTreeStandardEntity::Type AceTreeStandardEntity::type() const {
    Q_D(const AceTreeStandardEntity);
    return d->type;
}

QString AceTreeStandardEntity::name() const {
    Q_D(const AceTreeStandardEntity);
    return d->name;
}

bool AceTreeStandardEntity::read(const QJsonValue &value) {
    Q_D(AceTreeStandardEntity);

    class ABC {
    public:
        explicit ABC(int &i) : i(i) {
            i += 2;
        }
        ~ABC() {
            i -= 2;
        }

        int &i;
    };

    ABC abc(indent);

    auto schema = this->schema();
    switch (d->type) {
        case Vector: {
            QVector<QPair<QString, AceTreeEntity *>> childrenToAdd;
            if (!d->readVector_helper(schema, &AceTreeStandardSchema::rowTypeKey,
                                      &AceTreeStandardSchema::rowDefaultBuilder, &AceTreeStandardSchema::rowBuilder,
                                      value, childrenToAdd)) {
                return false;
            }
            insertRows(rowCount(), childrenToAdd);
            break;
        }

        case RecordTable: {
            QVector<QPair<QString, AceTreeEntity *>> childrenToAdd;
            if (!d->readVector_helper(schema, &AceTreeStandardSchema::recordTypeKey,
                                      &AceTreeStandardSchema::recordDefaultBuilder,
                                      &AceTreeStandardSchema::recordBuilder, value, childrenToAdd)) {
                return false;
            }
            for (const auto &pair : qAsConst(childrenToAdd)) {
                addRecord(pair.first, pair.second);
            }
            break;
        }

        case Mapping: {
            if (!d->readSet_helper(schema, value)) {
                return false;
            }
            break;
        }
        default:
            break;
    }
    return true;
}

QJsonValue AceTreeStandardEntity::write() const {
    Q_D(const AceTreeStandardEntity);

    QJsonValue res;
    switch (d->type) {
        case Vector: {
            auto schema = this->schema();
            res = d->writeVector_helper(schema, d->m_treeItem->rows());
            break;
        }

        case RecordTable: {
            auto schema = this->schema();

            // Get entities and sort
            QVector<AceTreeEntity *> entities;
            entities.reserve(d->m_treeItem->recordCount());
            for (const auto &seq : d->m_treeItem->records()) {
                auto item = d->m_treeItem->record(seq);
                entities.append(AceTreeEntity::itemToEntity(item));
            }
            sortRecords(entities);

            // Get items in the right order
            QVector<AceTreeItem *> childItems;
            for (const auto &entity : qAsConst(entities)) {
                childItems.append(AceTreeEntityPrivate::getItem(entity));
            }
            res = d->writeVector_helper(schema, childItems);
            break;
        }

        case Mapping: {
            auto schema = this->schema();
            res = d->writeSet_helper(schema);
            break;
        }

        default:
            break;
    }

    return res;
}

void AceTreeStandardEntity::sortRecords(QVector<AceTreeEntity *> &records) const {
    // Do nothing
}

template <class K, template <class> class List, class T>
static void unionHashCollections(QHash<K, List<T>> &to, const QHash<K, List<T>> &from) {
    for (auto it = from.begin(); it != from.end(); ++it) {
        auto it2 = to.find(it.key());
        if (it2 == to.end()) {
            it2 = to.insert(it.key(), {});
        }
        it2->reserve(it2->capacity() + it->size());
        for (const auto &item : qAsConst(it.value())) {
            it2->append(item);
        }
    }
}

template <class K, class T>
static void unionHash(QHash<K, T> &to, const QHash<K, T> &from) {
    for (auto it = from.begin(); it != from.end(); ++it) {
        if (to.contains(it.key()))
            continue;
        to.insert(it.key(), it.value());
    }
}

AceTreeStandardSchema AceTreeStandardEntity::schema() const {
    Q_D(const AceTreeStandardEntity);

    auto mo = metaObject();
    {
        // Search in cache
        auto it = globalSpecMapComplete->find(mo);
        if (it != globalSpecMapComplete->end()) {
            return it.value();
        }
    }

    auto mo0 = mo;

    // Get and cache schema
    auto thisSchema = AceTreeStandardSchema::globalSchema(mo);
    auto d1 = thisSchema.d.data();
    mo = mo->superClass();

    while (mo != &AceTreeStandardEntity::staticMetaObject) {
        bool ok;
        auto schema = AceTreeStandardSchema::globalSchema(mo, &ok);
        if (!ok) {
            mo = mo->superClass();
            continue;
        }

        // Add super class data
        auto d2 = schema.d.constData();
        unionHash(d1->dynamicData, d2->dynamicData);
        unionHash(d1->properties, d2->properties);

        unionHash(d1->rowBuilders, d2->rowBuilders);
        unionHashCollections(d1->rowBuilderIndexes, d2->rowBuilderIndexes);
        if (!d1->rowDefaultBuilder.isValid()) {
            d1->rowDefaultBuilder = d2->rowDefaultBuilder;
        }

        unionHash(d1->recordBuilders, d2->recordBuilders);
        unionHashCollections(d1->recordBuilderIndexes, d2->recordBuilderIndexes);
        if (!d1->recordDefaultBuilder.isValid()) {
            d1->recordDefaultBuilder = d2->recordDefaultBuilder;
        }

        unionHash(d1->nodeBuilders, d2->nodeBuilders);
        unionHashCollections(d1->nodeBuilderIndexes, d2->nodeBuilderIndexes);

        mo = mo->superClass();
    }

    globalSpecMapComplete->insert(mo0, thisSchema);

    return thisSchema;
}

QString AceTreeStandardEntity::searchChildType(const QMetaObject *metaObject) const {
    Q_D(const AceTreeStandardEntity);

    auto schema = this->schema();
    typedef QString (AceTreeStandardSchema::*SearchType)(const QMetaObject *) const;
    SearchType func = nullptr;
    switch (d->type) {
        case Vector:
            if (schema.rowTypeKey().isEmpty()) {
                return {};
            }
            func = &AceTreeStandardSchema::rowType;
            break;
        case RecordTable:
            if (schema.recordTypeKey().isEmpty()) {
                return {};
            }
            func = &AceTreeStandardSchema::recordType;
            break;
        case Mapping:
            func = &AceTreeStandardSchema::nodeKey;
            break;
        default:
            return {};
            break;
    }

    QString res;
    while (metaObject != &AceTreeStandardEntity::staticMetaObject) {
        res = (schema.*func)(metaObject);
        if (!res.isEmpty()) {
            break;
        }
        metaObject = metaObject->superClass();
    }
    return res;
}

AceTreeEntity *AceTreeStandardEntity::childTree(const QString &key) const {
    Q_D(const AceTreeStandardEntity);
    return d->childNodeMap.value(key);
}

QStringList AceTreeStandardEntity::childTreeKeys() const {
    Q_D(const AceTreeStandardEntity);
    return d->childNodeMap.keys();
}

int AceTreeStandardEntity::childTreeCount() const {
    Q_D(const AceTreeStandardEntity);
    return d->childNodeMap.size();
}

void AceTreeStandardEntity::doInitialize() {
    Q_D(AceTreeStandardEntity);

    // Add subscriber
    auto &treeItem = d->m_treeItem;

    switch (d->type) {
        case Mapping: {
            auto schema = this->schema();

            // Set default dynamic data
            auto hash = schema.dynamicDataSpecHash();
            for (auto it = hash.begin(); it != hash.end(); ++it) {
                treeItem->setDynamicData(it.key(), it->toVariant());
            }

            // Set default properties
            hash = schema.propertySpecHash();
            for (auto it = hash.begin(); it != hash.end(); ++it) {
                treeItem->setProperty(it.key(), it->toVariant());
            }

            // Set default node children
            for (const auto &key : schema.nodeKeys()) {
                auto builder = schema.nodeBuilder(key);
                auto child = builder();
                if (!child)
                    continue;

                child->initialize();
                addNode(key, child);
            }
            break;
        }

        default:
            break;
    }
}

void AceTreeStandardEntity::doSetup() {
    Q_D(AceTreeStandardEntity);

    // Add subscriber
    auto &treeItem = d->m_treeItem;

    switch (d->type) {
        case Vector: {
            d->rowsInserted(0, d->m_treeItem->rows());
            break;
        }
        case RecordTable: {
            for (auto seq : d->m_treeItem->records()) {
                auto childItem = treeItem->record(seq);
                d->recordAdded(seq, childItem);
            }
            break;
        }
        case Mapping: {
            for (auto childItem : d->m_treeItem->nodes()) {
                d->nodeAdded(childItem);
            }
            break;
        }
        default:
            break;
    }
}

AceTreeStandardEntity::AceTreeStandardEntity(AceTreeStandardEntityPrivate &d, QObject *parent)
    : AceTreeEntity(d, parent) {
}

QVariant AceTreeStandardEntity::dynamicData(const QString &key) const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->dynamicData(key);
}

void AceTreeStandardEntity::setDynamicData(const QString &key, const QVariant &value) {
    Q_D(AceTreeStandardEntity);
    d->m_treeItem->setDynamicData(key, value);
}

QVariantHash AceTreeStandardEntity::dynamicDataMap() const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->dynamicDataMap();
}

QVariant AceTreeStandardEntity::property(const QString &key) const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->property(key);
}

bool AceTreeStandardEntity::setProperty(const QString &key, const QVariant &value) {
    Q_D(AceTreeStandardEntity);
    return d->m_treeItem->setProperty(key, value);
}

QVariantHash AceTreeStandardEntity::properties() const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->propertyMap();
}

bool AceTreeStandardEntity::insertRows(int index, const QVector<QPair<QString, AceTreeEntity *>> &entities) {
    Q_D(AceTreeStandardEntity);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    for (const auto &pair : entities) {
        auto &child = pair.second;
        if (!d->testInsertable(__func__, child)) {
            return false;
        }
    }

    QVector<AceTreeItem *> childItems;
    childItems.reserve(entities.size());
    for (const auto &pair : entities) {
        auto &child = pair.second;
        auto childItem = AceTreeEntityPrivate::getItem(child);
        setTypeValueToItem(childItem, pair.first); // Set type value
        childItems.append(childItem);
    }

    // Commit change to model
    d->m_external = true;
    auto res = d->m_treeItem->insertRows(index, childItems);
    d->m_external = false;
    return res;
}

bool AceTreeStandardEntity::moveRows(int index, int count, int dest) {
    Q_D(AceTreeStandardEntity);

    if (!d->testModifiable(__func__))
        return false;

    // Commit change to model
    d->m_external = true;
    auto res = d->m_treeItem->moveRows(index, count, dest);
    d->m_external = false;
    return res;
}

bool AceTreeStandardEntity::removeRows(int index, int count) {
    Q_D(AceTreeStandardEntity);

    if (!d->testModifiable(__func__))
        return false;

    // Commit change to model
    d->m_external = true;
    auto res = d->m_treeItem->removeRows(index, count);
    d->m_external = false;
    return res;
}

AceTreeEntity *AceTreeStandardEntity::row(int row) const {
    Q_D(const AceTreeStandardEntity);
    return AceTreeEntity::itemToEntity(d->m_treeItem->row(row));
}

QVector<AceTreeEntity *> AceTreeStandardEntity::rows() const {
    Q_D(const AceTreeStandardEntity);
    QVector<AceTreeEntity *> res;
    res.reserve(d->m_treeItem->rowCount());
    for (const auto &item : d->m_treeItem->rows()) {
        auto child = AceTreeEntity::itemToEntity(item);
        if (!child)
            continue;
        res.append(child);
    }
    return res;
}

int AceTreeStandardEntity::rowIndexOf(AceTreeEntity *entity) const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->rowIndexOf(AceTreeEntityPrivate::getItem(entity));
}

int AceTreeStandardEntity::rowCount() const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->rowCount();
}

int AceTreeStandardEntity::addRecord(const QString &key, AceTreeEntity *entity) {
    Q_D(AceTreeStandardEntity);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!d->testInsertable(__func__, entity)) {
        return false;
    }

    auto childItem = AceTreeEntityPrivate::getItem(entity);
    setTypeValueToItem(childItem, key); // Set type value

    // Commit change to model
    d->m_external = true;
    auto res = d->m_treeItem->addRecord(childItem);
    d->m_external = false;
    return res;
}

bool AceTreeStandardEntity::removeRecord(int seq) {
    Q_D(AceTreeStandardEntity);

    if (!d->testModifiable(__func__))
        return false;

    // Commit change to model
    d->m_external = true;
    auto res = d->m_treeItem->removeRecord(seq);
    d->m_external = false;
    return res;
}

bool AceTreeStandardEntity::removeRecord(AceTreeEntity *entity) {
    Q_D(AceTreeStandardEntity);

    if (!d->testModifiable(__func__))
        return false;

    if (!entity) {
        myWarning(__func__) << "trying to remove a null record from" << this;
        return false;
    }

    // Commit change to model
    d->m_external = true;
    auto res = d->m_treeItem->removeRecord(AceTreeEntityPrivate::getItem(entity));
    d->m_external = false;
    return res;
}

AceTreeEntity *AceTreeStandardEntity::record(int seq) {
    Q_D(const AceTreeStandardEntity);
    return AceTreeEntity::itemToEntity(d->m_treeItem->record(seq));
}

int AceTreeStandardEntity::recordIndexOf(AceTreeEntity *entity) const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->recordIndexOf(AceTreeEntityPrivate::getItem(entity));
}

QList<int> AceTreeStandardEntity::records() const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->records();
}

int AceTreeStandardEntity::recordCount() const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->recordCount();
}

int AceTreeStandardEntity::maxRecordSeq() const {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->maxRecordSeq();
}

bool AceTreeStandardEntity::containsNode(AceTreeEntity *entity) {
    Q_D(const AceTreeStandardEntity);
    return d->m_treeItem->containsNode(AceTreeEntityPrivate::getItem(entity));
}

QList<AceTreeEntity *> AceTreeStandardEntity::nodes() const {
    Q_D(const AceTreeStandardEntity);
    return d->childNodeMap.values();
}

int AceTreeStandardEntity::nodeCount() const {
    Q_D(const AceTreeStandardEntity);
    return d->childNodeMap.size();
}

QStringList AceTreeStandardEntity::nodeKeys() const {
    Q_D(const AceTreeStandardEntity);
    return d->childNodeMap.keys();
}

AceTreeEntity *AceTreeStandardEntity::node(const QString &key) const {
    Q_D(const AceTreeStandardEntity);
    return d->childNodeMap.value(key, nullptr);
}

bool AceTreeStandardEntity::addNode(const QString &key, AceTreeEntity *entity) {
    Q_D(AceTreeStandardEntity);

    if (!d->testModifiable(__func__))
        return false;

    // Validate
    if (!d->testInsertable(__func__, entity)) {
        return false;
    }

    auto childItem = AceTreeEntityPrivate::getItem(entity);
    setTypeValueToItem(childItem, key); // Set type value

    // Commit change to model
    d->m_external = true;
    auto res = d->m_treeItem->addNode(childItem);
    d->m_external = false;
    return res;
}

bool AceTreeStandardEntity::removeNode(AceTreeEntity *entity) {
    Q_D(AceTreeStandardEntity);

    if (!d->testModifiable(__func__))
        return false;

    if (!entity) {
        myWarning(__func__) << "trying to remove a null node from" << this;
        return false;
    }

    // Commit change to model
    d->m_external = true;
    auto res = d->m_treeItem->removeNode(AceTreeEntityPrivate::getItem(entity));
    d->m_external = false;
    return res;
}

//===========================================================================


//===========================================================================
// AceTreeStandardSchema

AceTreeStandardSchema::AceTreeStandardSchema() : d(new AceTreeStandardSchemaData()) {
}

AceTreeStandardSchema::~AceTreeStandardSchema() {
}

AceTreeStandardSchema::AceTreeStandardSchema(const AceTreeStandardSchema &other) : d(other.d) {
}

AceTreeStandardSchema::AceTreeStandardSchema(AceTreeStandardSchema &&other) noexcept {
    qSwap(d, other.d);
}

AceTreeStandardSchema &AceTreeStandardSchema::operator=(const AceTreeStandardSchema &other) = default;

AceTreeStandardSchema &AceTreeStandardSchema::operator=(AceTreeStandardSchema &&other) noexcept {
    qSwap(d, other.d);
    return *this;
}

QJsonValue AceTreeStandardSchema::propertySpec(const QString &key) const {
    return d->properties.value(key);
}

void AceTreeStandardSchema::setPropertySpec(const QString &key, const QJsonValue &defaultValue) {
    if (defaultValue.isUndefined()) {
        d->properties.remove(key);
    } else {
        d->properties.insert(key, defaultValue);
    }
}

QStringList AceTreeStandardSchema::propertySpecKeys() const {
    return d->properties.keys();
}

QHash<QString, QJsonValue> AceTreeStandardSchema::propertySpecHash() const {
    return d->properties;
}

QJsonValue AceTreeStandardSchema::dynamicDataSpec(const QString &key) const {
    return d->dynamicData.value(key);
}

void AceTreeStandardSchema::setDynamicDataSpec(const QString &key, const QJsonValue &defaultValue) {
    if (defaultValue.isUndefined()) {
        d->dynamicData.remove(key);
    } else {
        d->dynamicData.insert(key, defaultValue);
    }
}

QStringList AceTreeStandardSchema::dynamicDataSpecKeys() const {
    return d->dynamicData.keys();
}

QHash<QString, QJsonValue> AceTreeStandardSchema::dynamicDataSpecHash() const {
    return d->dynamicData;
}

QString AceTreeStandardSchema::rowTypeKey() const {
    return d->rowTypeKey;
}

void AceTreeStandardSchema::setRowTypeKey(const QString &key) {
    d->rowTypeKey = key;
}

AceTreeEntityBuilder AceTreeStandardSchema::rowBuilder(const QString &type) const {
    return d->rowBuilders.value(type, nullptr);
}

static void setBuilder_helper(QHash<QString, AceTreeEntityBuilder> &builders,
                              QHash<const QMetaObject *, QMChronSet<QString>> &indexes, const QString &type,
                              const AceTreeEntityBuilder &builder) {
    // Remove old
    auto it = builders.find(type);
    if (it != builders.end()) {
        auto it2 = indexes.find(it->metaObject());
        it2->remove(type);
        if (it2->isEmpty()) {
            indexes.erase(it2);
        }
    }

    if (!builder.isValid())
        return;

    // Insert new
    builders.insert(type, builder);
    indexes[builder.metaObject()].append(type);
}

void AceTreeStandardSchema::setRowBuilder(const QString &type, const AceTreeEntityBuilder &builder) {
    setBuilder_helper(d->rowBuilders, d->rowBuilderIndexes, type, builder);
}

QStringList AceTreeStandardSchema::rowTypes() const {
    return d->rowBuilders.keys();
}

QString AceTreeStandardSchema::rowType(const QMetaObject *metaObject) const {
    auto it = d->rowBuilderIndexes.find(metaObject);
    if (it == d->rowBuilderIndexes.end())
        return {};
    return *it->begin();
}

QStringList AceTreeStandardSchema::rowTypes(const QMetaObject *metaObject) const {
    return d->rowBuilderIndexes.value(metaObject).values();
}

int AceTreeStandardSchema::rowTypeCount() const {
    return d->rowBuilders.size();
}

AceTreeEntityBuilder AceTreeStandardSchema::rowDefaultBuilder() const {
    return d->rowDefaultBuilder;
}

void AceTreeStandardSchema::setRowDefaultBuilder(const AceTreeEntityBuilder &builder) {
    d->rowDefaultBuilder = builder;
}

QString AceTreeStandardSchema::recordTypeKey() const {
    return d->rowTypeKey;
}

void AceTreeStandardSchema::setRecordTypeKey(const QString &key) {
    d->rowTypeKey = key;
}

AceTreeEntityBuilder AceTreeStandardSchema::recordBuilder(const QString &type) const {
    return d->recordBuilders.value(type, nullptr);
}

void AceTreeStandardSchema::setRecordBuilder(const QString &type, const AceTreeEntityBuilder &builder) {
    setBuilder_helper(d->recordBuilders, d->recordBuilderIndexes, type, builder);
}

QStringList AceTreeStandardSchema::recordTypes() const {
    return d->recordBuilders.keys();
}

QString AceTreeStandardSchema::recordType(const QMetaObject *metaObject) const {
    auto it = d->recordBuilderIndexes.find(metaObject);
    if (it == d->recordBuilderIndexes.end())
        return {};
    return *it->begin();
}

QStringList AceTreeStandardSchema::recordTypes(const QMetaObject *metaObject) const {
    return d->recordBuilderIndexes.value(metaObject).values();
}

int AceTreeStandardSchema::recordTypeCount() const {
    return d->recordBuilders.size();
}

AceTreeEntityBuilder AceTreeStandardSchema::recordDefaultBuilder() const {
    return d->recordDefaultBuilder;
}

void AceTreeStandardSchema::setRecordDefaultBuilder(const AceTreeEntityBuilder &builder) {
    d->recordDefaultBuilder = builder;
}

AceTreeEntityBuilder AceTreeStandardSchema::nodeBuilder(const QString &key) const {
    return d->nodeBuilders.value(key, nullptr);
}

QStringList AceTreeStandardSchema::nodeKeys() const {
    return d->nodeBuilders.keys();
}

QString AceTreeStandardSchema::nodeKey(const QMetaObject *metaObject) const {
    auto it = d->nodeBuilderIndexes.find(metaObject);
    if (it == d->nodeBuilderIndexes.end())
        return {};
    return *it->begin();
}

QStringList AceTreeStandardSchema::nodeKeys(const QMetaObject *metaObject) const {
    return d->nodeBuilderIndexes.value(metaObject).values();
}

int AceTreeStandardSchema::nodeBuilderCount() const {
    return d->nodeBuilders.size();
}

void AceTreeStandardSchema::setNodeBuilder(const QString &key, const AceTreeEntityBuilder &builder) {
    setBuilder_helper(d->nodeBuilders, d->nodeBuilderIndexes, key, builder);
}

AceTreeStandardSchema &AceTreeStandardSchema::globalSchemaRef(const QMetaObject *metaObject) {
    return (*globalSpecMap)[metaObject];
}

AceTreeStandardSchema AceTreeStandardSchema::globalSchema(const QMetaObject *metaObject, bool *ok) {
    auto it = globalSpecMap->find(metaObject);
    if (it == globalSpecMap->end()) {
        if (ok)
            *ok = false;
        return {};
    }
    if (ok)
        *ok = true;
    return it.value();
}

void AceTreeStandardSchema::setGlobalSchema(const QMetaObject *metaObject, const AceTreeStandardSchema &schema) {
    globalSpecMap->insert(metaObject, schema);
}

void AceTreeStandardSchema::clearGlobalSchemaCache(const QMetaObject *metaObject) {
    if (!metaObject) {
        // Clear all cache
        globalSpecMapComplete->clear();
        return;
    }

    // Clear derived classes' cache
    for (auto it = globalSpecMapComplete->begin(); it != globalSpecMapComplete->end();) {
        if (it.key()->inherits(metaObject)) {
            it = globalSpecMapComplete->erase(it);
            continue;
        }
        it++;
    }
}

//===========================================================================


//===========================================================================
// AceTreeEntityVector

using VectorSignals = AceTreeEntityVectorPrivate::Signals;
using VectorSignalIndexes = QHash<const QMetaObject *, QSharedPointer<VectorSignals>>;
Q_GLOBAL_STATIC(VectorSignalIndexes, vectorIndexes)

static VectorSignals *findVectorSignal(const QMetaObject *metaObject) {
    auto it = vectorIndexes->find(metaObject);
    if (it != vectorIndexes->end()) {
        return it->data();
    }

    VectorSignals s;
    QHash<QString, QPair<QMetaMethod *, bool (*)(QMetaMethod *)>> fields{
        {
         "inserted", {
                &s.inserted,
                [](QMetaMethod *method) {
                    return QRegularExpression(R"(inserted\(int,QVector<\w+\*>\))")
                        .match(method->methodSignature())
                        .hasMatch(); //
                },
            }, },
        {"aboutToMove",
         {
             &s.aboutToMove,
             [](QMetaMethod *method) {
                 return method->methodSignature() == "aboutToMove(int,int,int)"; //
             },
         }},
        {"moved",
         {
             &s.moved,
             [](QMetaMethod *method) {
                 return method->methodSignature() == "moved(int,int,int)"; //
             },
         }},
        {"aboutToRemove",
         {
             &s.aboutToRemove,
             [](QMetaMethod *method) {
                 return QRegularExpression(R"(aboutToRemove\(int,QVector<\w+\*>\))")
                     .match(method->methodSignature())
                     .hasMatch(); //
             },
         }},
        {"removed",
         {
             &s.removed,
             [](QMetaMethod *method) {
                 return method->methodSignature() == "removed(int,int)"; //
             },
         }},
    };

    int methodCount = metaObject->methodCount();

    for (int i = 0; i < methodCount; ++i) {
        QMetaMethod method = metaObject->method(i);
        if (method.methodType() == QMetaMethod::Signal) {
            auto it1 = fields.find(method.name());
            if (it1 == fields.end())
                continue;
            auto &ref = it1.value();
            if (!ref.second(&method))
                continue;
            *ref.first = method;
        }
    }

    it = vectorIndexes->insert(metaObject, QSharedPointer<VectorSignals>::create(s));
    return it->data();
}

AceTreeEntityVectorPrivate::AceTreeEntityVectorPrivate() : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Vector) {
    _signals = nullptr;
}

AceTreeEntityVectorPrivate::~AceTreeEntityVectorPrivate() {
}

void AceTreeEntityVectorPrivate::init_deferred() {
    AceTreeStandardEntityPrivate::init_deferred();

    Q_Q(AceTreeEntityVector);
    _signals = findVectorSignal(q->metaObject());
}

void AceTreeEntityVectorPrivate::rowsInserted(int index, const QVector<AceTreeItem *> &items) {
    AceTreeStandardEntityPrivate::rowsInserted(index, items);
    auto &mm = _signals->inserted;
    if (!mm.isValid()) {
        return;
    }

    Q_Q(AceTreeEntityVector);
    mm.invoke(q,                                                                 //
              QGenericArgument("int", &index),                                   //
              QGenericArgument(QMetaType::typeName(mm.parameterType(1)), &items) //
    );
}

void AceTreeEntityVectorPrivate::rowsAboutToMove(int index, int count, int dest) {
    AceTreeItemSubscriber::rowsAboutToMove(index, count, dest);
    auto &mm = _signals->aboutToMove;
    if (!mm.isValid()) {
        return;
    }

    Q_Q(AceTreeEntityVector);
    mm.invoke(q,                               //
              QGenericArgument("int", &index), //
              QGenericArgument("int", &count), //
              QGenericArgument("int", &dest)   //
    );
}

void AceTreeEntityVectorPrivate::rowsMoved(int index, int count, int dest) {
    AceTreeItemSubscriber::rowsMoved(index, count, dest);
    auto &mm = _signals->moved;
    if (!mm.isValid()) {
        return;
    }
    Q_Q(AceTreeEntityVector);
    mm.invoke(q,                               //
              QGenericArgument("int", &index), //
              QGenericArgument("int", &count), //
              QGenericArgument("int", &dest)   //
    );
}

void AceTreeEntityVectorPrivate::rowsAboutToRemove(int index, const QVector<AceTreeItem *> &items) {
    AceTreeItemSubscriber::rowsAboutToRemove(index, items);
    auto &mm = _signals->aboutToRemove;
    if (!mm.isValid()) {
        return;
    }
    Q_Q(AceTreeEntityVector);
    mm.invoke(q,                                                                 //
              QGenericArgument("int", &index),                                   //
              QGenericArgument(QMetaType::typeName(mm.parameterType(1)), &items) //
    );
}

void AceTreeEntityVectorPrivate::rowsRemoved(int index, const QVector<AceTreeItem *> &items) {
    AceTreeStandardEntityPrivate::rowsRemoved(index, items);
    auto &mm = _signals->removed;
    if (!mm.isValid()) {
        return;
    }
}

AceTreeEntityVector::AceTreeEntityVector(QObject *parent)
    : AceTreeEntityVector(*new AceTreeEntityVectorPrivate(), parent) {
}

AceTreeEntityVector::~AceTreeEntityVector() {
}

AceTreeEntityVector::AceTreeEntityVector(AceTreeEntityVectorPrivate &d, QObject *parent)
    : AceTreeStandardEntity(d, parent) {
    d.init();
}

//===========================================================================


//===========================================================================
// AceTreeEntityRecordTable

using RecordTableSignals = AceTreeEntityRecordTablePrivate::Signals;
using RecordTableSignalIndexes = QHash<const QMetaObject *, QSharedPointer<RecordTableSignals>>;
Q_GLOBAL_STATIC(RecordTableSignalIndexes, recordTableIndexes)

static RecordTableSignals *findRecordTableSignal(const QMetaObject *metaObject) {
    auto it = recordTableIndexes->find(metaObject);
    if (it != recordTableIndexes->end()) {
        return it->data();
    }


    RecordTableSignals s;
    QHash<QString, QPair<QMetaMethod *, bool (*)(QMetaMethod *)>> fields{
        {"inserted",
         {
             &s.inserted,
             [](QMetaMethod *method) {
                 return QRegularExpression(R"(inserted\(int,\w+\*\))").match(method->methodSignature()).hasMatch(); //
             },
         }},
        {"aboutToRemove",
         {
             &s.aboutToRemove,
             [](QMetaMethod *method) -> bool {
                 return QRegularExpression(R"(aboutToRemove\(int,\w+\*\))")
                     .match(method->methodSignature())
                     .hasMatch(); //
             },
         }},
        {"removed",
         {
             &s.removed,
             [](QMetaMethod *method) -> bool {
                 return method->methodSignature() == "removed(int)"; //
             },
         }},
    };

    int methodCount = metaObject->methodCount();
    for (int i = 0; i < methodCount; ++i) {
        QMetaMethod method = metaObject->method(i);
        if (method.methodType() == QMetaMethod::Signal) {
            auto it1 = fields.find(method.name());
            if (it1 == fields.end())
                continue;
            auto &ref = it1.value();
            if (!ref.second(&method))
                continue;
            *ref.first = method;
        }
    }

    it = recordTableIndexes->insert(metaObject, QSharedPointer<RecordTableSignals>::create(s));
    return it->data();
}

AceTreeEntityRecordTablePrivate::AceTreeEntityRecordTablePrivate()
    : AceTreeStandardEntityPrivate(AceTreeStandardEntity::RecordTable) {
    _signals = nullptr;
}

AceTreeEntityRecordTablePrivate::~AceTreeEntityRecordTablePrivate() {
}

void AceTreeEntityRecordTablePrivate::init_deferred() {
    AceTreeStandardEntityPrivate::init_deferred();

    Q_Q(AceTreeStandardEntity);
    _signals = findRecordTableSignal(q->metaObject());
}

void AceTreeEntityRecordTablePrivate::recordAdded(int seq, AceTreeItem *item) {
    AceTreeStandardEntityPrivate::recordAdded(seq, item);

    auto &mm = _signals->inserted;
    if (!mm.isValid()) {
        return;
    }
    Q_Q(AceTreeEntityRecordTable);

    mm.invoke(q,                                                                //
              QGenericArgument("int", &seq),                                    //
              QGenericArgument(QMetaType::typeName(mm.parameterType(1)), &item) //
    );
}
void AceTreeEntityRecordTablePrivate::recordAboutToRemove(int seq, AceTreeItem *item) {
    AceTreeItemSubscriber::recordAboutToRemove(seq, item);
    auto &mm = _signals->aboutToRemove;
    if (!mm.isValid()) {
        return;
    }
    Q_Q(AceTreeEntityRecordTable);
    mm.invoke(q,                                                                //
              QGenericArgument("int", &seq),                                    //
              QGenericArgument(QMetaType::typeName(mm.parameterType(1)), &item) //
    );
}
void AceTreeEntityRecordTablePrivate::recordRemoved(int seq, AceTreeItem *item) {
    AceTreeStandardEntityPrivate::recordRemoved(seq, item);
    auto &mm = _signals->removed;
    if (!mm.isValid()) {
        return;
    }
    Q_Q(AceTreeEntityRecordTable);
    mm.invoke(q,                                                                //
              QGenericArgument("int", &seq),                                    //
              QGenericArgument(QMetaType::typeName(mm.parameterType(1)), &item) //
    );
}

AceTreeEntityRecordTable::AceTreeEntityRecordTable(QObject *parent)
    : AceTreeEntityRecordTable(*new AceTreeEntityRecordTablePrivate(), parent) {
}

AceTreeEntityRecordTable::~AceTreeEntityRecordTable() {
}

AceTreeEntityRecordTable::AceTreeEntityRecordTable(AceTreeEntityRecordTablePrivate &d, QObject *parent)
    : AceTreeStandardEntity(d, parent) {
    d.init();
}

//===========================================================================


//===========================================================================
// AceTreeEntityMapping

AceTreeEntityMappingPrivate::AceTreeEntityMappingPrivate()
    : AceTreeStandardEntityPrivate(AceTreeStandardEntity::Mapping) {
}

AceTreeEntityMappingPrivate::~AceTreeEntityMappingPrivate() {
}

AceTreeEntityMapping::AceTreeEntityMapping(QObject *parent)
    : AceTreeEntityMapping(*new AceTreeEntityMappingPrivate(), parent) {
}

AceTreeEntityMapping::~AceTreeEntityMapping() {
}

AceTreeEntityMapping::AceTreeEntityMapping(AceTreeEntityMappingPrivate &d, QObject *parent)
    : AceTreeStandardEntity(d, parent) {
    d.init();
}

void AceTreeEntityMappingPrivate::dynamicDataChanged(const QString &key, const QVariant &newValue,
                                                     const QVariant &oldValue) {
    Q_Q(AceTreeEntityMapping);
    auto it = dynamicPropertyNotifiers.find(key);
    if (it == dynamicPropertyNotifiers.end())
        return;
    it.value()(q, newValue, oldValue);
}

void AceTreeEntityMappingPrivate::propertyChanged(const QString &key, const QVariant &newValue,
                                                  const QVariant &oldValue) {
    Q_Q(AceTreeEntityMapping);
    auto it = propertyNotifiers.find(key);
    if (it == propertyNotifiers.end())
        return;
    it.value()(q, newValue, oldValue);
}

//===========================================================================