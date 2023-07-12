#ifndef CSTACKEDSPLITTERPRIVATE_H
#define CSTACKEDSPLITTERPRIVATE_H

#include "CStackedSplitter.h"

class CStackedSplitterPrivate {
    Q_DECLARE_PUBLIC(CStackedSplitter)
public:
    CStackedSplitterPrivate();
    virtual ~CStackedSplitterPrivate();

    void init();

    CStackedSplitter *q_ptr;
};

#endif // CSTACKEDSPLITTERPRIVATE_H