#ifndef QSELEMAPPLICATIONPRIVATE_H
#define QSELEMAPPLICATIONPRIVATE_H

#include "../QsApplication.h"

#include "Managers/QsFileManager.h"
#include "Managers/QsPluginManager.h"

#include "CDecreateDir.h"

#include <QCommandLineParser>

class QSINTEGRATE_API QsApplicationPrivate {
    Q_DECLARE_PUBLIC(QsApplication)
public:
    QsApplicationPrivate();
    ~QsApplicationPrivate();

    void init();
    void deinit();

    QsApplication *q_ptr;

    // Managers
    QsPluginManager *pluginMgr;
    QsFileManager *fileMgr;

private:
    CDecreateDir dd;
};

#endif // QSELEMAPPLICATIONPRIVATE_H
