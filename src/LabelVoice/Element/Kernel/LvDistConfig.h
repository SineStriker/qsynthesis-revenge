#ifndef LVDISTCONFIG_H
#define LVDISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "lvelem_global.h"
#include "qsutils_macros.h"

#define qAppConf LvDistConfig::instance()

class LvDistConfigPrivate;

/*
 * Configuration of the following:
 *   - Important directories
 *   - Default built-in plugins
 *
 * Ways to instantialize:
 *   - Create in `init` of LvApplicationPrivate (Default)
 *   - Inherit LvApplicationPrivate, create and pass to the constructor
 *     of base LvApplicationPrivate, the LvDistConfig can also be inherited
 *
 */

class LVELEM_API LvDistConfig {
    Q_DECLARE_PRIVATE(LvDistConfig)
    Q_SINGLETON(LvDistConfig)
public:
    LvDistConfig();
    virtual ~LvDistConfig();

    virtual bool load(const QString &filename);

public:
    QString dataPath() const;
    QString tempPath() const;
    QString pluginDir() const;
    QString builtinDir() const;
    QString extDir() const;

    enum InternalPlugins {
        AudioDecoder,
        AudioEncoder,
        AudioPlayback,
        CompressEngine,
        WindowFactory,
    };

    QString internalPlugin(InternalPlugins id) const;

protected:
    LvDistConfig(LvDistConfigPrivate &d);

    QScopedPointer<LvDistConfigPrivate> d_ptr;
};

#endif // LVDISTCONFIG_H
