#include "LvDistConfig.h"
#include "SystemHelper.h"
#include "private/LvDistConfig_p.h"

#include <QDebug>

Q_SINGLETON_DECLARE(LvDistConfig)

LvDistConfig::LvDistConfig() : LvDistConfig(*new LvDistConfigPrivate()) {
}

LvDistConfig::~LvDistConfig() {
}

bool LvDistConfig::load(const QString &filename) {
    Q_D(LvDistConfig);
    if (!d->load_helper(filename)) {
        qDebug() << QString("load_config: configuration file %1 not found.")
                        .arg(Sys::PathFindFileName(filename));
        return false;
    }
    return true;
}

QString LvDistConfig::dataPath() const {
    Q_D(const LvDistConfig);
    return d->dataDir;
}

QString LvDistConfig::tempPath() const {
    Q_D(const LvDistConfig);
    return d->tempDir;
}

QString LvDistConfig::pluginDir() const {
    Q_D(const LvDistConfig);
    return d->pluginDir;
}

QString LvDistConfig::builtinDir() const {
    Q_D(const LvDistConfig);
    return d->builtinDir;
}

QString LvDistConfig::extDir() const {
    Q_D(const LvDistConfig);
    return d->extDir;
}

QString LvDistConfig::internalPlugin(LvDistConfig::InternalPlugins id) {
    Q_D(LvDistConfig);

    QString res;
    switch (id) {
        case ZipLib:
            res = d->compression;
            break;
        case NativeWindow:
            res = d->windowFac;
            break;
        default:
            break;
    }

    return res;
}

LvDistConfig::LvDistConfig(LvDistConfigPrivate &d) : d_ptr(&d) {
    construct();

    d.q_ptr = this;

    d.init();
}
