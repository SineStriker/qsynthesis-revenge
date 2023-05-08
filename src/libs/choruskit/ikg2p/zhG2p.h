#ifndef PHONEME2GRAPHEME_ZHG2P_H
#define PHONEME2GRAPHEME_ZHG2P_H

#include <QString>
#include <QMap>

class ZhG2p {
public:
    explicit ZhG2p(const QString &dict_path);
    ~ZhG2p();

    QString convert(const QString &input);

    QString convert(const QStringList &input);

private:
    QHash<QString, QString> phrases_map;
    QHash<QString, QString> phrases_dict;
    QHash<QString, QString> word_dict;
    QHash<QString, QString> trans_dict;

    static void loadDict(const QString &dict_dir, const QString &fileName, QHash<QString, QString> &resultMap);

    bool isPolyphonic(const QString &text);

    QString tradToSim(const QString &text);

    QString getDefaultPinyin(const QString &text);

    static void addString(const QString &text, QStringList &res);

    static void removeElements(QStringList &list, int start, int n);

    void zhPosition(const QStringList &input, QStringList &res, QList<int> &positions);

    static QString resetZH(const QStringList &input, const QStringList &res, QList<int> &positions);

    static QStringList splitString(const QString &input);

};

#endif //PHONEME2GRAPHEME_ZHG2P_H


