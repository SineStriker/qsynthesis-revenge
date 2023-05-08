#ifndef PHONEME2GRAPHEME_JPG2P_H
#define PHONEME2GRAPHEME_JPG2P_H

#include <QString>
#include <QMap>

class JpG2p {
public:
    enum KanaType {
        Hiragana, Katakana
    };

    static QString kana2romaji(const QString &kanaStr);

    static QString kana2romaji(const QStringList &kanaList);

private:
    static const QMap<QString, QString> kanaToRomajiMap;
    static const QMap<QString, QString> romajiToKanaMap;

    static QStringList convertKana(const QStringList &kanaList, KanaType kanaType);

    static QStringList splitString(const QString &input);
};

#endif //PHONEME2GRAPHEME_JPG2P_H


