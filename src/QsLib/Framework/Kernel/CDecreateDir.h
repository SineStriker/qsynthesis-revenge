#ifndef CDECREATEDIR_H
#define CDECREATEDIR_H

#include "QSimpleVarExp.h"
#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API CDecreateDir {
public:
    CDecreateDir();
    explicit CDecreateDir(const QString &dir);
    ~CDecreateDir();

public:
    QString dir;
    QSimpleVarExp vars;

    QString localeKey;
    QString themeKey;

    void setDir(const QString &dir);

    bool loadDefault(const QString &binName);
    
    bool load(const QString &filename);
    void unload();
};

#endif // CDECREATEDIR_H