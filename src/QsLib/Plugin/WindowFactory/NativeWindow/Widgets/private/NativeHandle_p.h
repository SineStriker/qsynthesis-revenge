#ifndef NATIVEHANDLEPRIVATE_H
#define NATIVEHANDLEPRIVATE_H

#include "../CWindowBarV2.h"
#include "../NativeHandle.h"
#include "Api/private/IWindowHandle_p.h"

#include <FramelessWidgetsHelper>
#include <private/widgetssharedhelper_p.h>

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

#endif // NATIVEHANDLEPRIVATE_H
