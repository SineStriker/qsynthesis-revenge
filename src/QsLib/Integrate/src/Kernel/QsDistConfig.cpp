#include "QsDistConfig.h"
#include "SystemHelper.h"
#include "private/QsDistConfig_p.h"

#include <QDebug>

Q_SINGLETON_DECLARE(QsDistConfig)

QsDistConfig::QsDistConfig() : QsDistConfig(*new QsDistConfigPrivate()) {
}

QsDistConfig::~QsDistConfig() {
}

void QsDistConfig::initAll() {
    Q_D(QsDistConfig);
    for (auto it = d->initializers.begin(); it != d->initializers.end(); ++it) {
        (*it)();
    }
}

bool QsDistConfig::load(const QString &filename) {
    Q_D(QsDistConfig);
    if (!d->load_helper(filename)) {
        qDebug() << QString("load_config: configuration file %1 not found.")
                        .arg(Sys::PathFindFileName(filename));
         d->save_default(filename);
        return false;
    }
    return true;
}

QString QsDistConfig::appDir(QsDistConfig::DirType type) const {
    Q_D(const QsDistConfig);
    return d->dirMap.value(type).dir;
}

QString QsDistConfig::internalPlugin(QsDistConfig::InternalPlugins id) const {
    Q_D(const QsDistConfig);
    return d->pluginMap.value(id).name;
}

QsDistConfig::QsDistConfig(QsDistConfigPrivate &d) : d_ptr(&d) {
    construct();

    d.q_ptr = this;

    d.init();
}

bool QsDistConfig::apply() {
    Q_D(QsDistConfig);
    return d->apply_helper();
}
