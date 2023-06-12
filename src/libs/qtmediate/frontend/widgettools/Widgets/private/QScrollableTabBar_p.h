#ifndef QSCROLLABLETABBARPRIVATE_H
#define QSCROLLABLETABBARPRIVATE_H

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QSlider>
#include <QTabBar>
#include <QtGlobal>

#include "QScrollableTabBarTab.h"

class QScrollableTabWidget;
class QScrollableTabBar;

class QScrollableTabBarPrivate {
    Q_DECLARE_PUBLIC(QScrollableTabBar)
public:
    QScrollableTabBarPrivate();
    virtual ~QScrollableTabBarPrivate();

    void init();

    void layoutScroll();
    void updateScroll();

    void runOpacityTween(bool visible);

    void startDrag(QScrollableTabBarTab *tab);

    QScrollableTabBarTab *tabAtIndex(int index) const;
    void setCurrentTab(QScrollableTabBarTab *tab);

    void autoScrollToCurrent() const;

    void updateVisibility();

    QScrollableTabBar *q_ptr;

    // Properties
    bool autoHide;

    QScrollableTabWidget *tabs;

    // Tabs Entity
    QHBoxLayout *entityLayout;
    QWidget *entity;

    // Scroll Bar
    QScrollBar *scrollBar;

    QPropertyAnimation *opacityTween;
    QGraphicsOpacityEffect *opacityEffect;

    int timerId;
    bool underMouse;
    QTime lastResized;

    // Tab Bar Properties
    QTabBar::SelectionBehavior selectionBehaviorOnRemove;

    // Realtime Status
    QScrollableTabBarTab *current, *previous;

    // Drag
    int draggedIndex;
    bool needAutoScroll;

    static QScrollableTabBar *draggedTabBar;
};

#endif // QSCROLLABLETABBARPRIVATE_H
