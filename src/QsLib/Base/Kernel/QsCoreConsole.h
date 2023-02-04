#ifndef QSCORECONSOLE_H
#define QSCORECONSOLE_H

#include <QObject>

#include "QsGlobal.h"
#include "QsMacros.h"

#define qCs QsCoreConsole::instance()

class QsCoreConsolePrivate;

class QSBASE_API QsCoreConsole : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsCoreConsole)
    Q_SINGLETON(QsCoreConsole)
public:
    explicit QsCoreConsole(QObject *parent = nullptr);
    ~QsCoreConsole();

    enum MessageBoxFlag {
        Critical,
        Warning,
        Question,
        Information,
    };

    virtual void MsgBox(QObject *parent, MessageBoxFlag flag, const QString &title,
                            const QString &text);

protected:
    QsCoreConsole(QsCoreConsolePrivate &d, QObject *parent = nullptr);

    QScopedPointer<QsCoreConsolePrivate> d_ptr;
};

#endif // QSCORECONSOLE_H
