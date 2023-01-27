#ifndef ICOMPRESSENGINE_H
#define ICOMPRESSENGINE_H

#include <QObject>

#include "QsGlobal.h"

class QSBASE_API ICompressEngine : public QObject {
    Q_OBJECT
public:
    ICompressEngine(QObject *parent = nullptr);
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

#define ICompressEngine_IID "QSynthesis.Plugin.Utils.CompressEngine"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ICompressEngine, ICompressEngine_IID)
QT_END_NAMESPACE

#endif // ICOMPRESSENGINE_H
