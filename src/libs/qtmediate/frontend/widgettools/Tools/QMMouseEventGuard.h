#ifndef QMMOUSEEVENTGUARD_H
#define QMMOUSEEVENTGUARD_H

#include <QMouseEvent>

#include "QMWidgetToolsGlobal.h"

class QMMouseEventGuardPrivate;

class QMWTOOLS_EXPORT QMMouseEventGuard : public QObject {
    Q_OBJECT
public:
    QMMouseEventGuard(Qt::MouseButton button, const char *slot, QWidget *parent = nullptr);
    ~QMMouseEventGuard();

private:
    QMMouseEventGuardPrivate *d;
};

#endif // QMMOUSEEVENTGUARD_H
