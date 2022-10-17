#ifndef ISVSCONVERTER_H
#define ISVSCONVERTER_H

#include <QStringList>

#include <functional>

#include "Utau/QUstFile.h"

#include "qssvs_global.h"

class Q_DECL_EXPORT ISVSConverter {
public:
    struct SelectArguments {
        QString title;
        QString caption;
        QStringList options;
        bool single;
    };

    typedef std::function<QList<bool>(SelectArguments)> Callback;

    virtual ~ISVSConverter(){};

    virtual bool load(const QString &filename, QUstFile *out, Callback callback) = 0;
    virtual bool save(const QString &filename, const QUstFile &in) = 0;
};

#define IFormatImporter_IID "QSynthesis.Plugin.SVSConverter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ISVSConverter, IFormatImporter_IID)
QT_END_NAMESPACE

#endif // ISVSCONVERTER_H
