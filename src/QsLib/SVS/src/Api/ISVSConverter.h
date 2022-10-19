#ifndef ISVSCONVERTER_H
#define ISVSCONVERTER_H

#include "ISVSPlugin.h"

#include <functional>

#include "Svip/QSvipFile.h"

#include "qssvs_global.h"

class QSSVS_API ISVSConverter : public ISVSPlugin {
    Q_OBJECT
public:
    ISVSConverter(QObject *parent = nullptr);
    ~ISVSConverter();

    /**
     * @brief load: Load another SVS porject file and convert to ust file
     * @param filename: Absolute path of input file
     * @param out: Output instance
     * @param callback: A selection callback if there's many tracks
     * @return true if success, false if failed
     */
    virtual bool load(const QString &filename, QSvipFile *out) = 0;

    /**
     * @brief save: Convert ust to another SVS project and store to file system
     * @param filename: Absolute path of output file
     * @param in: Input instance
     * @return true if success, false if failed
     */
    virtual bool save(const QString &filename, const QSvipFile &in) = 0;
};

#define ISVSConverter_IID "QSynthesis.Plugin.SVS.SVSConverter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ISVSConverter, ISVSConverter_IID)
QT_END_NAMESPACE

#endif // ISVSCONVERTER_H
