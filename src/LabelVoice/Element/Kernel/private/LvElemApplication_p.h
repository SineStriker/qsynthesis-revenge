#ifndef LVELEMAPPLICATIONPRIVATE_H
#define LVELEMAPPLICATIONPRIVATE_H

#include "../LvElemApplication.h"
#include "Private/CApplication_p.h"

#include "Managers/FileManager.h"
#include "Managers/PluginManager.h"
//#include "Managers/WindowManager.h"

class LVELEM_API LvElemApplicationPrivate : public CApplicationPrivate {
    Q_DECLARE_PUBLIC(LvElemApplication)
public:
    LvElemApplicationPrivate();
    ~LvElemApplicationPrivate();

    PluginManager *pluginMgr;
    FileManager *fileMgr;

    void init();
    void deinit();

    // Data
    QString dataPath;
    QString tempPath;

    QString pluginDir;
    QString extDir;

    // Translators
    bool translate(const QString &filename);
    void eliminate();

    QSet<QTranslator *> translators;

    // Stylesheets
    bool addTheme(const QString &filename);
    void removeThemes();

    QStringList stylesheets;
};

#endif // LVELEMAPPLICATIONPRIVATE_H
