#ifndef QSPLUGINMANAGERPRIVATE_H
#define QSPLUGINMANAGERPRIVATE_H

#include "../QsPluginManager.h"
#include "QsAbstractManager_p.h"

#include "Api/ISVSConverter.h"

#include <QPluginLoader>

class QSINTEGRATE_API QsPluginManagerPrivate : public QsAbstractManagerPrivate {
    Q_DECLARE_PUBLIC(QsPluginManager)
public:
    QsPluginManagerPrivate();
    ~QsPluginManagerPrivate();

    void init();

    void loadConverters();
    void unloadConverters();

    struct ConverterInfo {
        QPluginLoader *loader;
        QString target;
        QStringList filters;
    };
    QList<ConverterInfo> converters;
    QMap<QString, int> converterMap;
};

#endif // QSPLUGINMANAGERPRIVATE_H
