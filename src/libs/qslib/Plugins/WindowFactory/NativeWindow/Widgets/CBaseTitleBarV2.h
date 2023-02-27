#ifndef CBASETITLEBARV2_H
#define CBASETITLEBARV2_H

#include <QWidget>

#include "ANativeTitleBar.h"

class Q_DECL_EXPORT CBaseTitleBarV2 : public ANativeTitleBar {
    Q_OBJECT
public:
    explicit CBaseTitleBarV2(QWidget *parent = nullptr);
    ~CBaseTitleBarV2();
};

#endif // CBASETITLEBARV2_H
