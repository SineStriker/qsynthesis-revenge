#include "QZlibCompressEngine.h"

#include <JlCompress.h>

QZlibCompressEngine::QZlibCompressEngine(QObject *parent) : ICompressEngine(parent) {
}

QZlibCompressEngine::~QZlibCompressEngine() {
}

bool QZlibCompressEngine::createFromDirectory(const QString &dir, const QString &filename) {
    return JlCompress::compressDir(filename, dir);
}

bool QZlibCompressEngine::extractToDirectory(const QString &filename, const QString &dir) {
    auto files = JlCompress::extractDir(filename, dir);
    if (files.isEmpty()) {
        return false;
    }
    return true;
}
