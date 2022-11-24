#ifndef QNRBFREADERCOMMONPRIVATE_H
#define QNRBFREADERCOMMONPRIVATE_H

#include "../QNrbfRecord.h"

class QDataStream;

namespace QNrbfPrivate {

    class CommonReader {
    public:
        static bool readLengthPrefix(quint32 &size, QDataStream &in);

        static bool readString(QString &out, QDataStream &in);

        static bool readArrayInfo(QNrbf::ArrayInfo &out, QDataStream &in);

        static bool readClassInfo(QNrbf::ClassInfo &out, QDataStream &in);

        static bool readClassTypeInfo(QNrbf::ClassTypeInfo &out, QDataStream &in);

        static bool readMemberTypeInfo(QNrbf::MemberTypeInfo &out, QDataStream &in, qint32 count);

        static bool readBinaryArrayTypeEnum(QNrbf::BinaryArrayTypeEnumeration &out,
                                            QDataStream &in);

        static bool readBinaryTypeEnum(QNrbf::BinaryTypeEnumeration &out, QDataStream &in);

        static bool readPrimitiveTypeEnum(QNrbf::PrimitiveTypeEnumeration &out, QDataStream &in);

        static bool readPrimitive(QNrbf::ValueWithCode &out, QDataStream &in,
                                  QNrbf::PrimitiveTypeEnumeration typeEnum);
        static bool readValueWithCode(QNrbf::ValueWithCode &out, QDataStream &in);

        static bool readRemotingTypeInfo(QNrbf::RemotingTypeInfo &out, QDataStream &in,
                                           QNrbf::BinaryTypeEnumeration typeEnum);

        static bool readStringValueWithCode(QNrbf::StringValueWithCode &out, QDataStream &in);

        static bool readArrayOfValueWithCode(QNrbf::ArrayOfValueWithCode &out, QDataStream &in);
    };

}; // namespace QNrbfPrivate

#endif // QNRBFREADERCOMMONPRIVATE_H
