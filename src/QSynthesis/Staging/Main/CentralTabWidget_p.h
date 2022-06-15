#ifndef CENTRALTABWIDGETPRIVATE_H
#define CENTRALTABWIDGETPRIVATE_H

#include "QScrollableTabWidgetImpl/QScrollableTabWidget_p.h"

#include "CentralTabWidget.h"

class CentralTabWidgetPrivate : public QScrollableTabWidgetPrivate {
    Q_DECLARE_PUBLIC(CentralTabWidget)
public:
    CentralTabWidgetPrivate();
    ~CentralTabWidgetPrivate();

    void init();
};

#endif // CENTRALTABWIDGETPRIVATE_H
