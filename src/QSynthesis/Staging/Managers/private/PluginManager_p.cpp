#include "PluginManager_p.h"

#include "Api/ISVSConverter.h"
#include "Kernel/QsApplication.h"
#include "Serialization/QJsonFormatter.h"

#include <QDir>

PluginManagerPrivate::PluginManagerPrivate() {
}

PluginManagerPrivate::~PluginManagerPrivate() {
}

void PluginManagerPrivate::init() {
}

void PluginManagerPrivate::loadConverters() {
    QDir dir(qApp->appPluginDir() + "/svsconverters");
    if (!dir.exists()) {
        return;
    }

    // look for C++ plugin
    QFileInfoList files = dir.entryInfoList(QDir::Files);
    for (int k = 0; k < files.count(); ++k) {
        QFileInfo entry = files.at(k);
        QString filePath = entry.canonicalFilePath();
        QPluginLoader *loader = new QPluginLoader(filePath);
        if (loader->load() && qobject_cast<ISVSConverter *>(loader->instance())) {
            QJsonObject meta = loader->metaData().value("MetaData").toObject();
            QJsonValue target = meta.value("target");
            QJsonValue filters = meta.value("filters");
            if (target.isString() && filters.isArray()) {
                int idx = converters.size();
                QStringList filterList;
                QJsonArray filterArr = filters.toArray();
                for (auto it = filterArr.begin(); it != filterArr.end(); ++it) {
                    if (it->isString()) {
                        QString filter = it->toString().toLower();
                        filterList.append(filter);
                        converterMap.insert(filter, idx);
                    }
                }
                converters.append(ConverterInfo{loader, target.toString(), filterList});
                continue;
            }
        }
        loader->unload();
        delete loader;
    }
}

void PluginManagerPrivate::unloadConverters() {
    if (converters.isEmpty()) {
        return;
    }

    for (auto info : qAsConst(converters)) {
        auto loader = info.loader;
        loader->unload();
        delete loader;
    }
    converters.clear();
    converterMap.clear();
}
