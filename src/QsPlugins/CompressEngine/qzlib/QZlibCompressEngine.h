#ifndef QZLIBCOMPRESSENGINE_H
#define QZLIBCOMPRESSENGINE_H

#include "Api/ICompressEngine.h"

class QZlibCompressEngine : public ICompressEngine {
    Q_OBJECT
    Q_INTERFACES(ICompressEngine)
    Q_PLUGIN_METADATA(IID ICompressEngine_IID FILE "plugin.json")
public:
    explicit QZlibCompressEngine(QObject *parent = nullptr);
    ~QZlibCompressEngine();

public:
    bool createFromDirectory(const QString &dir, const QString &filename) override;
    bool extractToDirectory(const QString &filename, const QString &dir) override;

signals:
};

#endif // QZLIBCOMPRESSENGINE_H
