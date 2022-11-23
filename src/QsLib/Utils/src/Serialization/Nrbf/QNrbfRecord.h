#ifndef __QNRBFRECORD_H__
#define __QNRBFRECORD_H__

#include "QNrbfVariants.h"

class QSUTILS_API QNrbfRecord {
public:
    using Type = QNrbf::RecordTypeEnumeration;

    QNrbfRecord();
    ~QNrbfRecord();

    template <class T>
    QNrbfRecord(const T &data) {
        setValue<T>(data);
    }

    template <class T>
    QNrbfRecord(T &&data) {
        setValue<T>(data);
    }

    Type type() const;
    bool isNull() const;

    template <class T>
    T value() const {
        if (isNull())
            return T{};
        return std::any_cast<T>(_data);
    }

    template <class T>
    void setValue(const T &data) {
        _data = data;
        updateType();
    }

    template <class T>
    void setValue(T &&data) {
        _data = data;
        updateType();
    }

protected:
    Type _type;
    std::any _data;

    void updateType();
};


#endif // __QNRBFRECORD_H__
