#ifndef IUTAPLUGIN_H
#define IUTAPLUGIN_H

#include <QStringList>

#include "Utau/QUstFile.h"
#include "Utau/QUstSettings.h"
#include "Utau/QUstVersion.h"

#include "qssvs_global.h"

class Q_DECL_EXPORT IUtaPlugin {
public:
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

    virtual ~IUtaPlugin(){};

    virtual int exec(PluginArguments *args) = 0;
};

#define IUtaPlugin_IID "QSynthesis.Plugin.UstConverter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IUtaPlugin, IUtaPlugin_IID)
QT_END_NAMESPACE

#endif // IUTAPLUGIN_H
