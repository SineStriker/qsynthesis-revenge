#include "AceTreeObjectEntity.h"
#include "AceTreeEntity_p.h"

#include <QJsonObject>

class AceTreeObjectEntityPrivate : public AceTreeEntityPrivate {
    Q_DECLARE_PUBLIC(AceTreeObjectEntity)
public:
    explicit AceTreeObjectEntityPrivate(AceTreeObjectEntity::Type type) : type(type) {
    }

    AceTreeObjectEntity::Type type;

    void dynamicDataChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue) override {
        Q_Q(AceTreeObjectEntity);
        emit q->changed(key, newValue);
    }

    void propertyChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue) override {
        Q_Q(AceTreeObjectEntity);
        emit q->changed(key, newValue);
    }
};

AceTreeObjectEntity::AceTreeObjectEntity(AceTreeObjectEntity::Type type, QObject *parent)
    : AceTreeEntity(*new AceTreeObjectEntityPrivate(type), parent) {
}

AceTreeObjectEntity::~AceTreeObjectEntity() {
}

AceTreeObjectEntity::Type AceTreeObjectEntity::type() const {
    Q_D(const AceTreeObjectEntity);
    return d->type;
}

bool AceTreeObjectEntity::read(const QJsonValue &value) {
    Q_D(AceTreeObjectEntity);

    if (!value.isObject()) {
        return false;
    }

    auto treeItem = d->m_treeItem;

    auto obj = value.toObject();
    switch (d->type) {
        case DynamicData: {
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                treeItem->setDynamicData(it.key(), it->toVariant());
            }
            break;
        }

        case Property: {
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                treeItem->setAttribute(it.key(), it->toVariant());
            }
            break;
        }

        default:
            break;
    }

    return true;
}

QJsonValue AceTreeObjectEntity::write() const {
    Q_D(const AceTreeObjectEntity);

    QJsonObject obj;

    switch (d->type) {
        case DynamicData: {
            auto hash = d->m_treeItem->dynamicDataMap();
            for (auto it = hash.begin(); it != hash.end(); ++it) {
                obj.insert(it.key(), QJsonValue::fromVariant(it.value()));
            }
            break;
        }

        case Property: {
            auto hash = d->m_treeItem->propertyMap();
            for (auto it = hash.begin(); it != hash.end(); ++it) {
                obj.insert(it.key(), QJsonValue::fromVariant(it.value()));
            }
            break;
        }

        default:
            break;
    }

    return obj;
}

QVariant AceTreeObjectEntity::value(const QString &key) const {
    Q_D(const AceTreeObjectEntity);
    QVariant res;
    switch (d->type) {
        case DynamicData:
            res = d->m_treeItem->dynamicData(key);
            break;

        case Property:
            res = d->m_treeItem->attribute(key);
            break;

        default:
            break;
    }
    return res;
}

void AceTreeObjectEntity::setValue(const QString &key, const QVariant &value) {
    Q_D(AceTreeObjectEntity);
    switch (d->type) {
        case DynamicData:
            d->m_treeItem->setDynamicData(key, value);
            break;

        case Property:
            d->m_treeItem->setAttribute(key, value);
            break;

        default:
            break;
    }
}

QStringList AceTreeObjectEntity::keys() const {
    Q_D(const AceTreeObjectEntity);
    QStringList res;
    switch (d->type) {
        case DynamicData:
            res = d->m_treeItem->dynamicDataKeys();
            break;

        case Property:
            res = d->m_treeItem->propertyKeys();
            break;

        default:
            break;
    }
    return res;
}

QVariantHash AceTreeObjectEntity::valueMap() const {
    Q_D(const AceTreeObjectEntity);
    QVariantHash res;
    switch (d->type) {
        case DynamicData:
            res = d->m_treeItem->dynamicDataMap();
            break;

        case Property:
            res = d->m_treeItem->propertyMap();
            break;

        default:
            break;
    }
    return res;
}

void AceTreeObjectEntity::doInitialize() {
}

void AceTreeObjectEntity::doSetup() {
}