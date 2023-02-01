#include "QsCoreConfig.h"
#include "QsSystem.h"
#include "private/QsCoreConfig_p.h"

#include "QsCoreStartInfo.h"

#include <QDebug>

static bool hasInit = false;

Q_SINGLETON_DECLARE(QsCoreConfig)

QsCoreConfig::QsCoreConfig() : QsCoreConfig(*new QsCoreConfigPrivate()) {
}

QsCoreConfig::~QsCoreConfig() {
}

bool QsCoreConfig::load(const QString &filename) {
    Q_D(QsCoreConfig);
    bool res = true;
    if (!d->load_helper(filename)) {
        qDebug() << QString("load_config: configuration file %1 not found.")
                        .arg(QsSys::PathFindFileName(filename));
        res = false;
    }
    return res;
}

bool QsCoreConfig::apply() {
    Q_D(QsCoreConfig);
    return d->apply_helper();
}

void QsCoreConfig::saveDefault(const QString &filename) {
    Q_D(QsCoreConfig);
    d->save_default(filename);
}

QString QsCoreConfig::appDir(QsCoreConfig::DirType type) const {
    Q_D(const QsCoreConfig);
    return d->dirMap.value(type).dir;
}

QString QsCoreConfig::locateBinTool(const QString &name) const {
#ifdef Q_OS_WINDOWS
    const char _suffix[] = ".exe";
#else
    const char _suffix[] = "";
#endif
    QString filename = appDir(BinTool) + '/' + name + _suffix;
    if (QFileInfo(filename).isExecutable()) {
        return filename;
    }
    return QString();
}

QString QsCoreConfig::internalPlugin(QsCoreConfig::InternalPlugins id) const {
    Q_D(const QsCoreConfig);
    return d->pluginMap.value(id).name;
}

QsCoreConfig::QsCoreConfig(QsCoreConfigPrivate &d) : d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}

void QsCoreConfig::initAll() {
    if (hasInit) {
        return;
    }
    hasInit = true;

    Q_D(QsCoreConfig);
    for (const auto &fun : qAsConst(d->initializers)) {
        fun();
    }
}
