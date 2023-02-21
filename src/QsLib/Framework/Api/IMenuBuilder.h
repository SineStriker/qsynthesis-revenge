#ifndef IMENUBUILDER_H
#define IMENUBUILDER_H

#include "QsFrameworkGlobal.h"
#include "QMNamePlugin.h"

QSAPI_BEGIN_NAMESPACE

class IMenuBuilder : public QMNamePlugin {
    Q_OBJECT
public:
    explicit IMenuBuilder(QObject *parent = nullptr);
    ~IMenuBuilder();

signals:
};

QSAPI_END_NAMESPACE

#endif // IMENUBUILDER_H
