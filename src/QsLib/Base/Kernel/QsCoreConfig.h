#ifndef QSCORECONFIG_H
#define QSCORECONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "QsGlobal.h"
#include "QsMacros.h"

#define qAppConf QsCoreConfig::instance()

class QsCoreConfigPrivate;

/*
 * Configuration of the following:
 *   - Important directories
 *   - Default built-in plugins
 *
 * Ways to instantialize:
 *   - Create in `init` of LvApplicationPrivate (Default)
 *   - Inherit LvApplicationPrivate, create and pass to the constructor
 *     of base LvApplicationPrivate, the QsCoreConfig can also be inherited
 *
 */

class QSBASE_API QsCoreConfig {
    Q_DECLARE_PRIVATE(QsCoreConfig)
    Q_SINGLETON(QsCoreConfig)
public:
    QsCoreConfig();
    virtual ~QsCoreConfig();

    virtual bool load(const QString &filename);
    virtual bool apply();

    void saveDefault(const QString &filename);

public:
    enum DirType {
        AppData,
        AppTemp,
        AppShare,
        QtPlugins,
        QtTranslations,
        QsPlugins,
        BinTool,
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
    QsCoreConfig(QsCoreConfigPrivate &d);

    QScopedPointer<QsCoreConfigPrivate> d_ptr;

    friend class QsCoreStartInfo;
    friend class QsCoreStartInfoPrivate;;
};

#endif // QSCORECONFIG_H