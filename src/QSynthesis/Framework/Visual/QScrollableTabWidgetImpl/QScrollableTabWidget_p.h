#ifndef QSCROLLABLETABWIDGETPRIVATE_H
#define QSCROLLABLETABWIDGETPRIVATE_H

#include <QtGlobal>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "QScrollableTabBar.h"

class QScrollableTabWidget;

class QScrollableTabWidgetPrivate {
    Q_DECLARE_PUBLIC(QScrollableTabWidget)
public:
    QScrollableTabWidgetPrivate();
    ~QScrollableTabWidgetPrivate();

    void init();

    QScrollableTabWidget *q_ptr;

    QHBoxLayout *barLayout;
    QVBoxLayout *mainLayout;

    QScrollableTabBar *tabBar;
    QStackedWidget *stack;
};

#endif // QSCROLLABLETABWIDGETPRIVATE_H
