#include <QDebug>
#include <QFile>
#include <QStringList>
#include <set>
#include <QDir>
#include "zhG2p.h"


ZhG2p::ZhG2p(const QString &dict_dir) {
    loadDict(dict_dir, "dict/phrases_map.txt", phrases_map);
    loadDict(dict_dir, "dict/phrases_dict.txt", phrases_dict);
    loadDict(dict_dir, "dict/user_dict.txt", phrases_dict);
    loadDict(dict_dir, "dict/word.txt", word_dict);
    loadDict(dict_dir, "dict/trans_word.txt", trans_dict);
}

ZhG2p::~ZhG2p() = default;

void ZhG2p::loadDict(const QString &dict_dir, const QString &fileName, QHash<QString, QString> &resultMap) {
    QString file_path = QDir::cleanPath(dict_dir + QDir::separator() + fileName);
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << fileName << " error: " << file.errorString();
    }

    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        QStringList keyValuePair = line.split(":");
        if (keyValuePair.count() == 2) {
            QString key = keyValuePair[0];
            QString value = keyValuePair[1];
            resultMap[key] = value;
        }
    }
}


bool ZhG2p::isPolyphonic(const QString &text) {
    if (phrases_map.find(text) != phrases_map.end()) {
        return true;
    } else {
        return false;
    }
}


QString ZhG2p::tradToSim(const QString &text) {
    if (trans_dict.find(text) != trans_dict.end()) {
        return trans_dict[text];
    } else {
        return text;
    }
}


QString ZhG2p::getDefaultPinyin(const QString &text) {
    return word_dict[text];
}


void ZhG2p::addString(const QString &text, QStringList &res) {
    QStringList temp = text.split(" ");
    for (auto &pinyin: qAsConst(temp)) {
        res.append(pinyin);
    }
}


void ZhG2p::removeElements(QStringList &list, int start, int n) {
    list.removeAt(start); // Remove the first element to be deleted
    for (int i = 1; i < n; i++) {
        list.removeAt(start); // Remove the remaining elements
    }
}


void ZhG2p::zhPosition(const QStringList &input, QStringList &res, QList<int> &positions) {
    for (int i = 0; i < input.size(); i++) {
        if (word_dict.find(input.at(i)) != word_dict.end() || trans_dict.find(input.at(i)) != trans_dict.end()) {
            res.append(input.mid(i, 1));
            positions.append(i);
        }
    }
}


QString ZhG2p::resetZH(const QStringList &input, const QStringList &res, QList<int> &positions) {
    QStringList result = input;
    for (int i = 0; i < positions.size(); i++) {
        result.replace(positions[i], res.at(i));
    }

    return result.join(" ");
}

QStringList ZhG2p::splitString(const QString &input) {
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

QString ZhG2p::convert(const QStringList &input) {
//    qDebug() << input;
    QStringList inputList;
    QList<int> inputPos;
    //get char&pos in dict
    zhPosition(input, inputList, inputPos);
    QStringList result;
    int cursor = 0;
    while (cursor < inputList.size()) {
        //get char
        const QString &raw_current_char = inputList.at(cursor);
        QString current_char;
        current_char = tradToSim(raw_current_char);
//        qDebug() << raw_current_char << raw_current_char;
        //not in dict
        if (word_dict.find(current_char) == word_dict.end()) {
            result.append(current_char);
            cursor++;
            continue;
        }

//        qDebug() << current_char << isPolyphonic(current_char);
        //is polypropylene
        if (!isPolyphonic(current_char)) {
            result.append(getDefaultPinyin(current_char));
            cursor++;
        } else {
            bool found = false;
            for (int length = 4; length >= 2 && !found; length--) {
                //forward lookup
                QString sub_phrase = inputList.mid(cursor, length).join("");
                if (phrases_dict.find(sub_phrase) != phrases_dict.end()) {
//                    qDebug() << "get:sub_phrase:" << sub_phrase << phrases_dict[sub_phrase];
                    addString(phrases_dict[sub_phrase], result);
                    cursor += length;
                    found = true;
                } else if ((cursor + 1) >= length) {
                    //reverse lookup
                    QString x_sub_phrase = inputList.mid(cursor + 1 - length, length).join("");
                    if (phrases_dict.find(x_sub_phrase) != phrases_dict.end()) {
//                        qDebug() << "get:x_sub_phrase:" << x_sub_phrase << phrases_dict[x_sub_phrase];
                        int pos = x_sub_phrase.indexOf(current_char);
                        //overwrite pinyin
                        removeElements(result, cursor + 1 - length, pos);
                        addString(phrases_dict[x_sub_phrase], result);
                        cursor += 1;
                        found = true;
                    }
                }
            }
            //not found, use default pinyin
            if (!found) {
                result.append(getDefaultPinyin(current_char));
                cursor++;
            }
        }
    }

    return resetZH(input, result, inputPos);
}

QString ZhG2p::convert(const QString &raw) {
    QStringList input = splitString(raw);
    return convert(input);
}


