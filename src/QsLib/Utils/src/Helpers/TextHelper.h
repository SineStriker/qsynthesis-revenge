#ifndef TEXTHELPER_H
#define TEXTHELPER_H

#include <QByteArray>
#include <QTextCodec>

#include "qsutils_global.h"

namespace Txt {

    QSUTILS_API QTextCodec *GetUtfCodec(const QByteArray &data, bool *determined = nullptr);

}

#endif // TEXTHELPER_H
