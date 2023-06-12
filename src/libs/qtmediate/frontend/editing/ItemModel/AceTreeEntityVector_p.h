#ifndef CHORUSKIT_ACETREEENTITYVECTOR_P_H
#define CHORUSKIT_ACETREEENTITYVECTOR_P_H

#include "AceTreeEntityVector.h"
#include "AceTreeEntity_p.h"

class AceTreeEntityVectorPrivate : public AceTreeEntityPrivate {
    Q_OBJECT
    Q_DECLARE_PUBLIC(AceTreeEntityVector)
public:
    AceTreeEntityVectorPrivate();
    ~AceTreeEntityVectorPrivate();

    QVector<AceTreeEntity *> vector;

    QVector<AceTreeEntity *> lastInsertOnes;
};


#endif // CHORUSKIT_ACETREEENTITYVECTOR_P_H
