#ifndef QCSSANALYZER_H
#define QCSSANALYZER_H

#include <QByteArray>

#include "lvelem_global.h"

class LVELEM_API QCssAnalyzer {
public:
    QCssAnalyzer();
    ~QCssAnalyzer();

public:
    QString apply(const QString &stylesheet, double ratio) const;
};

#endif // QCSSANALYZER_H
