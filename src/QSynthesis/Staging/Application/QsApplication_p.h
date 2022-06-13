#ifndef QSAPPLICATIONPRIVATE_H
#define QSAPPLICATIONPRIVATE_H

#include "Private/CApplication_p.h"
#include "QsApplication.h"

#include "Logs/CRecordHolder.h"

#include "DataManager.h"
#include "ExtensionsManager.h"
#include "WindowManager.h"

class QsApplicationPrivate : public CApplicationPrivate {
    Q_DECLARE_PUBLIC(QsApplication)
public:
    QsApplicationPrivate();
    ~QsApplicationPrivate();

    void init();
    void deinit();

    DataManager *data;
    ExtensionsManager *themes;
    WindowManager *windows;

    CRecordHolder *record;

private:
    void initLocale();
    void initFonts();

    void initModules();
    void quitModules();
};

#endif // QSAPPLICATIONPRIVATE_H
