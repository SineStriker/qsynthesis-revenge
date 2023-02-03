#ifndef CDECORATEDIR_H
#define CDECORATEDIR_H

#include "QsFrameworkGlobal.h"
#include "QsLocaleDir.h"

class CDecorateDirPrivate;

class QSFRAMEWORK_API CDecorateDir : public QsLocaleDir {
    Q_DECLARE_PRIVATE(CDecorateDir)
public:
    CDecorateDir();
    explicit CDecorateDir(const QString &dir);
    ~CDecorateDir();

protected:
    CDecorateDir(CDecorateDirPrivate &d);
};

#endif // CDECORATEDIR_H