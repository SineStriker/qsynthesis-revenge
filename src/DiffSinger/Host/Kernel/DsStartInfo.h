#ifndef DSSTARTINFO_H
#define DSSTARTINFO_H

#include "QsStartInfo.h"

#ifdef qIStup
#undef qIStup
#endif
#define qIStup qobject_cast<DsStartInfo *>(QsCoreStartInfo::instance())

class DsStartInfoPrivate;

class DsStartInfo : public QsStartInfo {
    Q_OBJECT
    Q_DECLARE_PRIVATE(DsStartInfo)
public:
    explicit DsStartInfo(QObject *parent = nullptr);
    ~DsStartInfo();

    void load() override;

protected:
    QsCoreConsole *createConsole(QObject *parent = nullptr) override;

    QsCoreConfig *creatDistConfig() override;

protected:
    DsStartInfo(DsStartInfoPrivate &d, QObject *parent = nullptr);
};

#endif // DSSTARTINFO_H
