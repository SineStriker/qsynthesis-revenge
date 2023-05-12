#include "cleanlyric.h"
#include "g2pglobal.h"
#include <QDebug>

namespace IKg2p {
    QStringList CleanLyric::splitLyricAsWord(const QString &rawStr) {
        QStringList res;

        // negative lookahead:ッっ;letter,num,chinese,kana
        QRegExp rx("(?![ッっー゜])([a-zA-Z]+|[0-9]|[\u4e00-\u9fa5]|[\u3040-\u309F\u30A0-\u30FF]["
                   "ャュョゃゅょァィゥェォぁぃぅぇぉ]?|(\n|\r\n))");

        int pos = 0; // 记录匹配位置的变量

        while ((pos = rx.indexIn(rawStr, pos)) != -1) {
            res.append(rawStr.mid(pos, rx.matchedLength()));
            pos += rx.matchedLength(); // 更新匹配位置
        }

        //                qDebug() << res;
        return res;
    }

    QStringList CleanLyric::splitLyricAsChar(const QString &rawStr) {
        QStringList res;

        // negative lookahead:ッっ;letter,num,chinese,kana
        QRegExp rx("(?![ッっー゜])([a-zA-Z]|[0-9]|[\u4e00-\u9fa5]|[\u3040-\u309F\u30A0-\u30FF]["
                   "ャュョゃゅょァィゥェォぁぃぅぇぉ]?)");

        int pos = 0; // 记录匹配位置的变量

        while ((pos = rx.indexIn(rawStr, pos)) != -1) {
            res.append(rawStr.mid(pos, rx.matchedLength()));
            pos += rx.matchedLength(); // 更新匹配位置
        }

        //                qDebug() << res;
        return res;
    }
    QString CleanLyric::filterTextInParentheses(const QString &rawStr) {
        auto res = rawStr;
        QRegExp rx("[{【\\[（\\(].*[\\)）\\]】}]");
        rx.setMinimal(true);
        return res.remove(rx);
    }
}