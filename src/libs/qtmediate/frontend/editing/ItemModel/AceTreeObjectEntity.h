#ifndef CHORUSKIT_ACETREEOBJECTENTITY_H
#define CHORUSKIT_ACETREEOBJECTENTITY_H

#include "AceTreeEntity.h"

class AceTreeObjectEntityPrivate;

class QMEDITING_EXPORT AceTreeObjectEntity : public AceTreeEntity {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeObjectEntity)
public:
    enum Type {
        DynamicData,
        Property,
    };

    explicit AceTreeObjectEntity(Type type, QObject *parent = nullptr);
    ~AceTreeObjectEntity();

    Type type() const;

public:
    bool read(const QJsonValue &value) override;
    QJsonValue write() const override;

    QVariant value(const QString &key) const;
    void setValue(const QString &key, const QVariant &value);
    QStringList keys() const;
    QVariantHash valueMap() const;

protected:
    void doInitialize() override;
    void doSetup() override;

Q_SIGNALS:
    void changed(const QString &key, const QVariant &value);
};

#endif // CHORUSKIT_ACETREEOBJECTENTITY_H
