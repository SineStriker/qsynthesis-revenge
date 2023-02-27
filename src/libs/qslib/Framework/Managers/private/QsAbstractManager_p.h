#ifndef QSABSTRACTMANAGER_P_H
#define QSABSTRACTMANAGER_P_H

#include "../QsAbstractManager.h"

class QSFRAMEWORK_API QsAbstractManagerPrivate {
    Q_DECLARE_PUBLIC(QsAbstractManager)
public:
    QsAbstractManagerPrivate();
    virtual ~QsAbstractManagerPrivate();

    void init();

    QsAbstractManager *q_ptr;
};

#endif // QSABSTRACTMANAGER_P_H
