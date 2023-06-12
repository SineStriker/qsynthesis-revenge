#ifndef __CDOCKTABDRAGPROXY_H__
#define __CDOCKTABDRAGPROXY_H__

#include <QDebug>
#include <QLabel>

class CDockFrame;
class CDockTabBar;
class CDockCard;
class CDockSideBar;
class CDockDragLabel;

#include "QMWidgetToolsGlobal.h"

class QMWTOOLS_EXPORT CDockTabDragProxy : public QObject {
    Q_OBJECT
public:
    explicit CDockTabDragProxy(QObject *parent = nullptr);
    ~CDockTabDragProxy();

public:
    CDockFrame *frame() const;
    void setFrame(CDockFrame *frame);

    QList<CDockTabBar *> tabBars() const;
    QList<CDockSideBar *> doubleTabBars() const;

protected:
    CDockFrame *m_frame;
    CDockDragLabel *m_dragger;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void _q_tabDragStarted(CDockCard *card, const QPoint &pos, const QPixmap &pixmap);
    void _q_tabDragMove();
    void _q_tabDragOver();
};

#endif // __CDOCKTABDRAGPROXY_H__