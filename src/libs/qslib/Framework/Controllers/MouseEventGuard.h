#ifndef MOUSEEVENTGUARD_H
#define MOUSEEVENTGUARD_H

#include <QWidget>

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class MouseEventGuardPrivate;

    class QSFRAMEWORK_API MouseEventGuard : public QObject {
        Q_OBJECT
    public:
        MouseEventGuard(Qt::MouseButton button, const char *slot, QWidget *parent = nullptr);
        ~MouseEventGuard();

    private:
        MouseEventGuardPrivate *d;
    };

} // namespace QsApi

#endif // MOUSEEVENTGUARD_H
