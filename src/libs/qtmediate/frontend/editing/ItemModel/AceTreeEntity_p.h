#ifndef ACETREEENTITYPRIVATE_H
#define ACETREEENTITYPRIVATE_H

#include "AceTreeEntity.h"

class AceTreeEntityPrivate {
    Q_DECLARE_PUBLIC(AceTreeEntity)
public:
    AceTreeEntityPrivate();
    virtual ~AceTreeEntityPrivate();

    void init();

    AceTreeEntity *q_ptr;
};

class AceTreeEntityFactoryPrivate {
public:
    AceTreeEntityFactoryPrivate(AceTreeEntityFactory *q) : q(q){};

    AceTreeEntityFactory *q;
};

#endif // ACETREEENTITYPRIVATE_H