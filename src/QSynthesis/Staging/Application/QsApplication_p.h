#ifndef QSAPPLICATIONPRIVATE_H
#define QSAPPLICATIONPRIVATE_H

#include "Private/CApplication_p.h"
#include "QsApplication.h"

#include "Managers/DataManager.h"
#include "Managers/WindowManager.h"
#include "Managers/ExtentManager.h"

class QsApplicationPrivate : public CApplicationPrivate {
    Q_DECLARE_PUBLIC(QsApplication)
public:
    QsApplicationPrivate();
    ~QsApplicationPrivate();

    void init();
    void deinit();

    DataManager *dataMgr;
    WindowManager *winMgr;
    ExtentManager *extMgr;

private:
    void initLocale();
    void initFonts();

    void initModules();
    void quitModules();
};

#endif // QSAPPLICATIONPRIVATE_H
