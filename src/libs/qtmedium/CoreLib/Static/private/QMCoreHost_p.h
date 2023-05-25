#ifndef QMCOREHOST_P_H
#define QMCOREHOST_P_H

#include "../QMCoreHost.h"

#include "QMCoreFactory.h"

class QMCORELIB_API QMCoreHostPrivate {
    Q_DECLARE_PUBLIC(QMCoreHost)
public:
    QMCoreHostPrivate();
    virtual ~QMCoreHostPrivate();

    void init();

    QMCoreHost *q_ptr;
    
    bool isAboutToQuit;
    QSettings::Format settingsJsonFormat;

    QString appDataDir;
    QString tempDir;
    QString libDir;
    QString shareDir;

    QStringList translationPaths;
    QStringList themePaths;
    QStringList fontPaths;
    QString appFont;

    virtual QMCoreFactory *createFactory();

private:
    QScopedPointer<QMCoreFactory> fac;

    QMCoreDecoratorV2 *s_dec;
    QMCoreConsole *s_cs;
};

#endif // QMCOREHOST_P_H
