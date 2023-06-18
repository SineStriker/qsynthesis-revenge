#ifndef ACETREEENTITYPRIVATE_H
#define ACETREEENTITYPRIVATE_H

#include "AceTreeEntity.h"

class QMEDITING_EXPORT AceTreeEntityPrivate : public AceTreeItemSubscriber {
    Q_DECLARE_PUBLIC(AceTreeEntity)
public:
    AceTreeEntityPrivate();
    virtual ~AceTreeEntityPrivate();

    void init();

    AceTreeEntity *q_ptr;

    QString name;
    AceTreeItem *m_treeItem;

    QSet<AceTreeEntity *> children;
    AceTreeEntity *parent;

    virtual void init_deferred();

    inline static AceTreeItem *getItem(AceTreeEntity *entity) {
        return entity->d_func()->m_treeItem;
    }

    inline static const AceTreeItem *getItem(const AceTreeEntity *entity) {
        return entity->d_func()->m_treeItem;
    }

    inline static AceTreeEntityPrivate *get(AceTreeEntity *entity) {
        return entity->d_func();
    }

    inline static const AceTreeEntityPrivate *get(const AceTreeEntity *entity) {
        return entity->d_func();
    }

    static void setName(AceTreeEntity *entity, const QString &name) {
        entity->d_func()->name = name;
    }
};

#endif // ACETREEENTITYPRIVATE_H