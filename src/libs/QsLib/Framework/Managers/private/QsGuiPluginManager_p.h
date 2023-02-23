#ifndef QSGUIPLUGINMANAGERPRIVATE_H
#define QSGUIPLUGINMANAGERPRIVATE_H

#include "../QsGuiPluginManager.h"
#include "private/QsPluginManager_p.h"

class QsGuiPluginManagerPrivate : public QsPluginManagerPrivate {
    Q_DECLARE_PUBLIC(QsGuiPluginManager)
public:
    QsGuiPluginManagerPrivate();
    ~QsGuiPluginManagerPrivate();

    void init();
};

#endif // QSGUIPLUGINMANAGERPRIVATE_H
