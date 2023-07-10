#ifndef CDOCKTOOLWINDOW_H
#define CDOCKTOOLWINDOW_H

#include <QFrame>
#include <QMenu>

#include "QMWidgetToolsGlobal.h"

class CDockCard;

class CDockToolWindowPrivate;

class QMWTOOLS_EXPORT CDockToolWindow : public QFrame {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CDockToolWindow)
public:
    explicit CDockToolWindow(QWidget *parent = nullptr);
    ~CDockToolWindow();

public:
    CDockCard *card() const;
    void setCard(CDockCard *card);

    virtual QMenu *cardMenu() const;

protected:
    CDockToolWindow(CDockToolWindowPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<CDockToolWindowPrivate> d_ptr;
};

#endif // CDOCKTOOLWINDOW_H