#ifndef IFORMATIMPORTER_H
#define IFORMATIMPORTER_H

#include <QStringList>

#include <functional>

#include "Utau/QUstFile.h"

#include "qssvs_global.h"

class Q_DECL_EXPORT IFormatImporter {
public:
    struct SelectArguments {
        QString title;
        QString caption;
        QStringList options;
        bool single;
    };

    typedef std::function<QList<bool>(SelectArguments)> Callback;

    virtual ~IFormatImporter(){};

    virtual bool parse(const QString &filename, Callback callback, QUstFile *out) = 0;
};

#define IFormatImporter_IID "QSynthesis.Plugin.ImportFormats"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IFormatImporter, IFormatImporter_IID)
QT_END_NAMESPACE

#endif // IFORMATIMPORTER_H
