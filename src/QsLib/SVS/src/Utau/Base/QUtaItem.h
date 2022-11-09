#ifndef QUTAITEM_H
#define QUTAITEM_H

#include <QStringList>

#include "qssvs_global.h"

class QSSVS_API QUtaItem {
public:
    QUtaItem();
    virtual ~QUtaItem();

    virtual void reset();
    virtual bool isEmpty() const;

    void clear();
};

#endif // QUTAITEM_H
