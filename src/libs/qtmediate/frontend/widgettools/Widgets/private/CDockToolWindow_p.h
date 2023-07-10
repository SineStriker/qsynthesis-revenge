#ifndef CDOCKTOOLWINDOWPRIVATE_H
#define CDOCKTOOLWINDOWPRIVATE_H

#include "CDockToolWindow.h"

class CDockToolWindowPrivate {
    Q_DECLARE_PUBLIC(CDockToolWindow)
public:
    CDockToolWindowPrivate();
    virtual ~CDockToolWindowPrivate();

    void init();

    CDockToolWindow *q_ptr;

    CDockCard *card;
};

#endif // CDOCKTOOLWINDOWPRIVATE_H