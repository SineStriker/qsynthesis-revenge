#ifndef QSDISTCONFIG_H
#define QSDISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "qsintegrate_global.h"
#include "qsutils_macros.h"

#ifdef qAppConf
#undef qAppConf
#endif
#define qAppConf QsDistConfig::instance()

class QsDistConfigPrivate;

/*
 * Configuration of the following:
 *   - Important directories
 *   - Default built-in plugins
 *
 * Ways to instantialize:
 *   - Create in `init` of LvApplicationPrivate (Default)
 *   - Inherit LvApplicationPrivate, create and pass to the constructor
 *     of base LvApplicationPrivate, the QsDistConfig can also be inherited
 *
 */

class QSINTEGRATE_API QsDistConfig {
    Q_DECLARE_PRIVATE(QsDistConfig)
    Q_SINGLETON(QsDistConfig)
public:
    QsDistConfig();
    virtual ~QsDistConfig();

    void initAll();

    virtual bool load(const QString &filename);
    virtual bool apply();

public:
    enum DirType {
        AppData,
        AppTemp,
        Plugins,
        BuiltIn,
        BinTool,
        Locales,
        UserDir = 1000,
    };

    QString appDir(DirType type) const;
    QString locateBinTool(const QString &name) const;

    enum PluginType {
        AudioEncoder,
        AudioDecoder,
        AudioPlayback,
        CompressEngine,
        WindowFactory,
        UserPlugin = 1000,
    };

    using InternalPlugins = PluginType;

    QString internalPlugin(InternalPlugins id) const;

protected:
    QsDistConfig(QsDistConfigPrivate &d);

    QScopedPointer<QsDistConfigPrivate> d_ptr;
};

#endif // QSDISTCONFIG_H
