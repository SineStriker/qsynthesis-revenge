#ifndef QSELEMAPPLICATIONPRIVATE_H
#define QSELEMAPPLICATIONPRIVATE_H

#include "../QsApplication.h"
#include "Private/CApplication_p.h"

#include "Managers/QsFileManager.h"
#include "Managers/QsPluginManager.h"

#include "../QsDistConfig.h"

class QSINTEGRATE_API QsApplicationPrivate : public CApplicationPrivate {
    Q_DECLARE_PUBLIC(QsApplication)
public:
    QsApplicationPrivate(QsDistConfig *conf = nullptr);
    ~QsApplicationPrivate();

    void init();
    void deinit();

    QScopedPointer<QsDistConfig> conf;

    QsPluginManager *pluginMgr;
    QsFileManager *fileMgr;

    // Translators
    bool translate(const QString &filename);
    void eliminate();

    QSet<QTranslator *> translators;

    // Stylesheets
    bool addTheme(const QString &filename);
    void removeThemes();

    QStringList stylesheets;
};

#endif // QSELEMAPPLICATIONPRIVATE_H
