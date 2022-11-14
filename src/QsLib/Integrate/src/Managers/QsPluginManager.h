#ifndef QSQsPluginManager_H
#define QSQsPluginManager_H

#include "Api/ISVSConverter.h"
#include "Kernel/QsDistConfig.h"
#include "QsAbstractManager.h"

#include <QPluginLoader>

#include "qsintegrate_global.h"
#include "qsutils_macros.h"

class QsPluginManagerPrivate;

class QSINTEGRATE_API QsPluginManager : public QsAbstractManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsPluginManager)
    Q_SINGLETON(QsPluginManager)
public:
    QsPluginManager(QObject *parent = nullptr);
    ~QsPluginManager();

public:
    bool load() override;
    bool save() override;

    QString converterFilters() const;
    ISVSConverter *searchConverter(const QString &suffix) const;

    static QPluginLoader *loadInternalPlugin(QsDistConfig::InternalPlugins id);

protected:
    QsPluginManager(QsPluginManagerPrivate &d, QObject *parent = nullptr);
};

#endif // QSQsPluginManager_H
