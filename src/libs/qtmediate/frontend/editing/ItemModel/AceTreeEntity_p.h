#ifndef ACETREEENTITYPRIVATE_H
#define ACETREEENTITYPRIVATE_H

#include "AceTreeEntity.h"

class QMEDITING_EXPORT AceTreeEntityPrivate {
    Q_DECLARE_PUBLIC(AceTreeEntity)
public:
    AceTreeEntityPrivate();
    virtual ~AceTreeEntityPrivate();

    void init();

    AceTreeEntity *q_ptr;

    AceTreeItem *m_treeItem;

    QSet<AceTreeEntity *> children;
    AceTreeEntity *parent;

    inline static AceTreeItem *getItem(AceTreeEntity *entity) {
        return entity->d_func()->m_treeItem;
    }

    inline static AceTreeEntityPrivate *get(AceTreeEntity *entity) {
        return entity->d_func();
    }
};

#endif // ACETREEENTITYPRIVATE_H