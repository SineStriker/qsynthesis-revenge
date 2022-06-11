#ifndef TEXTHELPER_H
#define TEXTHELPER_H

#include <QByteArray>
#include <QTextCodec>

namespace Txt {

    QTextCodec *GetUtfCodec(const QByteArray &data, bool *determined = nullptr);

}

#endif // TEXTHELPER_H
