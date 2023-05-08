#include "zhg2p.h"
#include "zhg2p_p.h"

#include <QDebug>
#include <QDir>
#include <QFile>

#include "g2pglobal.h"

namespace IKg2p {

    static QString resetZH(const QStringList &input, const QStringList &res, QList<int> &positions) {
        QStringList result = input;
        for (int i = 0; i < positions.size(); i++) {
            result.replace(positions[i], res.at(i));
        }

        return result.join(" ");
    }

    static void addString(const QString &text, QStringList &res) {
        QStringList temp = text.split(" ");
        for (auto &pinyin : qAsConst(temp)) {
            res.append(pinyin);
        }
    }

    static inline void removeElements(QStringList &list, int start, int n) {
        //        list.removeAt(start);     // Remove the first element to be deleted
        //        for (int i = 1; i < n; i++) {
        //            list.removeAt(start); // Remove the remaining elements
        //        }
        list.erase(list.begin() + start, list.begin() + start + n);
    }

    ZhG2pPrivate::ZhG2pPrivate() {
    }

    ZhG2pPrivate::~ZhG2pPrivate() {
    }

    void ZhG2pPrivate::init() {
        auto dict_dir = dictionaryPath();
        loadDict(dict_dir, "phrases_map.txt", phrases_map);
        loadDict(dict_dir, "phrases_dict.txt", phrases_dict);
        loadDict(dict_dir, "user_dict.txt", phrases_dict);
        loadDict(dict_dir, "word.txt", word_dict);
        loadDict(dict_dir, "trans_word.txt", trans_dict);
    }

    bool ZhG2pPrivate::isPolyphonic(const QString &text) const {
        return phrases_map.contains(text);
    }

    QString ZhG2pPrivate::tradToSim(const QString &text) const {
        return trans_dict.value(text, text);
    }

    QString ZhG2pPrivate::getDefaultPinyin(const QString &text) const {
        return word_dict.value(text, {});
    }

    void ZhG2pPrivate::zhPosition(const QStringList &input, QStringList &res, QList<int> &positions) const {
        for (int i = 0; i < input.size(); i++) {
            if (word_dict.find(input.at(i)) != word_dict.end() || trans_dict.find(input.at(i)) != trans_dict.end()) {
                res.append(input.mid(i, 1));
                positions.append(i);
            }
        }
    }

    bool ZhG2pPrivate::loadDict(const QString &dict_dir, const QString &fileName, QHash<QString, QString> &resultMap) {
        QString file_path = QDir::cleanPath(dict_dir + "/" + fileName);
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << fileName << " error: " << file.errorString();
            return false;
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
        return true;
    }

    ZhG2p::ZhG2p(QObject *parent) : ZhG2p(*new ZhG2pPrivate(), parent) {
    }

    ZhG2p::~ZhG2p() {
    }

    QString ZhG2p::convert(const QString &input) {
        return convert(splitString(input));
    }

    QString ZhG2p::convert(const QStringList &input) {
        Q_D(const ZhG2p);
        //    qDebug() << input;
        QStringList inputList;
        QList<int> inputPos;
        // get char&pos in dict
        d->zhPosition(input, inputList, inputPos);
        QStringList result;
        int cursor = 0;
        while (cursor < inputList.size()) {
            // get char
            const QString &raw_current_char = inputList.at(cursor);
            QString current_char;
            current_char = d->tradToSim(raw_current_char);
            //        qDebug() << raw_current_char << raw_current_char;
            // not in dict
            if (d->word_dict.find(current_char) == d->word_dict.end()) {
                result.append(current_char);
                cursor++;
                continue;
            }

            //        qDebug() << current_char << isPolyphonic(current_char);
            // is polypropylene
            if (!d->isPolyphonic(current_char)) {
                result.append(d->getDefaultPinyin(current_char));
                cursor++;
            } else {
                bool found = false;
                for (int length = 4; length >= 2 && !found; length--) {
                    // forward lookup
                    QString sub_phrase = inputList.mid(cursor, length).join("");
                    if (d->phrases_dict.find(sub_phrase) != d->phrases_dict.end()) {
                        //                    qDebug() << "get:sub_phrase:" << sub_phrase << phrases_dict[sub_phrase];
                        addString(d->phrases_dict[sub_phrase], result);
                        cursor += length;
                        found = true;
                    } else if ((cursor + 1) >= length) {
                        // reverse lookup
                        QString x_sub_phrase = inputList.mid(cursor + 1 - length, length).join("");
                        if (d->phrases_dict.find(x_sub_phrase) != d->phrases_dict.end()) {
                            //                        qDebug() << "get:x_sub_phrase:" << x_sub_phrase <<
                            //                        phrases_dict[x_sub_phrase];
                            int pos = x_sub_phrase.indexOf(current_char);
                            // overwrite pinyin
                            removeElements(result, cursor + 1 - length, pos);
                            addString(d->phrases_dict[x_sub_phrase], result);
                            cursor += 1;
                            found = true;
                        }
                    }
                }
                // not found, use default pinyin
                if (!found) {
                    result.append(d->getDefaultPinyin(current_char));
                    cursor++;
                }
            }
        }

        return resetZH(input, result, inputPos);
    }

    ZhG2p::ZhG2p(ZhG2pPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }

}
