#ifndef QSTEXTHELPER_H
#define QSTEXTHELPER_H

#include <QByteArray>
#include <QTextCodec>

#include "QsGlobal.h"

namespace QsCodec {

    /**
     * @brief Get the Utf Codec of data (Deprecated)
     *
     * @param data bytes
     * @param determined If the target codec is determined
     * @return
     */
    QSBASE_API QTextCodec *GetUtfCodec(const QByteArray &data, bool *determined = nullptr);

    /**
     * @brief Convert escaped string in C++ to literal
     *
     * @param s string
     */
    QSBASE_API QString unescape(const QString &s);

    QSBASE_API QByteArray toNativePath(const QByteArray &s);

    QSBASE_API QByteArray fromNativePath(const QByteArray &s);

} // namespace QsCodec

#endif // QSTEXTHELPER_H
