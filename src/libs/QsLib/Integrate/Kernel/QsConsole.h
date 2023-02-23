#ifndef QSCONSOLE_H
#define QSCONSOLE_H

#include "CConsole.h"
#include "QsIntegrateGlobal.h"

#ifdef qCs
#undef qCs
#endif
#define qCs qobject_cast<QsConsole *>(QsCoreConsole::instance())

class QsConsolePrivate;

class QSINTEGRATE_API QsConsole : public CConsole {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsConsole)
public:
    explicit QsConsole(QObject *parent = nullptr);
    ~QsConsole();

public:


protected:
    QsConsole(QsConsolePrivate &d, QObject *parent = nullptr);
};

#endif // QSCONSOLE_H