#ifndef QUTADISTCONFIG_H
#define QUTADISTCONFIG_H

#include <QScopedPointer>
#include <QStringList>

#include "Kernel/QsDistConfig.h"

#include "qutaelem_global.h"

#ifdef qAppConf
#undef qAppConf
#endif
#define qAppConf QUtaDistConfig::instance()

class QUtaDistConfigPrivate;

class QUTAELEM_API QUtaDistConfig : public QsDistConfig {
    Q_DECLARE_PRIVATE(QUtaDistConfig)
public:
    QUtaDistConfig();
    ~QUtaDistConfig();

    enum ExtendedPlugins {
        OtoPlugin,
        UtaPlugin,
    };

protected:
    QUtaDistConfig(QUtaDistConfigPrivate &d);
};

#endif // QUTADISTCONFIG_H
