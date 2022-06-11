#ifndef QSAPPLICATIONPRIVATE_H
#define QSAPPLICATIONPRIVATE_H

#include "Private/CApplication_p.h"
#include "QsApplication.h"

#include "Logs/CRecordHolder.h"
#include "Managers/ColorTheme.h"
#include "Managers/DataManager.h"

class QsApplicationPrivate : public CApplicationPrivate {
    Q_DECLARE_PUBLIC(QsApplication)
public:
    QsApplicationPrivate();
    ~QsApplicationPrivate();

    void init();
    void deinit();

    DataManager* data;
    ColorTheme* themes;
    CRecordHolder* record;

private:
    void initLocale();
    void initFonts();

    void initModules();
    void quitModules();

    void initLogs();
    void quitLogs();
};

#endif // QSAPPLICATIONPRIVATE_H
