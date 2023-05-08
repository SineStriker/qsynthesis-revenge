#include "g2pglobal.h"

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
        int len = input.length();
        QString word = "";
        for (int i = 0; i < len; i++) {
            QChar ch = input.at(i);
            if (ch.unicode() < 128 && ch.isLetter()) {
                // joint english word
                word += ch;
            } else {
                // add num/chinese
                if (word.length() > 0) {
                    res.append(word);
                    word.clear();
                }
                if (ch != " ") {
                    res.append(ch);
                }
            }
        }
        // add last word
        if (word.length() > 0) {
            res.append(word);
        }
        return res;
    }

}
