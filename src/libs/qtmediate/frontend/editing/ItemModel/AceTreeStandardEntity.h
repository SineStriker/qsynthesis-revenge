#ifndef ACETREESTANDARDENTITY_H
#define ACETREESTANDARDENTITY_H

#include <QMetaEnum>
#include <utility>

#include "AceTreeEntity.h"

class AceTreeStandardSchema;

class AceTreeStandardEntityPrivate;

class QMEDITING_EXPORT AceTreeStandardEntity : public AceTreeEntity {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeStandardEntity)
public:
    enum Type {
        Vector,
        RecordTable,
        Mapping,
    };

    explicit AceTreeStandardEntity(Type type, QObject *parent = nullptr);
    ~AceTreeStandardEntity();

    Type type() const;

    QString name() const override;

public:
    bool read(const QJsonValue &value) override;
    QJsonValue write() const override;

    virtual void sortRecords(QVector<AceTreeEntity *> &records) const;

    AceTreeStandardSchema schema() const;

    QString searchChildType(const QMetaObject *metaObject) const;

    AceTreeEntity *childTree(const QString &key) const;
    QStringList childTreeKeys() const;
    int childTreeCount() const;

protected:
    void doInitialize() override;
    void doSetup() override;

    AceTreeStandardEntity(AceTreeStandardEntityPrivate &d, QObject *parent = nullptr);

protected:
    QVariant dynamicData(const QString &key) const;
    void setDynamicData(const QString &key, const QVariant &value);
    QVariantHash dynamicDataMap() const;

    QVariant property(const QString &key) const;
    bool setProperty(const QString &key, const QVariant &value);
    QVariantHash properties() const;

    // Prevents confusion with QObject::setProperty/QObject::property
    inline QVariant attribute(const QString &key) const;
    inline bool setAttribute(const QString &key, const QVariant &value);

    bool insertRows(int index, const QVector<QPair<QString, AceTreeEntity *>> &entities);
    bool moveRows(int index, int count, int dest);
    bool removeRows(int index, int count);
    AceTreeEntity *row(int row) const;
    QVector<AceTreeEntity *> rows() const;
    int rowIndexOf(AceTreeEntity *entity) const;
    int rowCount() const;

    int addRecord(const QString &key, AceTreeEntity *entity);
    bool removeRecord(int seq);
    bool removeRecord(AceTreeEntity *entity);
    AceTreeEntity *record(int seq);
    int recordIndexOf(AceTreeEntity *entity) const;
    QList<int> records() const;
    int recordCount() const;
    int maxRecordSeq() const;

    bool containsNode(AceTreeEntity *entity);
    QList<AceTreeEntity *> nodes() const;
    int nodeCount() const;
    QStringList nodeKeys() const;
    AceTreeEntity *node(const QString &key) const;

private:
    bool addNode(const QString &key, AceTreeEntity *entity);
    bool removeNode(AceTreeEntity *entity);

    template <class T>
    friend class AceTreeEntityVectorHelper;

    template <class T>
    friend class AceTreeEntityRecordTableHelper;
};

inline QVariant AceTreeStandardEntity::attribute(const QString &key) const {
    return property(key);
}

inline bool AceTreeStandardEntity::setAttribute(const QString &key, const QVariant &value) {
    return setProperty(key, value);
}

class AceTreeStandardSchemaData;

class AceTreeEntityBuilder {
public:
    using Builder = std::function<AceTreeEntity *()>;

    AceTreeEntityBuilder() : m_metaObject(nullptr){};
    AceTreeEntityBuilder(const QMetaObject *metaObject) : m_metaObject(metaObject){};
    AceTreeEntityBuilder(const QMetaObject *metaObject, Builder builder)
        : m_metaObject(metaObject), m_builder(std::move(builder)){};

    inline const QMetaObject *metaObject() const {
        return m_metaObject;
    }

    inline bool isValid() const {
        return m_metaObject != nullptr;
    }

    inline AceTreeEntity *operator()() const {
        return m_builder ? m_builder() : qobject_cast<AceTreeEntity *>(m_metaObject->newInstance());
    }

private:
    Builder m_builder;
    const QMetaObject *m_metaObject;
};

class QMEDITING_EXPORT AceTreeStandardSchema {
public:
    AceTreeStandardSchema();
    ~AceTreeStandardSchema();

    AceTreeStandardSchema(const AceTreeStandardSchema &other);
    AceTreeStandardSchema(AceTreeStandardSchema &&other) noexcept;

    AceTreeStandardSchema &operator=(const AceTreeStandardSchema &other);
    AceTreeStandardSchema &operator=(AceTreeStandardSchema &&other) noexcept;

public:
    // DynamicData
    QJsonValue dynamicDataSpec(const QString &key) const;
    void setDynamicDataSpec(const QString &key, const QJsonValue &defaultValue);
    QStringList dynamicDataSpecKeys() const;
    QHash<QString, QJsonValue> dynamicDataSpecHash() const;

    // Properties
    QJsonValue propertySpec(const QString &key) const;
    void setPropertySpec(const QString &key, const QJsonValue &defaultValue);
    QStringList propertySpecKeys() const;
    QHash<QString, QJsonValue> propertySpecHash() const;

    inline QJsonValue attributeSpec(const QString &key) const;
    inline void setAttributeSpec(const QString &key, const QJsonValue &defaultValue);
    inline QStringList attributeSpecKeys() const;
    inline QHash<QString, QJsonValue> attributeSpecHash() const;

    // Row
    QString rowTypeKey() const;
    void setRowTypeKey(const QString &key);

    AceTreeEntityBuilder rowBuilder(const QString &type) const;
    void setRowBuilder(const QString &type, const AceTreeEntityBuilder &builder);
    QStringList rowTypes() const;
    QString rowType(const QMetaObject *metaObject) const;
    QStringList rowTypes(const QMetaObject *metaObject) const;
    int rowTypeCount() const;

    AceTreeEntityBuilder rowDefaultBuilder() const;
    void setRowDefaultBuilder(const AceTreeEntityBuilder &builder);

    // Record
    QString recordTypeKey() const;
    void setRecordTypeKey(const QString &key);

    AceTreeEntityBuilder recordBuilder(const QString &type) const;
    void setRecordBuilder(const QString &type, const AceTreeEntityBuilder &builder);
    QStringList recordTypes() const;
    QString recordType(const QMetaObject *metaObject) const;
    QStringList recordTypes(const QMetaObject *metaObject) const;
    int recordTypeCount() const;

    AceTreeEntityBuilder recordDefaultBuilder() const;
    void setRecordDefaultBuilder(const AceTreeEntityBuilder &builder);

    // Node
    AceTreeEntityBuilder nodeBuilder(const QString &key) const;
    void setNodeBuilder(const QString &key, const AceTreeEntityBuilder &builder);
    QStringList nodeKeys() const;
    QString nodeKey(const QMetaObject *metaObject) const;
    QStringList nodeKeys(const QMetaObject *metaObject) const;
    int nodeBuilderCount() const;

public:
    static AceTreeStandardSchema &globalSchemaRef(const QMetaObject *metaObject);
    static AceTreeStandardSchema globalSchema(const QMetaObject *metaObject, bool *ok = nullptr);
    static void setGlobalSchema(const QMetaObject *metaObject, const AceTreeStandardSchema &schema);
    static void clearGlobalSchemaCache(const QMetaObject *metaObject = nullptr);

private:
    QSharedDataPointer<AceTreeStandardSchemaData> d;

    friend class AceTreeStandardEntity;
    friend class AceTreeStandardEntityPrivate;
};

inline QJsonValue AceTreeStandardSchema::attributeSpec(const QString &key) const {
    return propertySpec(key);
}

inline void AceTreeStandardSchema::setAttributeSpec(const QString &key, const QJsonValue &defaultValue) {
    setPropertySpec(key, defaultValue);
}

inline QStringList AceTreeStandardSchema::attributeSpecKeys() const {
    return propertySpecKeys();
}

inline QHash<QString, QJsonValue> AceTreeStandardSchema::attributeSpecHash() const {
    return propertySpecHash();
}

class AceTreeEntityVectorPrivate;

class QMEDITING_EXPORT AceTreeEntityVector : public AceTreeStandardEntity {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeEntityVector)
public:
    explicit AceTreeEntityVector(QObject *parent = nullptr);
    ~AceTreeEntityVector();

protected:
    AceTreeEntityVector(AceTreeEntityVectorPrivate &d, QObject *parent = nullptr);
};

class AceTreeEntityRecordTablePrivate;

class QMEDITING_EXPORT AceTreeEntityRecordTable : public AceTreeStandardEntity {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeEntityRecordTable)
public:
    explicit AceTreeEntityRecordTable(QObject *parent = nullptr);
    ~AceTreeEntityRecordTable();

protected:
    AceTreeEntityRecordTable(AceTreeEntityRecordTablePrivate &d, QObject *parent = nullptr);
};

class AceTreeEntityMappingPrivate;

class QMEDITING_EXPORT AceTreeEntityMapping : public AceTreeStandardEntity {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeEntityMapping)
public:
    explicit AceTreeEntityMapping(QObject *parent = nullptr);
    ~AceTreeEntityMapping();

protected:
    AceTreeEntityMapping(AceTreeEntityMappingPrivate &d, QObject *parent = nullptr);
};

template <class T>
class AceTreeEntityVectorHelper {
    static_assert(std::is_base_of<AceTreeEntity, T>::value, "T should inherit from AceTreeEntity");

public:
    bool prepend(T *item);
    bool prepend(const QVector<T *> &items);
    bool append(T *item);
    bool append(const QVector<T *> &items);
    bool insert(int index, T *item);
    bool remove(int index);
    bool insert(int index, const QVector<T *> &items);
    bool move(int index, int count, int dest);
    bool remove(int index, int count);
    T *at(int index) const;
    QVector<T *> values() const;
    int indexOf(T *item) const;
    int size() const;
    int count() const;

private:
    inline AceTreeStandardEntity *to_entity();
    inline const AceTreeStandardEntity *to_entity() const;
};

#define ACE_TREE_DECLARE_VECTOR_SIGNALS(T)                                                                             \
    void inserted(int index, const QVector<T *> &items);                                                               \
    void aboutToMove(int index, int count, int dest);                                                                  \
    void moved(int index, int count, int dest);                                                                        \
    void aboutToRemove(int index, const QVector<T *> &items);                                                          \
    void removed(int index, int count);

template <class T>
class AceTreeEntityRecordTableHelper {
    static_assert(std::is_base_of<AceTreeEntity, T>::value, "T should inherit from AceTreeEntity");

public:
    int insert(T *item);
    bool remove(int index);
    bool remove(T *item);
    T *at(int index);
    int indexOf(T *item) const;
    QList<int> indexes() const;
    int size() const;
    int count() const;
    int maxIndex() const;

private:
    inline AceTreeStandardEntity *to_entity();
    inline const AceTreeStandardEntity *to_entity() const;
};

#define ACE_TREE_DECLARE_RECORD_TABLE_SIGNALS(T)                                                                       \
    void inserted(int seq, T *item);                                                                                   \
    void aboutToRemove(int seq, T *item);                                                                              \
    void removed(int seq);

template <class T>
bool AceTreeEntityVectorHelper<T>::prepend(T *item) {
    return insert(0, QVector<T *>{item});
}

template <class T>
bool AceTreeEntityVectorHelper<T>::prepend(const QVector<T *> &items) {
    return insert(0, items);
}

template <class T>
bool AceTreeEntityVectorHelper<T>::append(T *item) {
    return insert(size(), QVector<T *>{item});
}

template <class T>
bool AceTreeEntityVectorHelper<T>::append(const QVector<T *> &items) {
    return insert(size(), items);
}

template <class T>
bool AceTreeEntityVectorHelper<T>::insert(int index, T *item) {
    return insert(index, QVector<T *>{item});
}

template <class T>
bool AceTreeEntityVectorHelper<T>::remove(int index) {
    return remove(index, 1);
}

template <class T>
bool AceTreeEntityVectorHelper<T>::insert(int index, const QVector<T *> &items) {
    QVector<QPair<QString, AceTreeEntity *>> tmp;
    tmp.resize(items.size());
    AceTreeStandardEntity *entity = to_entity();
    for (const auto &item : items) {
        tmp.append({item ? entity->searchChildType(item->metaObject()) : QString(), item});
    }
    return entity->insertRows(index, tmp);
}

template <class T>
bool AceTreeEntityVectorHelper<T>::move(int index, int count, int dest) {
    return to_entity()->moveRows(index, count, dest);
}

template <class T>
bool AceTreeEntityVectorHelper<T>::remove(int index, int count) {
    return to_entity()->removeRows(index, count);
}

template <class T>
T *AceTreeEntityVectorHelper<T>::at(int index) const {
    return qobject_cast<T *>(to_entity()->row(index));
}

template <class T>
QVector<T *> AceTreeEntityVectorHelper<T>::values() const {
    QVector<T *> tmp;
    const auto &children = to_entity()->rows();
    tmp.reserve(children.size());
    for (const auto &child : children) {
        tmp.append(qobject_cast<T *>(child));
    }
    return tmp;
}

template <class T>
int AceTreeEntityVectorHelper<T>::indexOf(T *item) const {
    return to_entity()->rowIndexOf(item);
}

template <class T>
int AceTreeEntityVectorHelper<T>::size() const {
    return to_entity()->rowCount();
}

template <class T>
int AceTreeEntityVectorHelper<T>::count() const {
    return size();
}

template <class T>
inline AceTreeStandardEntity *AceTreeEntityVectorHelper<T>::to_entity() {
    return reinterpret_cast<AceTreeStandardEntity *>(this);
}

template <class T>
inline const AceTreeStandardEntity *AceTreeEntityVectorHelper<T>::to_entity() const {
    return reinterpret_cast<const AceTreeStandardEntity *>(this);
}


template <class T>
int AceTreeEntityRecordTableHelper<T>::insert(T *item) {
    AceTreeStandardEntity *entity = to_entity();
    return to_entity()->addRecord(item ? entity->searchChildType(item->metaObject()) : QString(), item);
}

template <class T>
bool AceTreeEntityRecordTableHelper<T>::remove(int index) {
    return to_entity()->removeRecord(index);
}

template <class T>
bool AceTreeEntityRecordTableHelper<T>::remove(T *item) {
    return to_entity()->removeRecord(item);
}

template <class T>
T *AceTreeEntityRecordTableHelper<T>::at(int index) {
    return qobject_cast<T *>(to_entity()->record(index));
}

template <class T>
int AceTreeEntityRecordTableHelper<T>::indexOf(T *item) const {
    return to_entity()->recordIndexOf(item);
}

template <class T>
QList<int> AceTreeEntityRecordTableHelper<T>::indexes() const {
    return to_entity()->records();
}

template <class T>
int AceTreeEntityRecordTableHelper<T>::size() const {
    return to_entity()->recordCount();
}

template <class T>
int AceTreeEntityRecordTableHelper<T>::count() const {
    return size();
}

template <class T>
int AceTreeEntityRecordTableHelper<T>::maxIndex() const {
    return to_entity()->maxRecordSeq();
}

template <class T>
inline AceTreeStandardEntity *AceTreeEntityRecordTableHelper<T>::to_entity() {
    return reinterpret_cast<AceTreeStandardEntity *>(this);
}

template <class T>
inline const AceTreeStandardEntity *AceTreeEntityRecordTableHelper<T>::to_entity() const {
    return reinterpret_cast<const AceTreeStandardEntity *>(this);
}

#endif // ACETREESTANDARDENTITY_H