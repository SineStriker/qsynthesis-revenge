#ifndef QNRBFRECORDREADERPRIVATE_H
#define QNRBFRECORDREADERPRIVATE_H

#include "QNrbfCommonReader_p.h"

namespace QNrbfPrivate {

    class RecordReader {
    public:
        static bool readArraySingleObject(QNrbf::ArraySingleObject &out, QDataStream &in);

        static bool readArraySinglePrimitive(QNrbf::ArraySinglePrimitive &out, QDataStream &in);

        static bool readArraySingleString(QNrbf::ArraySingleString &out, QDataStream &in);

        static bool readBinaryArray(QNrbf::BinaryArray &out, QDataStream &in);

        static bool readBinaryLibrary(QNrbf::BinaryLibrary &out, QDataStream &in);

        static bool readBinaryObjectString(QNrbf::BinaryObjectString &out, QDataStream &in);

        static bool readClassWithId(QNrbf::ClassWithId &out, QDataStream &in);

        static bool readClassWithMembers(QNrbf::ClassWithMembers &out, QDataStream &in);

        static bool readClassWithMembersAndTypes(QNrbf::ClassWithMembersAndTypes &out,
                                                 QDataStream &in);
        static bool readMemberPrimitiveTyped(QNrbf::MemberPrimitiveTyped &out, QDataStream &in);

        static bool readMemberReference(QNrbf::MemberReference &out, QDataStream &in);

        static bool readObjectNullMultiple(QNrbf::ObjectNullMultiple &out, QDataStream &in);

        static bool readObjectNullMultiple256(QNrbf::ObjectNullMultiple256 &out, QDataStream &in);

        static bool readSerializationHeader(QNrbf::SerializationHeader &out, QDataStream &in);

        static bool readSystemClassWithMembersAndTypes(QNrbf::SystemClassWithMembersAndTypes &out,
                                                       QDataStream &in);
        static bool readSystemClassWithMembers(QNrbf::SystemClassWithMembers &out, QDataStream &in);

        // Extra
        static bool readMethodCall(QNrbf::BinaryMethodCall &out, QDataStream &in);

        static bool readMethodReturn(QNrbf::BinaryMethodReturn &out, QDataStream &in);
    };

} // namespace QNrbfPrivate

#endif // QNRBFRECORDREADERPRIVATE_H
