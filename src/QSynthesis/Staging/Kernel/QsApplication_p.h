#ifndef QSAPPLICATIONPRIVATE_H
#define QSAPPLICATIONPRIVATE_H

#include "Private/CApplication_p.h"
#include "QsApplication.h"

#include "Managers/FileManager.h"
#include "Managers/PluginManager.h"
#include "Windows/WindowManager.h"

class QsApplicationPrivate : public CApplicationPrivate {
    Q_DECLARE_PUBLIC(QsApplication)
public:
    QsApplicationPrivate();
    ~QsApplicationPrivate();

    PluginManager *pluginMgr;
    FileManager *fileMgr;
    WindowManager *windowMgr;

    void init();
    void deinit();
};

#endif // QSAPPLICATIONPRIVATE_H
