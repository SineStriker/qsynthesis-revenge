#ifndef CSCROLLBAR_H
#define CSCROLLBAR_H

#include <QAbstractScrollArea>
#include <QScrollBar>

#include "QMWidgetsGlobal.h"

class QMWIDGETS_API CScrollBar : public QScrollBar {
    Q_OBJECT
public:
    explicit CScrollBar(QWidget *parent = nullptr);
    explicit CScrollBar(Qt::Orientation orient, QWidget *parent = nullptr);
    ~CScrollBar();

public:
    static void replaceScrollBars(QAbstractScrollArea *area);

private:
    int pixelPosToRangeValue(int pos) const;

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif
};

#endif // CSCROLLBAR_H
