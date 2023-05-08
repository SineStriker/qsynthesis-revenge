#include "jpG2p.h"
#include <QDebug>
#include <QStringList>

QStringList JpG2p::splitString(const QString &input) {
    QStringList res;
    int len = input.length();
    QString word = "";
    for (int i = 0; i < len; i++) {
        QChar ch = input.at(i);
        if (ch.category() == QChar::Letter_Lowercase || ch.category() == QChar::Letter_Uppercase) {
            // joint english word
            word += ch;
        } else {
            // add num/chinese
            if (word.length() > 0) {
                res.append(word);
                word = "";
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


QStringList JpG2p::convertKana(const QStringList &kanaList, KanaType kanaType) {
    const ushort hiraganaStart = 0x3041;
    const ushort katakanaStart = 0x30A1;

    QStringList convertedList;
    for (const QString &kana: kanaList) {
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

QString JpG2p::kana2romaji(const QStringList &kanaList) {
    QStringList inputList = convertKana(kanaList, KanaType::Katakana);
    QStringList romajiList;
    for (const QString &kana: inputList) {
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

QString JpG2p::kana2romaji(const QString &kanaStr) {
    QStringList input = splitString(kanaStr);
    return kana2romaji(input);
}

const QMap<QString, QString> JpG2p::kanaToRomajiMap = {
        {"あ",  "a"},
        {"い",  "i"},
        {"う",  "u"},
        {"え",  "e"},
        {"お",  "o"},
        {"か",  "ka"},
        {"き",  "ki"},
        {"く",  "ku"},
        {"け",  "ke"},
        {"こ",  "ko"},
        {"が",  "ga"},
        {"ぎ",  "gi"},
        {"ぐ",  "gu"},
        {"げ",  "ge"},
        {"ご",  "go"},
        {"さ",  "sa"},
        {"し",  "shi"},
        {"す",  "su"},
        {"せ",  "se"},
        {"そ",  "so"},
        {"ざ",  "za"},
        {"じ",  "ji"},
        {"ず",  "zu"},
        {"ぜ",  "ze"},
        {"ぞ",  "zo"},
        {"た",  "ta"},
        {"ち",  "chi"},
        {"つ",  "tsu"},
        {"て",  "te"},
        {"と",  "to"},
        {"だ",  "da"},
        {"ぢ",  "ji"},
        {"づ",  "zu"},
        {"で",  "de"},
        {"ど",  "do"},
        {"な",  "na"},
        {"に",  "ni"},
        {"ぬ",  "nu"},
        {"ね",  "ne"},
        {"の",  "no"},
        {"は",  "ha"},
        {"ひ",  "hi"},
        {"ふ",  "fu"},
        {"へ",  "he"},
        {"ほ",  "ho"},
        {"ば",  "ba"},
        {"び",  "bi"},
        {"ぶ",  "bu"},
        {"べ",  "be"},
        {"ぼ",  "bo"},
        {"ぱ",  "pa"},
        {"ぴ",  "pi"},
        {"ぷ",  "pu"},
        {"ぺ",  "pe"},
        {"ぽ",  "po"},
        {"ま",  "ma"},
        {"み",  "mi"},
        {"む",  "mu"},
        {"め",  "me"},
        {"も",  "mo"},
        {"や",  "ya"},
        {"ゆ",  "yu"},
        {"よ",  "yo"},
        {"ら",  "ra"},
        {"り",  "ri"},
        {"る",  "ru"},
        {"れ",  "re"},
        {"ろ",  "ro"},
        {"わ",  "wa"},
        {"を",  "wo"},
        {"ん",  "N"},
        {"きゃ", "kya"},
        {"きゅ", "kyu"},
        {"きょ", "kyo"},
        {"しゃ", "sha"},
        {"しゅ", "shu"},
        {"しょ", "sho"},
        {"ちゃ", "cha"},
        {"ちゅ", "chu"},
        {"ちょ", "cho"},
        {"にゃ", "nya"},
        {"にゅ", "nyu"},
        {"にょ", "nyo"},
        {"ひゃ", "hya"},
        {"ひゅ", "hyu"},
        {"ひょ", "hyo"},
        {"みゃ", "mya"},
        {"みゅ", "myu"},
        {"みょ", "myo"},
        {"りゃ", "rya"},
        {"りゅ", "ryu"},
        {"りょ", "ryo"}
};

const QMap<QString, QString> JpG2p::romajiToKanaMap = {
        {"a",   "あ"},
        {"i",   "い"},
        {"u",   "う"},
        {"e",   "え"},
        {"o",   "お"},
        {"ka",  "か"},
        {"ki",  "き"},
        {"ku",  "く"},
        {"ke",  "け"},
        {"ko",  "こ"},
        {"ga",  "が"},
        {"gi",  "ぎ"},
        {"gu",  "ぐ"},
        {"ge",  "げ"},
        {"go",  "ご"},
        {"sa",  "さ"},
        {"shi", "し"},
        {"su",  "す"},
        {"se",  "せ"},
        {"so",  "そ"},
        {"za",  "ざ"},
        {"ji",  "じ"},
        {"zu",  "ず"},
        {"ze",  "ぜ"},
        {"zo",  "ぞ"},
        {"ta",  "た"},
        {"chi", "ち"},
        {"tsu", "つ"},
        {"te",  "て"},
        {"to",  "と"},
        {"da",  "だ"},
        {"di",  "ぢ"},
        {"du",  "づ"},
        {"de",  "で"},
        {"do",  "ど"},
        {"na",  "な"},
        {"ni",  "に"},
        {"nu",  "ぬ"},
        {"ne",  "ね"},
        {"no",  "の"},
        {"ha",  "は"},
        {"hi",  "ひ"},
        {"fu",  "ふ"},
        {"he",  "へ"},
        {"ho",  "ほ"},
        {"ba",  "ば"},
        {"bi",  "び"},
        {"bu",  "ぶ"},
        {"be",  "べ"},
        {"bo",  "ぼ"},
        {"pa",  "ぱ"},
        {"pi",  "ぴ"},
        {"pu",  "ぷ"},
        {"pe",  "ぺ"},
        {"po",  "ぽ"},
        {"ma",  "ま"},
        {"mi",  "み"},
        {"mu",  "む"},
        {"me",  "め"},
        {"mo",  "も"},
        {"ya",  "や"},
        {"yu",  "ゆ"},
        {"yo",  "よ"},
        {"ra",  "ら"},
        {"ri",  "り"},
        {"ru",  "る"},
        {"re",  "れ"},
        {"ro",  "ろ"},
        {"wa",  "わ"},
        {"wo",  "を"},
        {"N",   "ん"},
        {"kya", "きゃ"},
        {"kyu", "きゅ"},
        {"kyo", "きょ"},
        {"sha", "しゃ"},
        {"shu", "しゅ"},
        {"sho", "しょ"},
        {"cha", "ちゃ"},
        {"chu", "ちゅ"},
        {"cho", "ちょ"},
        {"nya", "にゃ"},
        {"nyu", "にゅ"},
        {"nyo", "にょ"},
        {"hya", "ひゃ"},
        {"hyu", "ひゅ"},
        {"hyo", "ひょ"},
        {"mya", "みゃ"},
        {"myu", "みゅ"},
        {"myo", "みょ"},
        {"rya", "りゃ"},
        {"ryu", "りゅ"},
        {"ryo", "りょ"}
};


