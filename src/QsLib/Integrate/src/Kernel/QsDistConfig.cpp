#include "QsDistConfig.h"
#include "SystemHelper.h"
#include "private/QsDistConfig_p.h"

#include "QsStartInfo.h"

#include <QDebug>

static bool hasInit = false;

Q_SINGLETON_DECLARE(QsDistConfig)

QsDistConfig::QsDistConfig() : QsDistConfig(*new QsDistConfigPrivate()) {
}

QsDistConfig::~QsDistConfig() {
}

bool QsDistConfig::load(const QString &filename) {
    Q_D(QsDistConfig);
    bool res = true;
    if (!d->load_helper(filename)) {
        qDebug() << QString("load_config: configuration file %1 not found.")
                        .arg(Sys::PathFindFileName(filename));
        res = false;
    }
    if (qIStup->parser.isSet("reset-config")) {
        d->save_default(filename);
    }
    return res;
}

bool QsDistConfig::apply() {
    Q_D(QsDistConfig);
    return d->apply_helper();
}

QString QsDistConfig::appDir(QsDistConfig::DirType type) const {
    Q_D(const QsDistConfig);
    return d->dirMap.value(type).dir;
}

QString QsDistConfig::locateBinTool(const QString &name) const {
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

QString QsDistConfig::internalPlugin(QsDistConfig::InternalPlugins id) const {
    Q_D(const QsDistConfig);
    return d->pluginMap.value(id).name;
}

QsDistConfig::QsDistConfig(QsDistConfigPrivate &d) : d_ptr(&d) {
    construct();

    d.q_ptr = this;

    d.init();
}

void QsDistConfig::initAll() {
    if (hasInit) {
        return;
    }
    hasInit = true;

    Q_D(QsDistConfig);
    for (const auto &fun : qAsConst(d->initializers)) {
        fun();
    }
}
