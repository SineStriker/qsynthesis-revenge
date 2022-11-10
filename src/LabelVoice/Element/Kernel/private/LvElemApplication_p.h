#ifndef LVELEMAPPLICATIONPRIVATE_H
#define LVELEMAPPLICATIONPRIVATE_H

#include "../LvElemApplication.h"
#include "Private/CApplication_p.h"

#include "Managers/FileManager.h"
#include "Managers/PluginManager.h"

#include "../LvDistConfig.h"

class LVELEM_API LvElemApplicationPrivate : public CApplicationPrivate {
    Q_DECLARE_PUBLIC(LvElemApplication)
public:
    LvElemApplicationPrivate(LvDistConfig *conf = nullptr);
    ~LvElemApplicationPrivate();

    void init();
    void deinit();

    QScopedPointer<LvDistConfig> conf;

    PluginManager *pluginMgr;
    FileManager *fileMgr;

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
