#ifndef QSPLUGINMANAGERPRIVATE_H
#define QSPLUGINMANAGERPRIVATE_H

#include "../QsPluginManager.h"
#include "QsAbstractManager_p.h"

#include <QPluginLoader>

class QSFRAMEWORK_API QsPluginManagerPrivate : public QsAbstractManagerPrivate {
    Q_DECLARE_PUBLIC(QsPluginManager)
public:
    QsPluginManagerPrivate();
    ~QsPluginManagerPrivate();

    void init();
};

#endif // QSPLUGINMANAGERPRIVATE_H
