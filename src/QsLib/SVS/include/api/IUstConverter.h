#ifndef IUSTCONVERTER_H
#define IUSTCONVERTER_H

#include <QStringList>

#include <functional>

#include "Utau/QUstFile.h"

#include "qssvs_global.h"

class Q_DECL_EXPORT IUstConverter {
public:
    struct SelectArguments {
        QString title;
        QString caption;
        QStringList options;
        bool single;
    };

    typedef std::function<QList<bool>(SelectArguments)> Callback;

    virtual ~IUstConverter(){};

    virtual bool load(const QString &filename, QUstFile *out, Callback callback) = 0;
    virtual bool save(const QString &filename, const QUstFile &in) = 0;
};

#define IUstConverter_IID "QSynthesis.Plugin.UstConverter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IUstConverter, IUstConverter_IID)
QT_END_NAMESPACE

#endif // IUSTCONVERTER_H
