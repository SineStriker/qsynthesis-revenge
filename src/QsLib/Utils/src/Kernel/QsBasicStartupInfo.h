#ifndef QSBASICSTARTUPINFO_H
#define QSBASICSTARTUPINFO_H

#include <QString>

#ifdef DISABLE_MYCOMMANDLINEPARSER
#include <QCommandLineParser>
using MyCommandLineParser = QCommandLineParser;
#else
#include "MyCommandLineParser.h"
#endif

#ifndef qIStup
#define qIStup QsBasicStartupInfo::instance()
#endif

#include "qsutils_global.h"
#include "qsutils_macros.h"

class QSUTILS_API QsBasicStartupInfo {
    Q_SINGLETON(QsBasicStartupInfo)
public:
    QsBasicStartupInfo();
    virtual ~QsBasicStartupInfo();

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

protected:
    void addInitializer(const std::function<void()> &initializer);

private:
    QList<std::function<void()>> initializers;

public:
    void initAll(); // Called by CApplication at constructor, don't call manually
};

#endif // QSBASICSTARTUPINFO_H
