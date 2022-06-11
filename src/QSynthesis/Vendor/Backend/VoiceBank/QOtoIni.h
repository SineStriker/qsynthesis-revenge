#ifndef QOTOINI_H
#define QOTOINI_H

#include "Macros.h"
#include "QOtoSample.h"
#include "SpecialConfig.h"

class QOtoIni : public SpecialConfig {
    Q_CHARSET
public:
    QOtoIni();
    QOtoIni(const QString &dirname);
    ~QOtoIni();

    QMap<QString, QOtoSample> OtoSamples;

protected:
    bool loadCore(const QString &filename, bool *valid) override;
    bool saveCore(const QString &filename) override;

    void resetCore() override;

private:
    QString configName() const override;
};

#endif // QOTOINI_H
