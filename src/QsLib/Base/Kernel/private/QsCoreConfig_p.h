#ifndef QSCORECONFIG_P_H
#define QSCORECONFIG_P_H

#include "../QsCoreConfig.h"

#include "QSimpleVarExp.h"

#include <QHash>

class QSBASE_API QsCoreConfigPrivate {
    Q_DECLARE_PUBLIC(QsCoreConfig)
public:
    QsCoreConfigPrivate();
    virtual ~QsCoreConfigPrivate();

    void init();

    void initByApp();

    QsCoreConfig *q_ptr;

    bool load_helper(const QString &filename);

    bool apply_helper();

    bool save_default(const QString &filename);

    void setDefault();

    /* Escape variables */
    QSimpleVarExp vars;

    /* Default dirs */
    struct DirInitArgs {
        enum CheckLevel {
            OnlyCheck,
            CreateIfNotExist,
            ErrorIfNotExist,
        };
        CheckLevel level;
        QList<std::function<void(const QString &)>> initializers;
    };
    struct DirInfo {
        QString key; // Json key
        QString dir;
        QString defaultDir;
        DirInitArgs initArgs;
    };
    QHash<int, DirInfo> dirMap;
    int dirTypeMax;

    /* Default builtin plugins */
    struct PluginInfo {
        QString key; // Json key
        QString catagory;
        QString name;
        QString defaultName;
    };
    QHash<int, PluginInfo> pluginMap;
    int pluginTypeMax;

    /* User registry */
    void registerEscapeVar(const QString &key, const QString &val, bool replace = true);

    int registerUserDir(const QString &key, const QString &dir, const DirInitArgs &args,
                        int hint = -1);

    int registerUserPlugin(const QString &key, const QString &catagory, const QString &name,
                           int hint = -1);

    void setDefaultDir(int type, const QString &dir);

    void setDefaultPluginName(int type, const QString &dir);

    void addInitializer(const std::function<void()> &fun);

private:
    QList<std::function<void()>> initializers;

    void initAll();

    friend class QsCoreStartInfoPrivate;
};

#endif // QSCORECONFIG_P_H
