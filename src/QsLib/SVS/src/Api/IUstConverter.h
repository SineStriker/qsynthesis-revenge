#ifndef IUSTCONVERTER_H
#define IUSTCONVERTER_H

#include <QObject>
#include <QStringList>

#include <functional>

#include "Utau/QUstFile.h"

#include "qssvs_global.h"

class QSSVS_API IUstConverter : public QObject {
    Q_OBJECT
public:
    IUstConverter(QObject *parent = nullptr);
    ~IUstConverter();

    struct SelectArguments {
        QString title;
        QString caption;
        QStringList options;
        bool single;
    };

    typedef std::function<QList<bool>(SelectArguments)> Callback;

    /**
     * @brief load: Load another SVS porject file and convert to ust file
     * @param filename: Absolute path of input file
     * @param out: Output instance
     * @param callback: A selection callback if there's many tracks
     * @return true if success, false if failed
     */
    virtual bool load(const QString &filename, QUstFile *out, Callback callback) = 0;

    /**
     * @brief save: Convert ust to another SVS project and store to file system
     * @param filename: Absolute path of output file
     * @param in: Input instance
     * @return true if success, false if failed
     */
    virtual bool save(const QString &filename, const QUstFile &in) = 0;
};

#define IUstConverter_IID "QSynthesis.Plugin.SVS.UstConverter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IUstConverter, IUstConverter_IID)
QT_END_NAMESPACE

#endif // IUSTCONVERTER_H
