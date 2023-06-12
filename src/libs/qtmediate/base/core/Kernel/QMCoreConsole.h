#ifndef QMCORECONSOLE_H
#define QMCORECONSOLE_H

#include <QObject>

#include "QMGlobal.h"

#define qmCon QMCoreConsole::instance()

class QMCoreConsolePrivate;

class QMCORE_EXPORT QMCoreConsole : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMCoreConsole)
    Q_SINGLETON(QMCoreConsole)
public:
    explicit QMCoreConsole(QObject *parent = nullptr);
    ~QMCoreConsole();

public:
    enum MessageBoxFlag {
        Critical,
        Warning,
        Question,
        Information,
    };

    /**
     * @brief MsgBox            Execute a message box displaying information
     * @param parent            Parent window or object
     * @param flag              Message type
     * @param title             Message box title
     * @param text              Message box text
     */
    virtual void MsgBox(QObject *parent, MessageBoxFlag flag, const QString &title, const QString &text);
    
protected:
    QMCoreConsole(QMCoreConsolePrivate &d, QObject *parent = nullptr);

    QScopedPointer<QMCoreConsolePrivate> d_ptr;
};

#endif // QMCORECONSOLE_H
