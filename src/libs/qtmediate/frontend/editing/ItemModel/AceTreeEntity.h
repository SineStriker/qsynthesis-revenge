#ifndef ACETREEENTITY_H
#define ACETREEENTITY_H

#include <QObject>

#include "AceTreeModel.h"

class AceTreeEntityPrivate;
class AceTreeEntityBuilder;
class AceTreeEntityBuilderPrivate;

class QMEDITING_EXPORT AceTreeEntity : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeEntity)
public:
    explicit AceTreeEntity(const QString &name, QObject *parent = nullptr);
    ~AceTreeEntity();

    AceTreeEntityBuilder *builder() const;
    void setBuilder(AceTreeEntityBuilder *builder);

    virtual bool setup(AceTreeItem *treeItem);

public:
    QString entityName() const;
    AceTreeModel *treeModel() const;
    AceTreeItem *treeItem() const;

public:
    AceTreeEntity *child(const QString &name) const;
    AceTreeEntity *parentEntity() const;

protected:
    bool addChild(AceTreeEntity *c);
    bool removeChild(AceTreeEntity *c);
    bool removeChild(const QString &name);

protected:
    AceTreeEntity(AceTreeEntityPrivate &d, const QString &name, QObject *parent = nullptr);

    QScopedPointer<AceTreeEntityPrivate> d_ptr;

    friend class AceTreeEntityBuilder;

    friend class AceTreeEntityBuilderPrivate;
};

class QMEDITING_EXPORT AceTreeEntityBuilder : public QObject {
    Q_OBJECT
public:
    explicit AceTreeEntityBuilder(QObject *parent = nullptr);
    AceTreeEntityBuilder(const QString &rootBuilder, QObject *parent = nullptr);
    ~AceTreeEntityBuilder();

public:
    QString rootBuilder() const;
    void setRootBuilder(const QString &id);

    AceTreeEntity *build(const QString &id) const;

    template <class T>
    inline T *build(const QString &id) const;

    template <class T>
    inline T *buildRoot() const;

public:
    using Builder = std::function<AceTreeEntity *()>;

    bool addBuilder(const QString &id, const Builder &builder);
    bool addBuilder(const QString &id, const QMetaObject *metaObject);
    bool removeBuilder(const QString &id);
    bool hasBuilder(const QString &id) const;

public:
    bool addSubBuilder(const QString &parentId, const QString &childId);
    bool removeSubBuilder(const QString &parentId, const QString &childId);
    bool hasSubBuilder(const QString &parentId, const QString &childId) const;

private:
    AceTreeEntityBuilderPrivate *d;
};

template <class T>
T *AceTreeEntityBuilder::build(const QString &id) const {
    static_assert(std::is_base_of<AceTreeEntity, T>::value, "T should inherit from AceTreeEntity");
    return qobject_cast<T *>(build(id));
}

template <class T>
T *AceTreeEntityBuilder::buildRoot() const {
    return build<T>(rootBuilder());
}

#endif // ACETREEENTITY_H
