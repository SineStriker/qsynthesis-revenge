#ifndef CHORUSKIT_ACETREESTANDARDENTITY_P_H
#define CHORUSKIT_ACETREESTANDARDENTITY_P_H

#include "AceTreeEntity_p.h"

#include <QJsonArray>
#include <QJsonObject>

#include "QMChronSet.h"

class QMEDITING_EXPORT AceTreeStandardEntityPrivate : public AceTreeEntityPrivate, public AceTreeItemSubscriber {
    Q_DECLARE_PUBLIC(AceTreeStandardEntity)
public:
    explicit AceTreeStandardEntityPrivate(AceTreeStandardEntity::Type type);
    ~AceTreeStandardEntityPrivate();

    AceTreeStandardEntity::Type type;
    QHash<QString, AceTreeEntity *> childNodeMap;
    bool m_external;

    mutable AceTreeStandardSchema cachedSpec;
    mutable const QMetaObject *cachedMetaObject;

    struct EntityPointerWrapper {
        EntityPointerWrapper() : ref(nullptr){};

        template <class T>
        EntityPointerWrapper(T **ref) : ref(reinterpret_cast<AceTreeEntity **>(ref)) {
            static_assert(std::is_base_of<AceTreeEntity, T>::value, "T should inherit from AceTreeEntity");
        }

        AceTreeEntity **ref;
    };
    QHash<QString, EntityPointerWrapper> childPostAssignRefs;

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

#endif // CHORUSKIT_ACETREESTANDARDENTITY_P_H
