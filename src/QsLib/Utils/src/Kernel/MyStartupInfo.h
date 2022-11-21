#ifndef MYSTARTUPINFO_H
#define MYSTARTUPINFO_H

#include <QString>

#ifdef DISABLE_MYCOMMANDLINEPARSER
#include <QCommandLineParser>
using MyCommandLineParser = QCommandLineParser;
#else
#include "MyCommandLineParser.h"
#endif

#ifndef qIStup
#define qIStup MyStartupInfo::instance()
#endif

#include "qsutils_global.h"
#include "qsutils_macros.h"

class QSUTILS_API MyStartupInfo {
    Q_SINGLETON(MyStartupInfo)
public:
    MyStartupInfo();
    ~MyStartupInfo();

    /* Properties can be append in each app's constructor */
    MyCommandLineParser parser;

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

#endif // MYSTARTUPINFO_H
