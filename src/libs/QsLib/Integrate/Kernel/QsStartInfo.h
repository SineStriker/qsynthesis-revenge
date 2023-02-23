#ifndef QSSTARTINFO_H
#define QSSTARTINFO_H

#include "CStartInfo.h"
#include "QsIntegrateGlobal.h"

#ifdef qIStup
#undef qIStup
#endif
#define qIStup qobject_cast<QsStartInfo *>(QsCoreStartInfo::instance())

class QsStartInfoPrivate;

class QSINTEGRATE_API QsStartInfo : public CStartInfo {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsStartInfo)
public:
    explicit QsStartInfo(QObject *parent = nullptr);
    ~QsStartInfo();

protected:
    void loadImpl() override;

public:
    static QString untitledFileName();

    static QString unsavedPrefix();

    static QString deletedPrefix();

protected:
    QsCoreConsole *createConsole(QObject *parent = nullptr) override;

protected:
    QsStartInfo(QsStartInfoPrivate &d, QObject *parent = nullptr);
};

#endif // QSSTARTINFO_H
