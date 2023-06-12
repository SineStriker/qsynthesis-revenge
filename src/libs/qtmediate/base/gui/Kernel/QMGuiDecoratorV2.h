#ifndef QMGUIDECORATORV2_H
#define QMGUIDECORATORV2_H

#include <QMCoreDecoratorV2.h>

#include "QMGuiGlobal.h"

class QMGuiDecoratorV2Private;

class QMGUI_EXPORT QMGuiDecoratorV2 : public QMCoreDecoratorV2 {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMGuiDecoratorV2)
public:
    explicit QMGuiDecoratorV2(QObject *parent = nullptr);
    ~QMGuiDecoratorV2();

protected:
    QMGuiDecoratorV2(QMGuiDecoratorV2Private &d, QObject *parent = nullptr);
};

#endif // QMGUIDECORATORV2_H