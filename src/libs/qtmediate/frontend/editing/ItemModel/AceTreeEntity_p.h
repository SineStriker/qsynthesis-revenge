#ifndef ACETREEENTITY_P_H
#define ACETREEENTITY_P_H

#include <QHash>
#include <QSet>

#include "AceTreeEntity.h"

class QMEDITING_EXPORT AceTreeEntityPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(AceTreeEntity)
public:
    AceTreeEntityPrivate();
    virtual ~AceTreeEntityPrivate();

    void init();

    AceTreeEntity *q_ptr;

    QString name;
    AceTreeItem *treeItem;
    AceTreeEntityBuilder *builder;

    QHash<QString, AceTreeEntity *> children;
};

#endif // ACETREEENTITY_P_H
