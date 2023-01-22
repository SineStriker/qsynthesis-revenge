#ifndef ICOMPRESSENGINE_H
#define ICOMPRESSENGINE_H

#include <QObject>

#include "QsGlobal.h"

class QSBASE_API ICompressEngine : public QObject {
    Q_OBJECT
public:
    ICompressEngine(QObject *parent = nullptr);
    ~ICompressEngine();

    virtual bool createFromDirectory(const QString &dir, const QString &filename) = 0;
    virtual bool extractToDirectory(const QString &filename, const QString &dir) = 0;
};

#define ICompressEngine_IID "QSynthesis.Plugin.Utils.CompressEngine"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ICompressEngine, ICompressEngine_IID)
QT_END_NAMESPACE

#endif // ICOMPRESSENGINE_H
