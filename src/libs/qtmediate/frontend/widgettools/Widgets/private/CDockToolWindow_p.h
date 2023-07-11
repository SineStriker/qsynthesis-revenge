#ifndef CDOCKTOOLWINDOWPRIVATE_H
#define CDOCKTOOLWINDOWPRIVATE_H

#include <QSet>

#include "CDockToolWindow.h"

class CDockToolWindowPrivate {
    Q_DECLARE_PUBLIC(CDockToolWindow)
public:
    CDockToolWindowPrivate();
    virtual ~CDockToolWindowPrivate();

    void init();

    void setCard(CDockCard *card);

    CDockToolWindow *q_ptr;

    CDockCard *card;

    QMargins resizeMargins;
    QSet<QWidget *> hitTestWidgets;
};

#endif // CDOCKTOOLWINDOWPRIVATE_H