#ifndef CHORUSKIT_DSPXENTITYUTILS_P_H
#define CHORUSKIT_DSPXENTITYUTILS_P_H

#include <QMetaEnum>
#include <QString>

namespace Core {

    template <class T>
    T stringToEnum(QString key) {
        key[0] = key[0].toUpper();
        auto me = QMetaEnum::fromType<T>();
        auto val = me.keyToValue(key.toLatin1());
        if (val < 0)
            return static_cast<T>(me.value(0));
        return static_cast<T>(val);
    }

    template <class T>
    T variantToEnum(const QVariant &v) {
        return stringToEnum<T>(v.toString());
    }

    template <class T>
    QString enumToString(T value) {
        auto key = QString::fromLatin1(QMetaEnum::fromType<T>().valueToKey(value));
        key[0] = key[0].toLower();
        return key;
    }

    template <class T = int>
    inline bool compareElementPos(const AceTreeEntity *left, const AceTreeEntity *right) {
        return left->property("pos").value<T>() < right->property("pos").value<T>();
    }

    template <class T = int>
    inline bool compareElementStart(const AceTreeEntity *left, const AceTreeEntity *right) {
        return left->property("start").value<T>() < right->property("start").value<T>();
    }

    template <class T = int>
    inline bool compareElementX(const AceTreeEntity *left, const AceTreeEntity *right) {
        return left->property("x").value<T>() < right->property("x").value<T>();
    }

}

#endif // CHORUSKIT_DSPXENTITYUTILS_P_H
