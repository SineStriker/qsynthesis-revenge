#ifndef CCONSOLE_H
#define CCONSOLE_H

#include "QsCoreConsole.h"
#include "QMWidgetsGlobal.h"

#ifdef qmConsole
#undef qmConsole
#endif
#define qmConsole qobject_cast<CConsole *>(QsCoreConsole::instance())

class CConsolePrivate;

class QMWIDGETS_API CConsole : public QsCoreConsole {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CConsole)
public:
    explicit CConsole(QObject *parent = nullptr);
    ~CConsole();

    void MsgBox(QObject *parent, MessageBoxFlag flag, const QString &title,
                const QString &text) override;

protected:
    CConsole(CConsolePrivate &d, QObject *parent = nullptr);
};

#endif // CCONSOLE_H
