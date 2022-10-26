#ifndef QCSSANALYZER_H
#define QCSSANALYZER_H

#include <QByteArray>

class QCssAnalyzer {
public:
    QCssAnalyzer();
    ~QCssAnalyzer();

public:
    QString apply(const QString &stylesheet, double ratio) const;
};

#endif // QCSSANALYZER_H
