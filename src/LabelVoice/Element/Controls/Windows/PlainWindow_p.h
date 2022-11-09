#ifndef PLAINWINDOWPRIVATE_H
#define PLAINWINDOWPRIVATE_H

#include <QPluginLoader>

#include "Api/IWindowHandle.h"
#include "PlainWindow.h"

#include "lvelem_global.h"

class LVELEM_API PlainWindowPrivate {
    Q_DECLARE_PUBLIC(PlainWindow)
public:
    PlainWindowPrivate();
    virtual ~PlainWindowPrivate();

    void init();

    PlainWindow *q_ptr;

    QPluginLoader *loader;
    IWindowHandle *winHandle;
};

#endif // PLAINWINDOWPRIVATE_H
