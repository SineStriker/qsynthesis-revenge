#ifndef DSSTARTINFO_H
#define DSSTARTINFO_H

#include "QsCoreStartInfo.h"

class DsStartInfo : public QsCoreStartInfo {
public:
    DsStartInfo();
    ~DsStartInfo();

protected:
    void receiveMessage(quint32 instanceId, const QByteArray &message) override;
};

#endif // DSSTARTINFO_H
