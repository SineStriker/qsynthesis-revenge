#ifndef __QNRBFRECORD_H__
#define __QNRBFRECORD_H__

#include <QStringList>

#include "QNrbfVariants.h"

#include "qsutils_global.h"

class QSUTILS_API QNrbfRecord {
public:
    QNrbfRecord();
    ~QNrbfRecord();

    template <class T>
    QNrbfRecord(const T &data) {
        setValue<T>(data);
    }

    QNrbf::RecordTypeEnumeration type() const;
    inline bool isNull() const {
        return _type == QNrbf::RecordTypeEnumeration::SerializedStreamHeader;
    }

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

protected:
    QNrbf::RecordTypeEnumeration _type;
    std::any _data;

    void updateType();
};


#endif // __QNRBFRECORD_H__
