#include "QsPluginDir_p.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>

#include "INamePlugin.h"
#include "QsCoreConfig.h"

QsPluginDirPrivate::QsPluginDirPrivate() {
}

QsPluginDirPrivate::~QsPluginDirPrivate() {
    Q_Q(QsPluginDir);
    if (q->autoRemove)
        unloadPlugins();
}

void QsPluginDirPrivate::init() {
    Q_Q(QsPluginDir);
    q->autoRemove = true;
}

void QsPluginDirPrivate::loadPlugins(const QString &dirname) {
    QDir dir(dirname);
    if (!dir.exists()) {
        return;
    }

    // look for C++ plugin
    QFileInfoList files = dir.entryInfoList(QDir::Files);
    for (int k = 0; k < files.count(); ++k) {
        QFileInfo entry = files.at(k);
        QString filePath = entry.canonicalFilePath();
        auto loader = new QPluginLoader(filePath);
        if (INamePlugin::load<INamePlugin>(loader)) {
            QJsonObject meta = loader->metaData().value("MetaData").toObject();

            {
                auto it = meta.find("extra");
                if (it == meta.end() || !it->isObject()) {
                    goto out;
                }

                auto extra = it->toObject();
                it = extra.find("priority");
                if (it == extra.end() || !it->isDouble()) {
                    goto out;
                }

                PriorPlugin pp;
                pp.priority = it->toDouble();
                pp.loader = loader;

                qDebug() << "found prior" << pp.priority
                         << loader->instance()->metaObject()->className();

                // Has plugin with same priority
                if (priorPlugins.find(pp) != priorPlugins.end()) {
                    goto out;
                }

                priorPlugins.insert(pp);
                continue;
            }
        out:
            plugins.append(loader);
            continue;
        }
        loader->unload();
        delete loader;
    }
}

void QsPluginDirPrivate::unloadPlugins() {
    if (priorPlugins.empty() && plugins.isEmpty()) {
        return;
    }

    for (const auto &pp : qAsConst(priorPlugins)) {
        auto loader = pp.loader;
        loader->unload();
        delete loader;
    }
    priorPlugins.clear();

    for (const auto &loader : qAsConst(plugins)) {
        loader->unload();
        delete loader;
    }
    plugins.clear();
}
