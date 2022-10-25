#ifndef LVAPPLICATIONPRIVATE_H
#define LVAPPLICATIONPRIVATE_H

#include "../LvApplication.h"
#include "Private/CApplication_p.h"

#include "Managers/FileManager.h"
#include "Managers/PluginManager.h"
#include "Managers/WindowManager.h"

class LvApplicationPrivate : public CApplicationPrivate {
    Q_DECLARE_PUBLIC(LvApplication)
public:
    LvApplicationPrivate();
    ~LvApplicationPrivate();

    PluginManager *pluginMgr;
    FileManager *fileMgr;
    WindowManager *windowMgr;

    void init();
    void deinit();

    void instanceStarted_helper() override;
    void messageReceived_helper(const QStringList &args) override;

    // Data
    QString dataPath;
    QString tempPath;

    QString pluginDir;

    // Translators
    bool translate(const QString &filename);
    void eliminate();

    QSet<QTranslator *> translators;
};

#endif // LVAPPLICATIONPRIVATE_H
