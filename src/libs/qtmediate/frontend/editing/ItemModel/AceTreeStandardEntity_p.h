#ifndef CHORUSKIT_ACETREESTANDARDENTITY_P_H
#define CHORUSKIT_ACETREESTANDARDENTITY_P_H

#include "AceTreeEntity_p.h"

#include <QJsonArray>
#include <QJsonObject>

#include <QMChronSet.h>
#include <QMPointerWrapper.h>

class QMEDITING_EXPORT AceTreeStandardEntityPrivate : public AceTreeEntityPrivate {
    Q_DECLARE_PUBLIC(AceTreeStandardEntity)
public:
    explicit AceTreeStandardEntityPrivate(AceTreeStandardEntity::Type type);
    ~AceTreeStandardEntityPrivate();

    AceTreeStandardEntity::Type type;
    QHash<QString, AceTreeEntity *> childNodeMap;
    bool m_external;

    QHash<QString, QMPointerWrapper<AceTreeEntity>> childPostAssignRefs;

    typedef QString (AceTreeStandardSchema::*GetTypeKey)() const;
    typedef AceTreeEntityBuilder (AceTreeStandardSchema::*GetDefaultBuilder)() const;
    typedef AceTreeEntityBuilder (AceTreeStandardSchema::*GetBuilder)(const QString &) const;

    bool testModifiable(const char *func) const;
    bool testInsertable(const char *func, const AceTreeEntity *entity) const;

    void rowsInserted(int index, const QVector<AceTreeItem *> &items) override;
    void rowsRemoved(int index, const QVector<AceTreeItem *> &items) override;
    void recordAdded(int seq, AceTreeItem *item) override;
    void recordRemoved(int seq, AceTreeItem *item) override;
    void nodeAdded(AceTreeItem *item) override;
    void nodeRemoved(AceTreeItem *item) override;

    bool readVector_helper(const AceTreeStandardSchema &schema, GetTypeKey getTypeKey,
                           GetDefaultBuilder getDefaultBuilder, GetBuilder getBuilder, const QJsonValue &value,
                           QVector<QPair<QString, AceTreeEntity *>> &childrenToAdd);
    bool readSet_helper(const AceTreeStandardSchema &schema, const QJsonValue &value);

    QJsonArray writeVector_helper(const AceTreeStandardSchema &schema, const QVector<AceTreeItem *> &childItems) const;
    QJsonObject writeSet_helper(const AceTreeStandardSchema &schema) const;

    void setupVector_helper(const AceTreeStandardSchema &schema, GetTypeKey getTypeKey,
                            GetDefaultBuilder getDefaultBuilder, GetBuilder getBuilder,
                            const QVector<AceTreeItem *> &items, QVector<AceTreeEntity *> &childrenToAdd);

    void addNode_assign(AceTreeEntity *child);
    void removeNode_assigns(AceTreeEntity *child);

public:
    inline static AceTreeStandardEntityPrivate *get(AceTreeStandardEntity *entity) {
        return entity->d_func();
    }

    inline static const AceTreeStandardEntityPrivate *get(const AceTreeStandardEntity *entity) {
        return entity->d_func();
    }
};

class AceTreeStandardSchemaData : public QSharedData {
public:
    QHash<QString, QJsonValue> dynamicData;
    QHash<QString, QJsonValue> properties;
    QString rowTypeKey;
    QHash<QString, AceTreeEntityBuilder> rowBuilders;
    QHash<const QMetaObject *, QMChronSet<QString>> rowBuilderIndexes;
    AceTreeEntityBuilder rowDefaultBuilder;

    QString recordTypeKey;
    QHash<QString, AceTreeEntityBuilder> recordBuilders;
    QHash<const QMetaObject *, QMChronSet<QString>> recordBuilderIndexes;
    AceTreeEntityBuilder recordDefaultBuilder;

    QHash<QString, AceTreeEntityBuilder> nodeBuilders;
    QHash<const QMetaObject *, QMChronSet<QString>> nodeBuilderIndexes;

    AceTreeStandardSchemaData() : rowDefaultBuilder(nullptr), recordDefaultBuilder(nullptr) {
    }
};

class QMEDITING_EXPORT AceTreeEntityVectorPrivate : public AceTreeStandardEntityPrivate {
    Q_DECLARE_PUBLIC(AceTreeEntityVector)
public:
    AceTreeEntityVectorPrivate();
    ~AceTreeEntityVectorPrivate();

    void init_deferred() override;

    struct Signals {
        QMetaMethod inserted;
        QMetaMethod aboutToMove;
        QMetaMethod moved;
        QMetaMethod aboutToRemove;
        QMetaMethod removed;
    };

    Signals *_signals;

    void rowsInserted(int index, const QVector<AceTreeItem *> &items) override;
    void rowsAboutToMove(int index, int count, int dest) override;
    void rowsMoved(int index, int count, int dest) override;
    void rowsAboutToRemove(int index, const QVector<AceTreeItem *> &items) override;
    void rowsRemoved(int index, const QVector<AceTreeItem *> &items) override;

public:
    inline static AceTreeEntityVectorPrivate *get(AceTreeEntityVector *entity) {
        return entity->d_func();
    }

    inline static const AceTreeEntityVectorPrivate *get(const AceTreeEntityVector *entity) {
        return entity->d_func();
    }
};

class QMEDITING_EXPORT AceTreeEntityRecordTablePrivate : public AceTreeStandardEntityPrivate {
    Q_DECLARE_PUBLIC(AceTreeEntityRecordTable)
public:
    AceTreeEntityRecordTablePrivate();
    ~AceTreeEntityRecordTablePrivate();

    void init_deferred() override;

    struct Signals {
        QMetaMethod inserted;
        QMetaMethod aboutToRemove;
        QMetaMethod removed;
    };

    Signals *_signals;

    void recordAdded(int seq, AceTreeItem *item) override;
    void recordAboutToRemove(int seq, AceTreeItem *item) override;
    void recordRemoved(int seq, AceTreeItem *item) override;

public:
    inline static AceTreeEntityRecordTablePrivate *get(AceTreeEntityRecordTable *entity) {
        return entity->d_func();
    }

    inline static const AceTreeEntityRecordTablePrivate *get(const AceTreeEntityRecordTable *entity) {
        return entity->d_func();
    }
};

class QMEDITING_EXPORT AceTreeEntityMappingPrivate : public AceTreeStandardEntityPrivate {
    Q_DECLARE_PUBLIC(AceTreeEntityMapping)
public:
    AceTreeEntityMappingPrivate();
    ~AceTreeEntityMappingPrivate();

    using Notifier = std::function<void(AceTreeEntityMapping *, const QVariant &, const QVariant &)>;

    QHash<QString, Notifier> dynamicPropertyNotifiers;
    QHash<QString, Notifier> propertyNotifiers;

    void dynamicDataChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue) override;
    void propertyChanged(const QString &key, const QVariant &newValue, const QVariant &oldValue) override;

public:
    inline static AceTreeEntityMappingPrivate *get(AceTreeEntityMapping *entity) {
        return entity->d_func();
    }

    inline static const AceTreeEntityMappingPrivate *get(const AceTreeEntityMapping *entity) {
        return entity->d_func();
    }
};

#define ACE_A        AceTreeEntityMapping *item, const QVariant &newValue, const QVariant &oldValue
#define ACE_Q(Class) auto *const q = reinterpret_cast<Class *>(item)

#endif // CHORUSKIT_ACETREESTANDARDENTITY_P_H
