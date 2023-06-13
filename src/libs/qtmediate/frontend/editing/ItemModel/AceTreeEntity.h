#ifndef ACETREEENTITY_H
#define ACETREEENTITY_H

#include <QObject>

#include "AceTreeModel.h"

class AceTreeEntityPrivate;

class QMEDITING_EXPORT AceTreeEntity : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeEntity)
public:
    explicit AceTreeEntity(QObject *parent = nullptr);
    ~AceTreeEntity();

    virtual void setup(AceTreeItem *item);

protected:
    AceTreeEntity(AceTreeEntityPrivate &d, QObject *parent = nullptr);

    QScopedPointer<AceTreeEntityPrivate> d_ptr;
};

class AceTreeEntityFactoryPrivate;

class QMEDITING_EXPORT AceTreeEntityFactory : public QObject {
    Q_OBJECT
public:
    explicit AceTreeEntityFactory(QObject *parent = nullptr);
    ~AceTreeEntityFactory();

    template <class T>
    T *create(AceTreeItem *item = nullptr) const {
        static_assert(std::is_base_of<AceTreeEntity, T>::value, "T should inherit from AceTreeEntity");
        return qobject_cast<T *>(createImpl(&T::staticMetaObject, item));
    }

public:
    bool addChild(const QMetaObject *parent, const QString &key, const QMetaObject *child);
    bool removeChild(const QMetaObject *parent, const QString &key);
    const QMetaObject *child(const QMetaObject *parent, const QString &key);

private:
    AceTreeEntity *createImpl(const QMetaObject *metaObject, AceTreeItem *treeItem);

    AceTreeEntityFactoryPrivate *d;
};

#endif // ACETREEENTITY_H