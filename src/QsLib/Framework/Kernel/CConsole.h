#ifndef CCONSOLE_H
#define CCONSOLE_H

#include "QsCoreConsole.h"
#include "QsFrameworkGlobal.h"

#ifdef qCs
#undef qCs
#endif
#define qCs qobject_cast<CConsole *>(QsCoreConsole::instance())

class CConsolePrivate;

class QSFRAMEWORK_API CConsole : public QsCoreConsole {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CConsole)
public:
    explicit CConsole(QObject *parent = nullptr);
    ~CConsole();

    void MsgBox(QObject *parent, MessageBoxFlag flag, const QString &title,
                const QString &text) override;

protected:
    QsFileManager *createFileManager(QObject *parent = nullptr) override;

    QsPluginManager *createPluginManager(QObject *parent = nullptr) override;

protected:
    CConsole(CConsolePrivate &d, QObject *parent = nullptr);
};

#endif // CCONSOLE_H
