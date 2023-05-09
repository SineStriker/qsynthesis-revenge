#include "g2pglobal.h"
#include <QDebug>

namespace IKg2p {

    class G2pGlobal {
    public:
        QString path;
    };

    Q_GLOBAL_STATIC(G2pGlobal, m_global)

    QString dictionaryPath() {
        return m_global->path;
    }

    void setDictionaryPath(const QString &dir) {
        m_global->path = dir;
    }

    QStringList splitString(const QString &input) {
        QStringList res;

        // negative lookahead:ッっ;letter,num,chinese,kana
        QRegExp rx("(?![ッっー゜])([a-zA-Z]+|[0-9]|[\u4e00-\u9fa5]|[\u3040-\u309F\u30A0-\u30FF]["
                   "ャュョゃゅょァィゥェォぁぃぅぇぉ]?)");

        int pos = 0; // 记录匹配位置的变量

        while ((pos = rx.indexIn(input, pos)) != -1) {
            res.append(input.mid(pos, rx.matchedLength()));
            pos += rx.matchedLength(); // 更新匹配位置
        }

        //                qDebug() << res;
        return res;
    }

}
