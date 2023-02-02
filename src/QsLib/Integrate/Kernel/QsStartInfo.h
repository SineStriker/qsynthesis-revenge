#ifndef QSSTARTINFO_H
#define QSSTARTINFO_H

#include "CStartInfo.h"
#include "QsIntegrateGlobal.h"

class QsStartInfoPrivate;

class QSINTEGRATE_API QsStartInfo : public CStartInfo {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsStartInfo)
public:
    explicit QsStartInfo(QObject *parent = nullptr);
    ~QsStartInfo();
    
    void load() override;

public:
    static QString untitledFileName();

    static QString unsavedPrefix();
    
    static QString deletedPrefix();

protected:
    QsStartInfo(QsStartInfoPrivate &d, QObject *parent = nullptr);
};

#endif // QSSTARTINFO_H
