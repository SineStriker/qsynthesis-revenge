#ifndef LVDISTCONFIGPRIVATE_H
#define LVDISTCONFIGPRIVATE_H

#include "../LvDistConfig.h"

#include <QHash>

class LvDistConfigPrivate {
    Q_DECLARE_PUBLIC(LvDistConfig)
public:
    LvDistConfigPrivate();
    virtual ~LvDistConfigPrivate();

    void init();

    LvDistConfig *q_ptr;

    /* Default dirs */
    QString dataDir;
    QString tempDir;
    QString pluginDir;
    QString builtinDir;
    QString extDir;

    /* Default builtin plugins */
    QString encoder;
    QString decoder;
    QString compression;
    QString playback;
    QString windowFac;

    QHash<QString, QString> vars;

    bool load_helper(const QString &filename);

    bool save_default(const QString &filename);

    void setDefault();

    QString parse(const QString &str) const;
};

#endif // LVDISTCONFIGPRIVATE_H
