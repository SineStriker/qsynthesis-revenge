#ifndef PLAINWINDOW_P_H
#define PLAINWINDOW_P_H

#include <QPluginLoader>

#include "../PlainWindow.h"
#include "Api/IWindowHandle.h"

#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API PlainWindowPrivate {
    Q_DECLARE_PUBLIC(PlainWindow)
public:
    PlainWindowPrivate();
    virtual ~PlainWindowPrivate();

    void init();

    PlainWindow *q_ptr;
    QsApi::IWindowHandle *winHandle;
};

#endif // PLAINWINDOW_P_H
