#ifndef CSTARTINFO_H
#define CSTARTINFO_H

#include "QsCoreStartInfo.h"
#include "QsFrameworkGlobal.h"

#ifdef qIStup
#    undef qIStup
#endif
#define qIStup qobject_cast<CStartInfo *>(QsCoreStartInfo::instance())

class CStartInfoPrivate;

class QSFRAMEWORK_API CStartInfo : public QsCoreStartInfo {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CStartInfo)
public:
    explicit CStartInfo(QObject *parent = nullptr);
    ~CStartInfo();

protected:
    void loadImpl() override;

protected:
    QsCoreConsole *createConsole(QObject *parent = nullptr) override;

    QsCoreDecorator *createDecorator(QObject *parent = nullptr) override;

protected:
    CStartInfo(CStartInfoPrivate &d, QObject *parent = nullptr);

    QScopedPointer<CStartInfoPrivate> d_ptr;
};

#endif // CSTARTINFO_H
