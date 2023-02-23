#ifndef NATIVEHANDLE_P_H
#define NATIVEHANDLE_P_H

#include "../NativeHandle.h"
#include "Api/private/IWindowHandle_p.h"
#include "widgets/CWindowBarV2.h"

#include <FramelessWidgetsHelper>
#include <private/widgetssharedhelper_p.h>
#include <private/framelessconfig_p.h>

FRAMELESSHELPER_USE_NAMESPACE

class NativeHandlePrivate : public IWindowHandlePrivate {
    Q_DECLARE_PUBLIC(NativeHandle)
public:
    NativeHandlePrivate();
    ~NativeHandlePrivate();

    void init();

    void updateTitleBar() override;

    void setup_helper();
    void setMenuBar_helper(QMenuBar *menuBar);

    void installTitleBar();

    QScopedPointer<WidgetsSharedHelper> m_helper;
    CWindowBarV2 *titleBar;
};

#endif // NATIVEHANDLE_P_H
