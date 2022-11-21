#ifndef QCSSANALYZER_H
#define QCSSANALYZER_H

#include <QByteArray>

#include "qsintegrate_global.h"

class QSINTEGRATE_API QCssAnalyzer {
public:
    QCssAnalyzer();
    ~QCssAnalyzer();

public:
    QString apply(const QString &stylesheet, double ratio) const;
};

#endif // QCSSANALYZER_H
