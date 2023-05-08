#ifndef JPG2PPRIVATE_H
#define JPG2PPRIVATE_H

#include <QHash>

#include "jpg2p_p.h"

namespace IKg2p {

    class JpG2pPrivate {
        Q_DECLARE_PUBLIC(JpG2p)
    public:
        JpG2pPrivate();
        virtual ~JpG2pPrivate();

        void init();

        JpG2p *q_ptr;

        QHash<QString, QString> kanaToRomajiMap;
        QHash<QString, QString> romajiToKanaMap;

        enum KanaType { Hiragana, Katakana };

        QStringList convertKana(const QStringList &kanaList, KanaType kanaType) const;

        QString kana2romaji(const QStringList &kanaList) const;
    };
}

#endif // JPG2PPRIVATE_H