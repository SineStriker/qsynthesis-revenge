#ifndef QZLIBENGINE_H
#define QZLIBENGINE_H

#include "Api/ICompressEngine.h"

class QZlibEngine : public ICompressEngine {
    Q_OBJECT
    Q_INTERFACES(ICompressEngine)
    Q_PLUGIN_METADATA(IID ICompressEngine_IID FILE "plugin.json")
public:
    explicit QZlibEngine(QObject *parent = nullptr);
    ~QZlibEngine();

public:
    bool createFromDirectory(const QString &dir, const QString &filename) override;
    bool extractToDirectory(const QString &filename, const QString &dir) override;

signals:
};

#endif // QZLIBENGINE_H
