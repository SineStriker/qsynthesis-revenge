#ifndef QMGUICONSOLE_H
#define QMGUICONSOLE_H

#include <QMCoreConsole.h>

#include "QMGuiGlobal.h"

class QMGuiConsolePrivate;

class QMGUI_EXPORT QMGuiConsole : public QMCoreConsole {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMGuiConsole)
public:
    explicit QMGuiConsole(QObject *parent = nullptr);
    ~QMGuiConsole();

protected:
    QMGuiConsole(QMGuiConsolePrivate &d, QObject *parent = nullptr);
};

#endif // QMGUICONSOLE_H
