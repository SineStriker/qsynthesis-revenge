#ifndef CSTARTUPINFO_H
#define CSTARTUPINFO_H

#include "QsCoreStartInfo.h"
#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API CStartupInfo : public QsCoreStartInfo {
    Q_OBJECT
public:
    explicit CStartupInfo(QObject *parent = nullptr);
    ~CStartupInfo();

protected:
    QsCoreDecorator *createDecorator(QObject *parent = nullptr) override;
};

#endif // CSTARTUPINFO_H