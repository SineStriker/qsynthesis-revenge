#ifndef QREADMETEXT_H
#define QREADMETEXT_H

#include "SpecialConfig.h"
#include "Macros.h"

class QReadmeText : public SpecialConfig {
    Q_CHARSET
public:
    QReadmeText();
    QReadmeText(const QString &dirname);
    virtual ~QReadmeText();

    QString Text;

protected:
    bool loadCore(const QString &filename, bool *valid) override;
    bool saveCore(const QString &filename) override;

    void resetCore() override;

private:
    QString configName() const override;
};

#endif // QREADMETEXT_H
