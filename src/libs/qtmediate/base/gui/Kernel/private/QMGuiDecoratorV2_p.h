#ifndef QMGUIDECORATORV2_P_H
#define QMGUIDECORATORV2_P_H

#include <private/QMCoreDecoratorV2_p.h>

#include "QMGuiDecoratorV2.h"

class QMGUI_EXPORT QMGuiDecoratorV2Private : public QMCoreDecoratorV2Private {
    Q_DECLARE_PUBLIC(QMGuiDecoratorV2)
public:
    QMGuiDecoratorV2Private();
    ~QMGuiDecoratorV2Private();

    void init();
};

#endif // QMGUIDECORATORV2_P_H
