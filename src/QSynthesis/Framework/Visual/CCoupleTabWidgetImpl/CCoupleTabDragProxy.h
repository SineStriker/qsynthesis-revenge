#ifndef CCOUPLETABDRAGPROXY_H
#define CCOUPLETABDRAGPROXY_H

#include "CCoupleTabTypes.h"

#include <QDebug>
#include <QLabel>

class CCoupleTabFrame;
class CCoupleTabBar;
class CCoupleTabBarCard;
class CCoupleTabDoubleBar;
class CCoupleTabDragLabel;

class CCoupleTabDragProxy : public QObject {
    Q_OBJECT
public:
    explicit CCoupleTabDragProxy(QObject *parent = nullptr);
    ~CCoupleTabDragProxy();

public:
    CCoupleTabFrame *frame() const;
    void setFrame(CCoupleTabFrame *frame);

    QList<CCoupleTabBar *> tabBars() const;
    QList<CCoupleTabDoubleBar *> doubleTabBars() const;

protected:
    CCoupleTabFrame *m_frame;
    CCoupleTabDragLabel *m_dragger;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void handleTabDragStarted(CCoupleTabBarCard *card, const QPoint &pos, const QPixmap &pixmap);
    void handleTabDragMove();
    void handleTabDragOver();
};

#endif // CCOUPLETABDRAGPROXY_H
