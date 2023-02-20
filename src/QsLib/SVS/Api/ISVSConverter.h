#ifndef ISVSCONVERTER_H
#define ISVSCONVERTER_H

#include <functional>

#include "ISVSPlugin.h"
#include "Svip/QSvipModel.h"

QSAPI_BEGIN_NAMESPACE

class QSSVS_API ISVSConverter : public ISVSPlugin {
    Q_OBJECT
public:
    explicit ISVSConverter(QObject *parent = nullptr);
    ~ISVSConverter();

    QString err;

    /**
     * @brief Convert an SVS project file to an OpenSVIP model
     *
     * @param filename Absolute path of input file
     * @param out Output model reference
     * @param args Other key-value arguments
     * @return true Success
     * @return false Failed
     */
    virtual bool load(const QString &filename, QSvipModel *out,
                      const QMap<QString, QVariant> &args = {}) = 0;

    /**
     * @brief Convert an OpenSVIP model to an SVS project file
     *
     * @param filename Absolute path of output file
     * @param in Input model
     * @param args Other key-value arguments
     * @return true Success
     * @return false Failed
     */
    virtual bool save(const QString &filename, const QSvipModel &in,
                      const QMap<QString, QVariant> &args = {}) = 0;
};

QSAPI_END_NAMESPACE

#define QsApi_ISVSConverter_IID "QSynthesis.Plugin.SVS.SVSConverter"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QsApi::ISVSConverter, QsApi_ISVSConverter_IID)
QT_END_NAMESPACE

#endif // ISVSCONVERTER_H
