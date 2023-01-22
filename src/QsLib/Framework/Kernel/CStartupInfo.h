#ifndef QSBASICSTARTUPINFO_H
#define QSBASICSTARTUPINFO_H

#include <QCommandLineParser>
#include <QString>

#include "QsFrameworkGlobal.h"
#include "QsMacros.h"

#ifndef qIStup
#define qIStup QsBasicStartupInfo::instance()
#endif

class QSFRAMEWORK_API QsBasicStartupInfo {
    Q_SINGLETON(QsBasicStartupInfo)
public:
    QsBasicStartupInfo();
    virtual ~QsBasicStartupInfo();

    /* Properties can be append in each app's constructor */
    QCommandLineParser parser;

    /* Properties setup in main functions */
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

#endif // QSBASICSTARTUPINFO_H
