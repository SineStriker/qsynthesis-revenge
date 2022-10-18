#ifndef IUTAPLUGIN_H
#define IUTAPLUGIN_H

#include <QJsonObject>
#include <QObject>
#include <QStringList>

#include "Utau/QUstFile.h"
#include "Utau/QUstSettings.h"
#include "Utau/QUstVersion.h"

#include "qssvs_global.h"

class QSSVS_API IUtaPlugin : public QObject {
    Q_OBJECT
public:
    IUtaPlugin(QObject *parent = nullptr);
    ~IUtaPlugin();

    struct NoteReadonly {
        double preuttr;
        double overlap;
        double stpoint;
        QString filename;
        QString alias;
        QString cache;
    };

    struct NoteItem {
        QUstNote note;
        NoteReadonly readonly;
    };

    struct PluginArguments {
        QString project;
        QUstSettings settings;
        QUstVersion version;
        QList<NoteItem> notes;
        bool hasPrev;
        bool hasNext;
        int startSeq;
    };

    /**
     *  Useful information for plugin to acquire
     */
    QString path;

    /**
     * @brief exec: Start plugin and block main thread
     * @param args: Piano role selections
     * @param userdata: User data if necessary
     * @return 0 as committed, 1 as aborted and negative number as error
     */
    virtual int exec(PluginArguments *args, void *userdata) = 0;
};

#define IUtaPlugin_IID "QSynthesis.Plugin.SVS.UstConverter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IUtaPlugin, IUtaPlugin_IID)
QT_END_NAMESPACE

#endif // IUTAPLUGIN_H
