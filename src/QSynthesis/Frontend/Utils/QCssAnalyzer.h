#ifndef QCSSANALYZER_H
#define QCSSANALYZER_H

#include <QFile>

class QCssAnalyzer : public QFile {
#ifndef QT_NO_QOBJECT
    Q_OBJECT
#endif
public:
    QCssAnalyzer();
    QCssAnalyzer(const QString &filename);
    ~QCssAnalyzer();

public:
    double ratio() const;
    void setRatio(double ratio);

    QByteArray readAndApply();

private:
    double m_ratio;
};

#endif // QCSSANALYZER_H
