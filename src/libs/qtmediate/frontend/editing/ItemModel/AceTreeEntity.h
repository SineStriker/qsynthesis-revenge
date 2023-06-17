#ifndef ACETREEENTITY_H
#define ACETREEENTITY_H

#include <QJsonValue>
#include <QObject>
#include <QSharedData>

#include "AceTreeModel.h"

class AceTreeEntityPrivate;

class QMEDITING_EXPORT AceTreeEntity : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeEntity)
public:
    explicit AceTreeEntity(QObject *parent = nullptr);
    ~AceTreeEntity();

    virtual QString name() const = 0;

public:
    void initialize();
    void setup(AceTreeItem *item);

    virtual bool read(const QJsonValue &value) = 0;
    virtual QJsonValue write() const = 0;

    QList<AceTreeEntity *> childEntities() const;
    bool hasChildEntity(AceTreeEntity *child) const;
    AceTreeEntity *parentEntity() const;

    inline bool isFree() const;
    inline bool isWritable() const;
    const AceTreeItem *treeItem() const;
    const AceTreeModel *treeModel() const;

    bool addChild(AceTreeEntity *child);
    bool removeChild(AceTreeEntity *child);

    static AceTreeEntity *itemToEntity(AceTreeItem *item);

protected:
    virtual void doInitialize();
    virtual void doSetup();

    virtual void childAdded(AceTreeEntity *child);
    virtual void childAboutToRemove(AceTreeEntity *child);

    AceTreeEntity(AceTreeEntityPrivate &d, QObject *parent = nullptr);
    QScopedPointer<AceTreeEntityPrivate> d_ptr;
};

inline bool AceTreeEntity::isFree() const {
    return !parentEntity() && treeItem()->isFree();
}

inline bool AceTreeEntity::isWritable() const {
    return treeItem()->isWritable();
}

#endif // ACETREEENTITY_H