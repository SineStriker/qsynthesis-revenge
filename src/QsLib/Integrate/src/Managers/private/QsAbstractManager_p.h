#ifndef QSBASTRACTMANAGERPRIVATE_H
#define QSBASTRACTMANAGERPRIVATE_H

#include "../QsAbstractManager.h"

#include "qsintegrate_global.h"

class QSINTEGRATE_API QsAbstractManagerPrivate {
    Q_DECLARE_PUBLIC(QsAbstractManager)
public:
    QsAbstractManagerPrivate();
    virtual ~QsAbstractManagerPrivate();

    void init();

    QsAbstractManager *q_ptr;
};

#endif // QSBASTRACTMANAGERPRIVATE_H
