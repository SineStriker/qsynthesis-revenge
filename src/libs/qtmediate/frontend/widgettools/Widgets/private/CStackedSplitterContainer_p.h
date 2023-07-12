#ifndef CSTACKEDSPLITTERCONTAINERPRIVATE_H
#define CSTACKEDSPLITTERCONTAINERPRIVATE_H

#include "CStackedSplitterContainer.h"

class CStackedSplitterContainerPrivate {
    Q_DECLARE_PUBLIC(CStackedSplitterContainer)
public:
    CStackedSplitterContainerPrivate();
    virtual ~CStackedSplitterContainerPrivate();

    void init();

    CStackedSplitterContainer *q_ptr;
};

#endif // CSTACKEDSPLITTERCONTAINERPRIVATE_H