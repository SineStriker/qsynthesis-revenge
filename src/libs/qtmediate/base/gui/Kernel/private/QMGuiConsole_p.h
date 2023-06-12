#ifndef QMGUICONSOLE_P_H
#define QMGUICONSOLE_P_H

#include <private/QMCoreConsole_p.h>

#include "QMGuiConsole.h"

class QMGUI_EXPORT QMGuiConsolePrivate : public QMCoreConsolePrivate {
    Q_DECLARE_PUBLIC(QMGuiConsole)
public:
    QMGuiConsolePrivate();
    ~QMGuiConsolePrivate();

    void init();
};

#endif // QMGUICONSOLE_P_H
