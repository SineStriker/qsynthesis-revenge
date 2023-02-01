#ifndef CDECREATEDIR_H
#define CDECREATEDIR_H

#include "QsFrameworkGlobal.h"
#include "QsLocaleDir.h"

class QSFRAMEWORK_API CDecreateDir : public QsLocaleDir {
public:
    CDecreateDir();
    explicit CDecreateDir(const QString &dir);
    ~CDecreateDir();

public:
    QString themeKey;

    bool load(const QString &filename) override;

private:
    void unloadTheme();
};

#endif // CDECREATEDIR_H
