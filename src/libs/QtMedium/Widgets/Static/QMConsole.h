#ifndef QMCONSOLE_H
#define QMCONSOLE_H

#include "QMCoreConsole.h"
#include "QMWidgetsGlobal.h"

#ifdef qmCon
#    undef qmCon
#endif
#define qmCon qobject_cast<QMConsole *>(QMCoreConsole::instance())

class QMConsolePrivate;

class QMWIDGETS_API QMConsole : public QMCoreConsole {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMConsole)
public:
    explicit QMConsole(QObject *parent = nullptr);
    ~QMConsole();

    void MsgBox(QObject *parent, MessageBoxFlag flag, const QString &title, const QString &text) override;

protected:
    QMConsole(QMConsolePrivate &d, QObject *parent = nullptr);
};

#endif // QMCONSOLE_H
