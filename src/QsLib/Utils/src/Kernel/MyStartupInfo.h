#ifndef MYSTARTUPINFO_H
#define MYSTARTUPINFO_H

#include <QCommandLineParser>
#include <QString>

#include "qsutils_global.h"
#include "qsutils_macros.h"

#ifndef qIStup
#define qIStup MyStartupInfo::instance()
#endif

class QSUTILS_API MyStartupInfo {
    Q_SINGLETON(MyStartupInfo)
public:
    MyStartupInfo();
    ~MyStartupInfo();

    /* Properties can be append in each app's constructor */
    QCommandLineParser parser;

    /* Properties setup in main functions */
    QString appName;

    QString appVersion;

    QString appDisplayName;

    bool allowRootUser; // Default: false

    bool allowSecondary; // Default: false

    virtual QString mainTitle() const;

    virtual QString windowTitle() const;

    virtual QString errorTitle() const;
};

#endif // MYSTARTUPINFO_H
