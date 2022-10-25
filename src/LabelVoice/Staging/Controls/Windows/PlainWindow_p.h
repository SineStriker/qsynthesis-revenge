#ifndef PLAINWINDOWPRIVATE_H
#define PLAINWINDOWPRIVATE_H

#include "PlainWindow.h"

#include "Api/IWindowHandle.h"

#include <QPluginLoader>

class PlainWindowPrivate {
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
