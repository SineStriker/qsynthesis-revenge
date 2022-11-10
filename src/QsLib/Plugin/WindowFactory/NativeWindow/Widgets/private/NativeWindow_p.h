#ifndef NATIVEWINDOWPRIVATE_H
#define NATIVEWINDOWPRIVATE_H

#include "../NativeWindow.h"

class NativeWindowPrivate {
    Q_DECLARE_PUBLIC(NativeWindow)
public:
    NativeWindowPrivate();

    void init();

    NativeWindow *q_ptr;

    CWindowBarV2 *titleBar;
};

#endif // NATIVEWINDOWPRIVATE_H
