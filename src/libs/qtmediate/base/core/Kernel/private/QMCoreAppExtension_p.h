#ifndef QMCOREAPPEXTENSION_P_H
#define QMCOREAPPEXTENSION_P_H

#include <QJsonObject>

#include "../QMCoreAppExtension.h"
#include "../QMCoreConsole.h"
#include "../QMCoreDecoratorV2.h"

#include "QMSimpleVarExp.h"

class QMCORE_EXPORT QMCoreInitFactory {
public:
    virtual QMCoreConsole *createConsole(QObject *parent);
    virtual QMCoreDecoratorV2 *createDecorator(QObject *parent);
};

class QMCORE_EXPORT QMCoreAppExtensionPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(QMCoreAppExtension)
public:
    QMCoreAppExtensionPrivate();
    ~QMCoreAppExtensionPrivate();

    void init();

    bool readConfiguration(const QString &fileName);

    QMCoreAppExtension *q_ptr;

    QMSimpleVarExp configVars;

    bool isAboutToQuit;

    QString appDataDir;
    QString tempDir;
    QString libDir;
    QString shareDir;

    QString appShareDir;
    QString appPluginsDir;

    QStringList pluginPaths;
    QStringList translationPaths;
    QStringList themePaths;
    QStringList fontPaths;

    QJsonObject appFont;

    virtual QMCoreInitFactory *createFactory();

private:
    QMCoreDecoratorV2 *s_dec;
    QMCoreConsole *s_cs;
};



#endif // QMCOREAPPEXTENSION_P_H
