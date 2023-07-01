#ifndef DSPXENTITYUTILS_P_H
#define DSPXENTITYUTILS_P_H

#include <QMetaEnum>
#include <QString>

#include "AceTreeEntity.h"

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

    template <class T>
    inline bool compareElementX(const AceTreeEntity *left, const AceTreeEntity *right) {
        return static_cast<const T *>(left)->x() < static_cast<const T *>(right)->x();
    }

    template <class T>
    inline bool compareElementPos(const AceTreeEntity *left, const AceTreeEntity *right) {
        return static_cast<const T *>(left)->position() < static_cast<const T *>(right)->position();
    }

    template <class T>
    inline bool compareElementStart(const AceTreeEntity *left, const AceTreeEntity *right) {
        return static_cast<const T *>(left)->start() < static_cast<const T *>(right)->start();
    }

}

#endif // DSPXENTITYUTILS_P_H
