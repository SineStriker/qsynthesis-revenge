#ifndef CStartupInfo_H
#define CStartupInfo_H

#include <QCommandLineParser>
#include <QString>

#include "QsFrameworkGlobal.h"
#include "QsMacros.h"

#ifndef qIStup
#define qIStup CStartupInfo::instance()
#endif

class QSFRAMEWORK_API CStartupInfo {
    Q_SINGLETON(CStartupInfo)
public:
    CStartupInfo();
    virtual ~CStartupInfo();

    /* Properties can be append in each app's constructor */
    QCommandLineParser parser;

    /* Properties setup in main functions */
    QString qsLibPath;

    QString appName;

    QString appVersion;

    QString appDisplayName;

    QString appDescription;

    bool allowRootUser; // Default: false

    bool allowSecondary; // Default: false

    virtual QString mainTitle() const;

    virtual QString windowTitle() const;

    virtual QString errorTitle() const;
};

#endif // CStartupInfo_H
