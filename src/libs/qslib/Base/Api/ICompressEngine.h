#ifndef ICOMPRESSENGINE_H
#define ICOMPRESSENGINE_H

#include "QMNamePlugin.h"
#include "QsGlobal.h"

QSAPI_BEGIN_NAMESPACE

class QSBASE_API ICompressEngine : public QMNamePlugin {
    Q_OBJECT
public:
    explicit ICompressEngine(QObject *parent = nullptr);
    ~ICompressEngine();

    /**
     * @brief Create a ZIP file from existing irectory
     *
     * @param dir Directory path
     * @param filename Target ZIP file path
     * @return true Success
     * @return false Failed
     */
    virtual bool createFromDirectory(const QString &dir, const QString &filename) = 0;

    /**
     * @brief Extract a ZIP file to a directory
     *
     * @param filename Target directory path
     * @param dir ZIP file path
     * @return true Success
     * @return false Failed
     */
    virtual bool extractToDirectory(const QString &filename, const QString &dir) = 0;
};

QSAPI_END_NAMESPACE

#define QsApi_ICompressEngine_IID "org.ChorusKit.QsLib.ICompressEngine"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QsApi::ICompressEngine, QsApi_ICompressEngine_IID)
QT_END_NAMESPACE

#endif // ICOMPRESSENGINE_H
