#ifndef LVDISTCONFIG_H
#define LVDISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "lvelem_global.h"
#include "qsutils_macros.h"

#define qAppConf LvDistConfig::instance()

class LvDistConfigPrivate;

class LVELEM_API LvDistConfig {
    Q_DECLARE_PRIVATE(LvDistConfig)
    Q_SINGLETON(LvDistConfig)
public:
    LvDistConfig();
    virtual ~LvDistConfig();

    bool load(const QString &filename);

public:
    QString dataPath() const;
    QString tempPath() const;
    QString pluginDir() const;
    QString builtinDir() const;
    QString extDir() const;

    enum InternalPlugins {
        ZipLib,
        NativeWindow,
    };

    QString internalPlugin(InternalPlugins id);

protected:
    LvDistConfig(LvDistConfigPrivate &d);

    QScopedPointer<LvDistConfigPrivate> d_ptr;
};

#endif // LVDISTCONFIG_H
