#ifndef DSSTARTINFO_H
#define DSSTARTINFO_H

#include "QsStartInfo.h"

class DsStartInfoPrivate;

class DsStartInfo : public QsStartInfo {
    Q_OBJECT
    Q_DECLARE_PRIVATE(DsStartInfo)
public:
    explicit DsStartInfo(QObject *parent = nullptr);
    ~DsStartInfo();

    void load() override;
    void save() override;

protected:
    QsCoreConfig *creatDistConfig() override;

protected:
    DsStartInfo(DsStartInfoPrivate &d, QObject *parent = nullptr);
};

#endif // DSSTARTINFO_H
