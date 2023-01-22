#include "QZlibEngine.h"

#include <JlCompress.h>

QZlibEngine::QZlibEngine(QObject *parent) : ICompressEngine(parent) {
}

QZlibEngine::~QZlibEngine() {
}

bool QZlibEngine::createFromDirectory(const QString &dir, const QString &filename) {
    return JlCompress::compressDir(filename, dir);
}

bool QZlibEngine::extractToDirectory(const QString &filename, const QString &dir) {
    auto files = JlCompress::extractDir(filename, dir);
    if (files.isEmpty()) {
        return false;
    }
    return true;
}
