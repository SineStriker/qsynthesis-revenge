#include "jpg2p.h"
#include "jpg2p_p.h"

#include "zhg2p_p.h"

#include "g2pglobal.h"

namespace IKg2p {

    JpG2pPrivate::JpG2pPrivate() {
    }

    JpG2pPrivate::~JpG2pPrivate() {
    }

    void JpG2pPrivate::init() {
        auto dict_dir = dictionaryPath();
        ZhG2pPrivate::loadDict(dict_dir, "kana2romaji.txt", kanaToRomajiMap);

        for (auto it = kanaToRomajiMap.begin(); it != kanaToRomajiMap.end(); ++it) {
            romajiToKanaMap.insert(it.value(), it.key());
        }
    }

    QStringList JpG2pPrivate::convertKana(const QStringList &kanaList, KanaType kanaType) const {
        const ushort hiraganaStart = 0x3041;
        const ushort katakanaStart = 0x30A1;

        QStringList convertedList;
        for (const QString &kana : kanaList) {
            QString convertedKana;
            if (kana.size() == 1) {
                QChar kanaChar = kana.at(0);
                if (kanaType == KanaType::Katakana) {
                    // target is Katakana
                    if (kanaChar >= katakanaStart && kanaChar < katakanaStart + 0x5E) {
                        // Hiragana->Katakana
                        convertedKana = QChar(kanaChar.unicode() - katakanaStart + hiraganaStart);
                    } else {
                        convertedKana = kanaChar;
                    }
                } else {
                    // target is Hiragana
                    if (kanaChar >= hiraganaStart && kanaChar < hiraganaStart + 0x5E) {
                        // Katakana->Hiragana
                        convertedKana = QChar(kanaChar.unicode() + katakanaStart - hiraganaStart);
                    } else {
                        convertedKana = kanaChar;
                    }
                }
            } else {
                convertedKana = kana;
            }
            convertedList.append(convertedKana);
        }
        return convertedList;
    }

    QString JpG2pPrivate::kana2romaji(const QStringList &kanaList) const {
        QStringList inputList = convertKana(kanaList, KanaType::Katakana);
        QStringList romajiList;
        for (const QString &kana : inputList) {
            QString romaji;
            for (int i = 0; i < kana.length(); ++i) {
                QString kanaChar = kana.mid(i, 1);
                QString romajiChar = kanaToRomajiMap.value(kanaChar);
                if (romajiChar.isEmpty()) {
                    romaji.append(kanaChar);
                } else {
                    romaji.append(romajiChar);
                }
            }
            romajiList.append(romaji);
        }
        return romajiList.join(" ");
    }

    JpG2p::JpG2p(QObject *parent) : JpG2p(*new JpG2pPrivate(), parent) {
    }

    JpG2p::~JpG2p() {
    }

    QString JpG2p::kana2romaji(const QStringList &kanaList) {
        Q_D(const JpG2p);
        QStringList inputList = d->convertKana(kanaList, JpG2pPrivate::KanaType::Katakana);
        QStringList romajiList;
        for (const QString &kana : inputList) {
            QString romaji;
            for (int i = 0; i < kana.length(); ++i) {
                QString kanaChar = kana.mid(i, 1);
                QString romajiChar = d->kanaToRomajiMap.value(kanaChar, {});
                if (romajiChar.isEmpty()) {
                    romaji.append(kanaChar);
                } else {
                    romaji.append(romajiChar);
                }
            }
            romajiList.append(romaji);
        }
        return romajiList.join(" ");
    }

    QString JpG2p::kana2romaji(const QString &kanaStr) {
        QStringList input = splitString(kanaStr);
        return kana2romaji(input);
    }

    JpG2p::JpG2p(JpG2pPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
}
