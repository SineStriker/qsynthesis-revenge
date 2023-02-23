#ifndef QMCODEC_H
#define QMCODEC_H

#include <QByteArray>
#include <QTextCodec>

#include "QMGlobal.h"

namespace QMCodec {

    /**
     * @brief Get the Utf Codec of data (Deprecated)
     *
     * @param data bytes
     * @param determined If the target codec is determined
     * @return
     */
    QMCORELIB_API QTextCodec *GetUtfCodec(const QByteArray &data, bool *determined = nullptr);

    /**
     * @brief Convert escaped string in C++ to literal
     *
     * @param s string
     */
    QMCORELIB_API QString unescape(const QString &s);

    QMCORELIB_API QByteArray toNativePath(const QByteArray &s);

    QMCORELIB_API QByteArray fromNativePath(const QByteArray &s);

}

#endif // QMCODEC_H
