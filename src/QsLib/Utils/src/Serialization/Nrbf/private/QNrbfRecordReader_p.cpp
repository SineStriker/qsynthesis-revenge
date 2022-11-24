#include "QNrbfRecordReader_p.h"

#include <QDataStream>

using namespace QNrbfPrivate;

bool RecordReader::readArraySingleObject(QNrbf::ArraySingleObject &out, QDataStream &in) {
    return CommonReader::readArrayInfo(out.arrayInfo, in);
}

bool RecordReader::readArraySinglePrimitive(QNrbf::ArraySinglePrimitive &out, QDataStream &in) {
    if (!CommonReader::readArrayInfo(out.arrayInfo, in)) {
        return false;
    }
    if (!CommonReader::readPrimitiveTypeEnum(out.primitiveTypeEnum, in)) {
        return false;
    }
    return true;
}

bool RecordReader::readArraySingleString(QNrbf::ArraySingleString &out, QDataStream &in) {
    return CommonReader::readArrayInfo(out.arrayInfo, in);
}

bool RecordReader::readBinaryArray(QNrbf::BinaryArray &out, QDataStream &in) {
    in >> out.objectId;

    if (!CommonReader::readBinaryArrayTypeEnum(out.binaryArrayTypeEnum, in)) {
        return false;
    }

    in >> out.rank;
    if (in.status() != QDataStream::Ok) {
        return false;
    }

    for (qint32 i = 0; i < out.rank; ++i) {
        qint32 num;
        in >> num;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
        out.lengths.append(num);
    }

    switch (out.binaryArrayTypeEnum) {
        case QNrbf::BinaryArrayTypeEnumeration::SingleOffset:
        case QNrbf::BinaryArrayTypeEnumeration::JaggedOffset:
        case QNrbf::BinaryArrayTypeEnumeration::RectangularOffset: {
            for (qint32 i = 0; i < out.rank; ++i) {
                qint32 num;
                in >> num;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out.lowerBounds.append(num);
            }
            break;
        }
        default:
            break;
    }

    QNrbf::BinaryTypeEnumeration binaryTypeEnum;
    if (!CommonReader::readBinaryTypeEnum(binaryTypeEnum, in)) {
        return false;
    }
    if (!CommonReader::readRemotingTypeInfo(out.additionInfo, in, binaryTypeEnum)) {
        return false;
    }
    return true;
}

bool RecordReader::readBinaryLibrary(QNrbf::BinaryLibrary &out, QDataStream &in) {
    in >> out.libraryId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (!CommonReader::readString(out.libraryName, in)) {
        return false;
    }
    return true;
}

bool RecordReader::readBinaryObjectString(QNrbf::BinaryObjectString &out, QDataStream &in) {
    in >> out.objectId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (!CommonReader::readString(out.value, in)) {
        return false;
    }
    return true;
}

bool RecordReader::readClassWithId(QNrbf::ClassWithId &out, QDataStream &in) {
    in >> out.objectId;
    in >> out.metadataId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

bool RecordReader::readClassWithMembers(QNrbf::ClassWithMembers &out, QDataStream &in) {
    if (!CommonReader::readClassInfo(out.classInfo, in)) {
        return false;
    }
    in >> out.libraryId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

bool RecordReader::readClassWithMembersAndTypes(QNrbf::ClassWithMembersAndTypes &out,
                                                QDataStream &in) {
    if (!CommonReader::readClassInfo(out.classInfo, in)) {
        return false;
    }
    if (!CommonReader::readMemberTypeInfo(out.memberTypeInfo, in, out.classInfo.memberCount)) {
        return false;
    }
    in >> out.libraryId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

bool RecordReader::readMemberPrimitiveTyped(QNrbf::MemberPrimitiveTyped &out, QDataStream &in) {
    return CommonReader::readValueWithCode(out, in);
}

bool RecordReader::readMemberReference(QNrbf::MemberReference &out, QDataStream &in) {
    in >> out.idRef;
    if (out.idRef <= 0) {
        return false;
    }
    return true;
}

bool RecordReader::readObjectNullMultiple(QNrbf::ObjectNullMultiple &out, QDataStream &in) {
    in >> out.nullCount;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

bool RecordReader::readObjectNullMultiple256(QNrbf::ObjectNullMultiple256 &out, QDataStream &in) {
    in >> out.nullCount;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

bool RecordReader::readSerializationHeader(QNrbf::SerializationHeader &out, QDataStream &in) {
    in >> out.rootId;
    in >> out.headerId;
    in >> out.majorVersion;
    in >> out.minorVersion;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (out.majorVersion != 1 || out.minorVersion != 0) {
        // Invalid NRBF stream
        return false;
    }
    return true;
}

bool RecordReader::readSystemClassWithMembersAndTypes(QNrbf::SystemClassWithMembersAndTypes &out,
                                                      QDataStream &in) {
    if (!CommonReader::readClassInfo(out.classInfo, in)) {
        return false;
    }
    if (!CommonReader::readMemberTypeInfo(out.memberTypeInfo, in, out.classInfo.memberCount)) {
        return false;
    }
    return true;
}

bool RecordReader::readSystemClassWithMembers(QNrbf::SystemClassWithMembers &out, QDataStream &in) {
    if (!CommonReader::readClassInfo(out.classInfo, in)) {
        return false;
    }
    return true;
}

bool RecordReader::readMethodCall(QNrbf::BinaryMethodCall &out, QDataStream &in) {
    in >> out.messageEnum;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (!CommonReader::readStringValueWithCode(out.methodName, in)) {
        return false;
    }
    if (!CommonReader::readStringValueWithCode(out.typeName, in)) {
        return false;
    }
    if ((out.messageEnum & (quint32) QNrbf::MessageFlags::ContextInline) &&
        !CommonReader::readStringValueWithCode(out.callContext, in)) {
        return false;
    }
    if ((out.messageEnum & (quint32) QNrbf::MessageFlags::ArgsInline) &&
        !CommonReader::readArrayOfValueWithCode(out.args, in)) {
        return false;
    }
    return false;
}

bool RecordReader::readMethodReturn(QNrbf::BinaryMethodReturn &out, QDataStream &in) {
    in >> out.messageEnum;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if ((out.messageEnum & (quint32) QNrbf::MessageFlags::ReturnValueInline) &&
        !CommonReader::readValueWithCode(out.returnValue, in)) {
        return false;
    }
    if ((out.messageEnum & (quint32) QNrbf::MessageFlags::ContextInline) &&
        !CommonReader::readStringValueWithCode(out.callContext, in)) {
        return false;
    }
    if ((out.messageEnum & (quint32) QNrbf::MessageFlags::ArgsInline) &&
        !CommonReader::readArrayOfValueWithCode(out.args, in)) {
        return false;
    }
    return false;
}
