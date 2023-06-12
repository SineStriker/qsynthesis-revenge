#ifndef ACETREEENTITYVECTOR_H
#define ACETREEENTITYVECTOR_H

#include "AceTreeEntity.h"

class AceTreeEntityVectorPrivate;

class QMEDITING_EXPORT AceTreeEntityVector : public AceTreeEntity {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeEntityVector)
public:
    explicit AceTreeEntityVector(const QString &name, QObject *parent = nullptr);
    ~AceTreeEntityVector();

    bool setup(AceTreeItem *treeItem) override;

public:
    void move(int from, int to, int count);
    void remove(int index, int count);
    int size() const;

signals:
    void itemsInserted(int index, int count);
    void itemsMoved(int from, int to, int count);
    void itemsRemoved(int index, int count);

protected:
    void insertImpl(int index, const QVector<AceTreeEntity *> &items);
    AceTreeEntity *atImpl(int index) const;
    int indexOfImpl(AceTreeEntity *item) const;

protected:
    AceTreeEntityVector(AceTreeEntityVectorPrivate &d, const QString &name, QObject *parent = nullptr);
};

template <class T>
class AceTreeEntityVectorHelper {
    static_assert(std::is_base_of<AceTreeEntity, T>::value, "T should inherit from AceTreeEntity");

public:
    void insert(int index, const QVector<T *> &items);
    T *at(int index) const;
    int indexOf(T *item) const;
};

template <class T>
void AceTreeEntityVectorHelper<T>::insert(int index, const QVector<T *> &items) {
    QVector<AceTreeEntity *> tmp;
    tmp.reserve(items.size());
    for (const auto &item : items)
        tmp.append(tmp);
    return this->insertImpl(index, tmp);
}

template <class T>
T *AceTreeEntityVectorHelper<T>::at(int index) const {
    return qobject_cast<T *>(this->atImpl(index));
}

template <class T>
int AceTreeEntityVectorHelper<T>::indexOf(T *item) const {
    return this->indexOfImpl(item);
}

#endif // ACETREEENTITYVECTOR_H
