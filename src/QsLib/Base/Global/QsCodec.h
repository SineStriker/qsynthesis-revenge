#ifndef QSTEXTHELPER_H
#define QSTEXTHELPER_H

#include <QByteArray>
#include <QTextCodec>

#include "QsGlobal.h"

namespace QsCodec {

    QSBASE_API QTextCodec *GetUtfCodec(const QByteArray &data, bool *determined = nullptr);

}

#endif // QSTEXTHELPER_H
