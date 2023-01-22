#ifndef PLAINWINDOWPRIVATE_H
#define PLAINWINDOWPRIVATE_H

#include <QPluginLoader>

#include "../PlainWindow.h"
#include "Api/IWindowHandle.h"

#include "QsIntegrateGlobal.h"

class QSINTEGRATE_API PlainWindowPrivate {
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
