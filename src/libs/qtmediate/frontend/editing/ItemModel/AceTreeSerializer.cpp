#include "AceTreeSerializer.h"

#include <QHash>
#include <QStack>

#include <set>

class AceTreeSerializerPrivate {
public:
    AceTreeSerializerPrivate(AceTreeSerializer *q);
    ~AceTreeSerializerPrivate();

    void init();

    AceTreeSerializer *q;

    QString id;
    QJsonValue::Type preferredType;

    void *userdata;
    QString selfKey;

    AceTreeSerializer::ArrayOptions arrayOptions;
    AceTreeSerializer::ObjectOptions objectOptions;

    AceTreeSerializer *root;
    AceTreeSerializer *parent;

    QHash<QString, AceTreeSerializer *> childKeyIndexes;
    QHash<AceTreeSerializer *, QString> childNodeIndexes;

    QHash<QString, AceTreeSerializer *> rootIndexes; // Only root item own this

    bool addChild(const QString &key, AceTreeSerializer *child);
    bool removeChild(const QString &_id, AceTreeSerializer **child_ref = nullptr);
};

AceTreeSerializerPrivate::AceTreeSerializerPrivate(AceTreeSerializer *q)
    : q(q), preferredType(QJsonValue::Null), userdata(nullptr), root(q), parent(nullptr),
      arrayOptions({{}, {}, {}, {}, {}, {}}), objectOptions({{}, {}, {}, {}, {}, {}}) {
}

AceTreeSerializerPrivate::~AceTreeSerializerPrivate() {
    if (parent) {
        parent->removeChild(q);
    }
    qDeleteAll(childKeyIndexes);
}

void AceTreeSerializerPrivate::init() {
}

bool AceTreeSerializerPrivate::addChild(const QString &key, AceTreeSerializer *child) {
    if (key.isEmpty()) {
        qWarning() << "AceTreeSerializer::addChild(): trying to add empty key";
        return false;
    }

    if (!child) {
        qWarning() << "AceTreeSerializer::addChild(): trying to add null child";
        return false;
    }

    auto &_parent = child->d->parent;
    if (_parent) {
        qWarning() << "AceTreeSerializer::addChild(): trying to add a child which already has a parent"
                   << _parent;
        return false;
    }

    if (q->child(child->id())) {
        qWarning() << "AceTreeSerializer::addChild(): trying to add duplicated child:" << child->id();
        return false;
    }

    if (childKeyIndexes.contains(key)) {
        qWarning() << "AceTreeSerializer::addChild(): trying to add duplicated key or index" << key;
        return false;
    }

    auto &d1 = child->d;
    auto &d2 = root->d;

    if (d1->rootIndexes.keys().toSet().intersects(d2->rootIndexes.keys().toSet())) {
        qWarning() << "AceTreeSerializer::addChild(): some children of two trees have duplicated id:" << child;
        return false;
    }

    // Add parent index
    childKeyIndexes.insert(key, child);
    childNodeIndexes.insert(child, key);
    _parent = q;

    // Add root index
    d2->rootIndexes.insert(d1->id, child);
    d1->root = root;

    // Propagate
    for (const auto &item : qAsConst(d1->rootIndexes)) {
        item->d->root = root;
    }

    // Transfer root
    d2->rootIndexes.insert(d1->rootIndexes);
    d1->rootIndexes.clear();

    return true;
}

bool AceTreeSerializerPrivate::removeChild(const QString &_id, AceTreeSerializer **child_ref) {
    auto &d2 = root->d;

    auto it = d2->rootIndexes.find(_id);
    if (it == d2->childKeyIndexes.end()) {
        qWarning() << "AceTreeSerializer::removeChild(): child does not exist:" << _id;
        return false;
    }

    auto child = it.value();
    auto &_parent = child->d->parent;

    // Remove parent index
    if (_parent) {
        auto &d3 = _parent->d;
        auto it2 = d3->childNodeIndexes.find(child); // Find key
        d3->childKeyIndexes.remove(it2.value());     // Remove key index
        d3->childNodeIndexes.erase(it2);             // Remove node index
    }
    _parent = nullptr;

    // Remove root index
    d2->rootIndexes.erase(it);
    child->d->root = child;

    // Build root
    QHash<QString, AceTreeSerializer *> indexes;
    for (const auto &item : child->children()) {
        item->d->root = child; // Propagate
        indexes.insert(item->id(), item);

        d2->rootIndexes.remove(item->id());
    }

    child->d->rootIndexes = std::move(indexes);

    if (child_ref)
        *child_ref = child;

    return true;
}

static QJsonValue variant_to_jsonValue(const QVariant &var) {
    auto res = var.toJsonValue();
    if (res.isUndefined())
        res = QJsonValue::Null;
    return res;
}

AceTreeSerializer::AceTreeSerializer(const QString &id) : AceTreeSerializer(id, QJsonValue::Null) {
}

AceTreeSerializer::AceTreeSerializer(const QString &id, QJsonValue::Type preferredType)
    : d(new AceTreeSerializerPrivate(this)) {
    d->init();
    d->id = id;
    d->preferredType = preferredType;
}

AceTreeSerializer::~AceTreeSerializer() {
    delete d;
}

QString AceTreeSerializer::id() const {
    return d->id;
}

QJsonValue::Type AceTreeSerializer::preferredType() const {
    return d->preferredType;
}

void AceTreeSerializer::setPreferredType(QJsonValue::Type type) {
    d->preferredType = type;
}

void *AceTreeSerializer::userdata() const {
    return d->userdata;
}

void AceTreeSerializer::setUserdata(void *userdata) {
    d->userdata = userdata;
}

AceTreeSerializer::ArrayOptions AceTreeSerializer::arrayOptions() const {
    return d->arrayOptions;
}

QString AceTreeSerializer::arrayTypeFiled() const {
    return d->arrayOptions.typeField;
}

bool AceTreeSerializer::isArrayUnordered() const {
    return d->arrayOptions.unordered;
}

void AceTreeSerializer::setArrayOptions(const AceTreeSerializer::ArrayOptions &opt) {
    d->arrayOptions = opt;
}

void AceTreeSerializer::setArrayUnordered(bool on) {
    d->arrayOptions.unordered = on;
}

void AceTreeSerializer::setArrayTypeField(const QString &key) {
    d->arrayOptions.typeField = key;
}

AceTreeSerializer::ObjectOptions AceTreeSerializer::objectOptions() const {
    return d->objectOptions;
}

void AceTreeSerializer::setObjectOptions(const AceTreeSerializer::ObjectOptions &opt) {
    d->objectOptions = opt;
}

void AceTreeSerializer::setObjectAcceptAll(AceTreeSerializer::AcceptValueType type) {
    d->objectOptions.acceptAll = type;
}

void AceTreeSerializer::setObjectAcceptOnes(const QHash<QString, AcceptValueTypeStrict> &hash) {
    d->objectOptions.acceptOnes = hash;
}

QString AceTreeSerializer::name() const {
    return d->selfKey;
}

bool AceTreeSerializer::addChild(const QString &key, AceTreeSerializer *child) {
    if (d->addChild(key, child)) {
        child->d->selfKey = key;
        return true;
    }
    return false;
}

bool AceTreeSerializer::removeChild(const QString &key) {
    auto child = d->childKeyIndexes.value(key);
    if (!child) {
        qWarning() << "AceTreeSerializer::removeChild(): key does not exist:" << key;
        return false;
    }
    return removeChildById(child->id());
}

bool AceTreeSerializer::removeChild(AceTreeSerializer *child) {
    if (!child) {
        qWarning() << "AceTreeSerializer::removeChild(): trying to remove null child";
        return false;
    }
    return removeChildById(child->id());
}

bool AceTreeSerializer::removeChildById(const QString &id) {
    AceTreeSerializer *child;
    if (d->removeChild(id, &child)) {
        child->d->selfKey.clear();
        return true;
    }
    return false;
}

AceTreeSerializer *AceTreeSerializer::child(const QString &id) const {
    return d->root->d->rootIndexes.value(id, nullptr);
}

QList<AceTreeSerializer *> AceTreeSerializer::children() const {
    QList<AceTreeSerializer *> res;
    for (const auto &page : qAsConst(d->childKeyIndexes)) {
        res.append(page);
        res.append(page->children());
    }
    return res;
}

AceTreeSerializer *AceTreeSerializer::parent() const {
    return d->parent;
}

AceTreeSerializer *AceTreeSerializer::root() const {
    return d->root;
}

QStringList AceTreeSerializer::keys() const {
    std::set<QString> res;
    for (const auto &key : d->childKeyIndexes.keys())
        res.insert(key);
    return {res.begin(), res.end()};
}

AceTreeSerializer *AceTreeSerializer::keyToChild(const QString &key) const {
    return d->childKeyIndexes.value(key, nullptr);
}

bool AceTreeSerializer::readValue(const QJsonValue &value, AceTreeItem *item) {
    if (d->selfKey.isEmpty() || !d->parent) {
        return false;
    }

    switch (d->parent->preferredType()) {
        case QJsonValue::Array:
        case QJsonValue::Object:
            break;
        default:
            return false;
            break;
    }

    AceTreeItem *childItem;
    switch (d->preferredType) {
        case QJsonValue::Null:
        case QJsonValue::Bool:
        case QJsonValue::Double:
        case QJsonValue::String: {
            item->setProperty(d->selfKey, value.toVariant());
            return true;
            break;
        }

        case QJsonValue::Array: {
            childItem = d->parent->createChildItem(this, item);
            if (!readArray(value.toArray(), childItem)) {
                goto failed;
            }
            break;
        }

        case QJsonValue::Object: {
            childItem = d->parent->createChildItem(this, item);
            if (!readObject(value.toObject(), childItem)) {
                goto failed;
            }
            break;
        }

        default:
            return false;
            break;
    }

    if (!d->parent->addChildItem(childItem, item)) {
        goto failed;
    }
    return true;

failed:
    delete childItem;
    return false;
}

bool AceTreeSerializer::readArray(const QJsonArray &array, AceTreeItem *item) {
    const auto &opt = d->arrayOptions;
    const auto &func = opt.reader;
    for (int i = 0; i < array.size(); ++i) {
        const auto &val = array.at(i);

        // Try default
        if (func) {
            switch (func(i, val, item, d->userdata)) {
                case AceTreeSerializer::Unhandled:
                    break;
                case AceTreeSerializer::Success:
                    continue;
                    break;
                default:
                    return false;
                    break;
            }
        }

        // Try children
        auto child = opt.typeField.isEmpty() ? (d->childKeyIndexes.isEmpty() ? nullptr : *d->childKeyIndexes.begin())
                                             : keyToChild(val[opt.typeField].toString());
        if (child && !child->readValue(val, item)) {
            return false;
        }

        // Leave it unhandled
    }

    return true;
}

bool AceTreeSerializer::readObject(const QJsonObject &object, AceTreeItem *item) {
    const auto &opt = d->objectOptions;
    const auto &func = opt.reader;

    QSet<QString> queriedKeys;
    QSet<AceTreeSerializer *> queriedChildren;

    for (auto it = object.begin(); it != object.end(); ++it) {
        const auto &key = it.key();
        const auto &val = it.value();

        // Try default
        if (func) {
            switch (func(key, val, item, d->userdata)) {
                case AceTreeSerializer::Unhandled:
                    break;
                case AceTreeSerializer::Success:
                    continue;
                    break;
                default:
                    return false;
                    break;
            }
        }

        // Try plain
        decltype(opt.acceptOnes)::const_iterator it1;
        if (opt.acceptAll != NoAccept || (it1 = opt.acceptOnes.find(key)) != opt.acceptOnes.end()) {
            switch (val.type()) {
                case QJsonValue::Null:
                case QJsonValue::Bool:
                case QJsonValue::Double:
                case QJsonValue::String:
                case QJsonValue::Array:
                case QJsonValue::Object: {
                    AcceptValueType type = opt.acceptAll;
                    if (type == NoAccept) {
                        if (!it1->validate(val.type())) {
                            return false;
                        }
                        type = it1->aType();
                    }
                    switch (type) {
                        case Property:
                            item->setProperty(key, val.toVariant());
                            break;
                        case DynamicData:
                            item->setDynamicData(key, val.toVariant());
                            break;
                        default:
                            break;
                    }
                    queriedKeys.insert(key);
                    continue;
                    break;
                }
                default:
                    break;
            }
        }

        // Try children
        auto child = keyToChild(key);
        if (child) {
            if (!child->readValue(val, item)) {
                return false;
            }
            queriedChildren.insert(child);
        }

        // Leave it unhandled
    }

    // Unused children
    for (const auto &child : qAsConst(d->childKeyIndexes)) {
        if (queriedChildren.contains(child))
            continue;
        child->createValue(item);
    }

    // Default properties
    for (auto it = opt.acceptOnes.begin(); it != opt.acceptOnes.end(); ++it) {
        if (queriedKeys.contains(it.key()))
            continue;

        if (it->aType() == Property)
            item->setProperty(it.key(), it->jValue());
        else if (it->aType() == DynamicData)
            item->setDynamicData(it.key(), it->jValue());
    }

    return true;
}

bool AceTreeSerializer::writeValue(const ValueRef &ref, const ChildOrProperty &cop) {
    if (!d->parent) {
        return false;
    }

    QJsonValue value;
    if (cop.is_item) {
        const auto &item = cop.i;
        switch (d->preferredType) {
            case QJsonValue::Null:
            case QJsonValue::Bool:
            case QJsonValue::Double:
            case QJsonValue::String: {
                value = variant_to_jsonValue(item->property(d->selfKey));
                break;
            }
            case QJsonValue::Array: {
                QJsonArray arr;
                if (!writeArray(&arr, item)) {
                    return false;
                }
                value = arr;
                break;
            }
            case QJsonValue::Object: {
                QJsonObject obj;
                if (!writeObject(&obj, item)) {
                    return false;
                }

                // Add type field
                auto &d1 = d->parent->d;
                if (d1->preferredType == QJsonValue::Array && !d1->arrayOptions.typeField.isEmpty()) {
                    obj.insert(d1->arrayOptions.typeField, d->selfKey);
                }

                value = obj;
                break;
            }
            default:
                return false;
                break;
        }
    } else if (cop.value_type != NoAccept) {
        value = variant_to_jsonValue(*cop.v);
    }

    if (ref.is_object) {
        ref.o->insert(d->selfKey, value);
    } else {
        ref.a->append(value);
    }
    return true;
}

bool AceTreeSerializer::writeArray(QJsonArray *array, const AceTreeItem *item) {
    const auto &opt = d->arrayOptions;
    const auto &func = opt.writer;

    if (isArrayUnordered()) {
        // Get array items from nodes
        const auto &nodes = item->nodes();
        for (int i = 0; i < nodes.size(); ++i) {
            const auto &childItem = nodes.at(i);

            // Try default
            if (func) {
                switch (func(i, array, childItem, d->userdata)) {
                    case AceTreeSerializer::Unhandled:
                        break;
                    case AceTreeSerializer::Success:
                        continue;
                        break;
                    default:
                        return false;
                        break;
                }
            }

            // Try children
            auto child = keyToChild(childItem->name());
            if (child && !child->writeValue(array, childItem)) {
                return false;
            }

            // Leave it unhandled
        }
    } else {
        // Get array items from rows
        for (int i = 0; i < item->rowCount(); ++i) {
            const auto &childItem = item->row(i);

            // Try default
            if (func) {
                switch (func(i, array, childItem, d->userdata)) {
                    case AceTreeSerializer::Unhandled:
                        break;
                    case AceTreeSerializer::Success:
                        continue;
                        break;
                    default:
                        return false;
                        break;
                }
            }

            // Try children
            auto child = keyToChild(childItem->name());
            if (child && !child->writeValue(array, childItem)) {
                return false;
            }

            // Leave it unhandled
        }
    }

    return true;
}

bool AceTreeSerializer::writeObject(QJsonObject *object, const AceTreeItem *item) {
    const auto &opt = d->objectOptions;
    const auto &func = opt.writer;

    QSet<QString> queriedKeys;
    QSet<AceTreeSerializer *> queriedChildren;

    // Write properties
    auto properties = item->propertyMap();
    for (auto it = properties.begin(); it != properties.end(); ++it) {
        const auto &key = it.key();
        const auto &val = it.value();

        // Try default
        if (func) {
            QJsonValue value;
            switch (func(key, object, {&val, Property}, d->userdata)) {
                case AceTreeSerializer::Unhandled:
                    break;
                case AceTreeSerializer::Success:
                    object->insert(key, value);
                    continue;
                    break;
                default:
                    return false;
                    break;
            }
        }

        // Try plain
        decltype(opt.acceptOnes)::const_iterator it1;
        if (opt.acceptAll == Property ||
            ((it1 = opt.acceptOnes.find(key)) != opt.acceptOnes.end() && it1.value() == Property)) {
            QJsonValue value = variant_to_jsonValue(val);
            object->insert(key, value);
            queriedKeys.insert(key);
            continue;
        }

        // Try children
        auto child = keyToChild(key);
        if (child) {
            if (!child->writeValue(object, {&val, Property})) {
                return false;
            }
            queriedChildren.insert(child);
        }

        // Leave it unhandled
    }

    // Write dynamic data
    auto dynamicData = item->dynamicDataMap();
    for (auto it = dynamicData.begin(); it != dynamicData.end(); ++it) {
        const auto &key = it.key();
        const auto &val = it.value();

        // Try default
        if (func) {
            QJsonValue value;
            switch (func(key, object, {&val, DynamicData}, d->userdata)) {
                case AceTreeSerializer::Unhandled:
                    break;
                case AceTreeSerializer::Success:
                    object->insert(key, value);
                    continue;
                    break;
                default:
                    return false;
                    break;
            }
        }

        // Try plain
        decltype(opt.acceptOnes)::const_iterator it1;
        if (opt.acceptAll == DynamicData ||
            ((it1 = opt.acceptOnes.find(key)) != opt.acceptOnes.end() && it1.value() == DynamicData)) {
            QJsonValue value = variant_to_jsonValue(val);
            object->insert(key, value);
            continue;
        }

        // Try children
        auto child = keyToChild(key);
        if (child) {
            if (!child->writeValue(object, {&val, DynamicData})) {
                return false;
            }
            queriedChildren.insert(child);
        }

        // Leave it unhandled
    }

    // Write nodes
    for (const auto &childItem : item->nodes()) {
        const auto &key = childItem->name();

        // Try default
        if (func) {
            switch (func(key, object, childItem, d->userdata)) {
                case AceTreeSerializer::Unhandled:
                    break;
                case AceTreeSerializer::Success:
                    continue;
                    break;
                default:
                    return false;
                    break;
            }
        }

        // Try children
        auto child = keyToChild(key);
        if (child) {
            if (!child->writeValue(object, childItem)) {
                return false;
            }
            queriedChildren.insert(child);
        }

        // Leave it unhandled
    }

    Q_UNUSED(queriedChildren)

    // Default properties
    for (auto it = opt.acceptOnes.begin(); it != opt.acceptOnes.end(); ++it) {
        if (queriedKeys.contains(it.key()))
            continue;

        object->insert(it.key(), it->jValue());
    }

    return true;
}

void AceTreeSerializer::createValue(AceTreeItem *item) {
    if (d->selfKey.isEmpty() || !d->parent) {
        return;
    }

    AceTreeItem *childItem;
    switch (d->preferredType) {
        case QJsonValue::Null:
        case QJsonValue::Bool:
        case QJsonValue::Double:
        case QJsonValue::String: {
            item->setProperty(d->selfKey, d->preferredType);
            return;
            break;
        }

        case QJsonValue::Array: {
            childItem = d->parent->createChildItem(this, item);
            createArray(childItem);
            break;
        }

        case QJsonValue::Object: {
            childItem = d->parent->createChildItem(this, item);
            createObject(childItem);
            break;
        }

        default:
            return;
            break;
    }

    if (!d->parent->addChildItem(childItem, item)) {
        delete childItem;
    }
}

void AceTreeSerializer::createArray(AceTreeItem *item) {
    const auto &opt = d->arrayOptions;
    const auto &func = opt.creator;
    if (func) {
        switch (func(item, d->userdata)) {
            case Unhandled:
                break;
            default:
                return;
                break;
        }
    }
}

void AceTreeSerializer::createObject(AceTreeItem *item) {
    const auto &opt = d->objectOptions;
    const auto &func = opt.creator;
    if (func) {
        switch (func(item, d->userdata)) {
            case Unhandled:
                break;
            default:
                return;
                break;
        }
    }

    // Default properties
    for (auto it = opt.acceptOnes.begin(); it != opt.acceptOnes.end(); ++it) {
        if (it->aType() == Property)
            item->setProperty(it.key(), it->jValue());
        else if (it->aType() == DynamicData)
            item->setDynamicData(it.key(), it->jValue());
    }

    // Children
    for (const auto &child : qAsConst(d->childKeyIndexes)) {
        child->createValue(item);
    }
}

void AceTreeSerializer::restoreValue(AceTreeItem *item) {
    if (!d->parent) {
        return;
    }

    switch (d->preferredType) {
        case QJsonValue::Array: {
            restoreArray(item);
            break;
        }
        case QJsonValue::Object: {
            restoreObject(item);
            break;
        }
        default:
            return;
            break;
    }

    item->setDynamicData("ATS_sid", id());
    item->setDynamicData("ATS_restored", true);
}

void AceTreeSerializer::restoreArray(AceTreeItem *item) {
    const auto &opt = d->arrayOptions;
    const auto &func = opt.restorer;
    if (func) {
        switch (func(item, d->userdata)) {
            case Unhandled:
                break;
            default:
                return;
                break;
        }
    }

    if (isArrayUnordered()) {
        // Get array items from nodes
        const auto &nodes = item->nodes();
        for (int i = 0; i < nodes.size(); ++i) {
            const auto &childItem = nodes.at(i);

            // Try children
            auto child = keyToChild(childItem->name());
            if (child) {
                restoreValue(childItem);
                continue;
            }

            // Leave it unhandled
        }

    } else {
        // Get array items from rows
        for (int i = 0; i < item->rowCount(); ++i) {
            const auto &childItem = item->row(i);

            // Try children
            auto child = keyToChild(childItem->name());
            if (child) {
                restoreValue(childItem);
                continue;
            }

            // Leave it unhandled
        }
    }
}

void AceTreeSerializer::restoreObject(AceTreeItem *item) {
    const auto &opt = d->objectOptions;
    const auto &func = opt.restorer;
    if (func) {
        switch (func(item, d->userdata)) {
            case Unhandled:
                break;
            default:
                return;
                break;
        }
    }

    // Write nodes
    for (const auto &childItem : item->nodes()) {
        const auto &key = childItem->name();

        // Try children
        auto child = keyToChild(key);
        if (child) {
            child->restoreValue(childItem);
        }

        // Leave it unhandled
    }
}

AceTreeItem *AceTreeSerializer::createChildItem(AceTreeSerializer *child, AceTreeItem *item) {
    Q_UNUSED(item)

    QString childKey = child->d->selfKey;

    auto childItem = new AceTreeItem(childKey);
    childItem->setDynamicData("ATS_sid", child->id());
    childItem->setDynamicData("ATS_restored", true);

    return childItem;
}

bool AceTreeSerializer::addChildItem(AceTreeItem *childItem, AceTreeItem *item) {
    switch (d->preferredType) {
        case QJsonValue::Array:
            if (!isArrayUnordered()) {
                item->appendRow(childItem);
                break;
            }
        case QJsonValue::Object:
            item->insertNode(childItem);
            break;
        default:
            return false;
            break;
    }
    return true;
}
