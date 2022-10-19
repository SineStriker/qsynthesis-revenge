#ifndef QSAPPLICATIONPRIVATE_H
#define QSAPPLICATIONPRIVATE_H

#include "../QsApplication.h"
#include "Private/CApplication_p.h"

#include "Managers/FileManager.h"
#include "Managers/PluginManager.h"
#include "Managers/WindowManager.h"

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

    void instanceStarted_helper() override;
    void messageReceived_helper(const QStringList &args) override;

    // Data
    QString dataPath;
    QString tempPath;

    QString pluginDir;
#ifdef Q_OS_WINDOWS
    QString runtimeDir;
#endif

    // Translators
    bool translate(const QString &filename);
    void eliminate();

    QSet<QTranslator *> translators;
};

#endif // QSAPPLICATIONPRIVATE_H
