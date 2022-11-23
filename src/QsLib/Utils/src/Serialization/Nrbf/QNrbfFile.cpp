#include "QNrbfFile.h"

#include <QDataStream>
#include <QFile>

QNrbfFile::QNrbfFile() {
}

QNrbfFile::~QNrbfFile() {
}

bool QNrbfFile::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QVector<QNrbfRecord> records;
    quint8 recordType;
    bool failed = false;

    // Read primitive enum
    auto readPrimitiveEnum = [](QNrbf::PrimitiveTypeEnumeration &out, QDataStream &in) -> bool {
        quint8 primitiveTypeEnum;
        in >> primitiveTypeEnum; // set as 0 if failed
        if (primitiveTypeEnum < (quint8) QNrbf::PrimitiveTypeEnumeration::Boolean ||
            primitiveTypeEnum >= (quint8) QNrbf::PrimitiveTypeEnumeration::Null) {
            return false;
        }
        out = (QNrbf::PrimitiveTypeEnumeration) primitiveTypeEnum;
        return true;
    };

    // Read resizable length
    auto readLength = [](quint32 &size, QDataStream &in) -> bool {
        size = 0;

        quint8 flag = 0b10000000;
        quint8 mask = 0b01111111;
        quint8 byte;

        // 1st byte
        in >> byte;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
        size |= byte & mask;
        if (!(byte & flag)) {
            return true;
        }

        // 2nd/3rd/4th byte
        for (int8_t i = 0; i < 3; ++i) {
            in >> byte;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            size <<= 7;
            size |= byte & mask;
            if (!(byte & flag)) {
                return true;
            }
        }

        quint8 mask2 = 0b00000111;

        // 5th byte
        in >> byte;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
        size <<= 3;
        size |= byte & mask2;
        if (byte & (~mask2)) {
            // High 5 bits must be 0
            return false;
        }

        return true;
    };

    // Read LengthPrefixedString
    auto readString = [=](QString &out, QDataStream &in) -> bool {
        quint32 size;
        // Parse size
        if (!readLength(size, in)) {
            return false;
        }
        // Read bytes
        auto buf = new char[size];
        if (in.readRawData(buf, size) < 0) {
            delete[] buf;
            return false;
        }
        // Decode
        out = QString::fromUtf8(buf, size);
        delete[] buf;
        return true;
    };

    // Read Primitive
    auto readPrimitive = [=](QNrbf::ValueWithCode &out, QDataStream &in,
                             QNrbf::PrimitiveTypeEnumeration typeEnum) -> bool {
        out.primitiveTypeEnum = typeEnum;

        switch (typeEnum) {
            case QNrbf::PrimitiveTypeEnumeration::Boolean:
            case QNrbf::PrimitiveTypeEnumeration::Byte: {
                quint8 val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::Char: {
                quint8 head;
                in >> head;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                int size = (head & 0xD0) != 0   ? 4
                           : (head & 0xC0) != 0 ? 3
                           : (head & 0x80) != 0 ? 2
                                                : 1;
                char buf[4];
                if (in.readRawData(buf, size) < 0) {
                    return false;
                }
                ushort value =
                    (head & 0xD0) != 0 ? ((head & 0x3F) << 18) + ((buf[0] & 0x3F) << 12) +
                                             ((buf[1] & 0x3F) << 6) + (buf[0] & 0x3F)
                    : (head & 0xC0) != 0
                        ? ((head & 0x3F) << 12) + ((buf[0] & 0x3F) << 6) + (buf[1] & 0x3F)
                    : (head & 0x80) != 0 ? ((head & 0x3F) << 6) + (buf[0] & 0x3F)
                                         : (head & 0x3F);
                out._data = QChar(value);
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::Double: {
                double val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::Int16: {
                qint16 val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::Int32: {
                qint32 val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::TimeSpan:
            case QNrbf::PrimitiveTypeEnumeration::Int64: {
                qint64 val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::SByte: {
                qint8 val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::Single: {
                float val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::DateTime: {
                qint64 val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                QNrbf::DateTime dt;
                dt._data = val;
                out._data = dt;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::UInt16: {
                quint16 val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::UInt32: {
                quint32 val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::UInt64: {
                quint64 val;
                in >> val;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                out._data = val;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::Decimal:
            case QNrbf::PrimitiveTypeEnumeration::String: {
                QString str;
                if (!readString(str, in)) {
                    return false;
                }
                out._data = str;
                break;
            }
            case QNrbf::PrimitiveTypeEnumeration::Null:
            case QNrbf::PrimitiveTypeEnumeration::Unused: {
                break;
            }
        }
        return true;
    };

    // Read ValueWithCode
    auto readValueWithCode = [=](QNrbf::ValueWithCode &out, QDataStream &in) -> bool {
        QNrbf::PrimitiveTypeEnumeration primitiveTypeEnum;
        if (!readPrimitiveEnum(primitiveTypeEnum, in)) {
            return false;
        }
        if (!readPrimitive(out, in, primitiveTypeEnum)) {
            return false;
        }
        return true;
    };

    // Read ClassInfo
    auto readClassInfo = [=](QNrbf::ClassInfo &out, QDataStream &in) -> bool {
        in >> out.objectId;
        if (!readString(out.name, in)) {
            return false;
        }
        in >> out.memberCount;
        for (qint32 i = 0; i < out.memberCount; ++i) {
            QString str;
            if (!readString(str, in)) {
                return false;
            }
        }
        return true;
    };

    // Read AdditionalTypeInfo
    auto readAdditionalTypeInfo = [=](QNrbf::RemotingTypeInfo &out, QDataStream &in,
                                      QNrbf::BinaryTypeEnumeration typeEnum) -> bool {
        out.binaryTypeEnum = typeEnum;

        switch (typeEnum) {
            case QNrbf::BinaryTypeEnumeration::Primitive:
            case QNrbf::BinaryTypeEnumeration::PrimitiveArray: {
                QNrbf::PrimitiveTypeEnumeration primitiveTypeEnum;
                if (!readPrimitiveEnum(primitiveTypeEnum, in)) {
                    return false;
                }
                out._data = primitiveTypeEnum;
                break;
            }
            case QNrbf::BinaryTypeEnumeration::SystemClass: {
                QString str;
                if (!readString(str, in)) {
                    return false;
                }
                out._data = str;
                break;
            }
            case QNrbf::BinaryTypeEnumeration::Class: {
                QNrbf::ClassInfo info;
                if (!readClassInfo(info, in)) {
                    return false;
                }
                out._data = info;
                break;
            }
            case QNrbf::BinaryTypeEnumeration::String:
            case QNrbf::BinaryTypeEnumeration::Object:
            case QNrbf::BinaryTypeEnumeration::ObjectArray:
            case QNrbf::BinaryTypeEnumeration::StringArray:
            default:
                break;
        }

        return true;
    };

    // Read MemberTypeInfo
    auto readMemberTypeInfo = [=](QNrbf::MemberTypeInfo &out, QDataStream &in,
                                  qint32 count) -> bool {
        QVector<quint8> binaryTypeEnums;
        binaryTypeEnums.reserve(count);
        out.additionalInfos.reserve(count);

        // Read enums
        quint8 binaryTypeEnum;
        for (qint32 i = 0; i < count; ++i) {
            in >> binaryTypeEnum;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            binaryTypeEnums.append(binaryTypeEnum);
        }

        for (qint32 i = 0; i < count; ++i) {
            QNrbf::RemotingTypeInfo info;
            if (!readAdditionalTypeInfo(info, in,
                                        (QNrbf::BinaryTypeEnumeration) binaryTypeEnums.at(i))) {
                return false;
            }
            out.additionalInfos.append(info);
        }

        return true;
    };

    // Read ArrayInfo
    auto readArrayInfo = [=](QNrbf::ArrayInfo &out, QDataStream &in) -> bool {
        in >> out.objectId;
        in >> out.length;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
        return true;
    };

    // Read StringValueWithCode
    auto readStringValueWithCode = [=](QNrbf::StringValueWithCode &out, QDataStream &in) -> bool {
        if (!readPrimitiveEnum(out.primitiveTypeEnum, in)) {
            return false;
        }
        if (!readString(out.stringValue, in)) {
            return false;
        }
        return true;
    };

    // Read ArrayOfValueWithCode
    auto readArrayOfValueWithCode = [=](QNrbf::ArrayOfValueWithCode &out, QDataStream &in) -> bool {
        qint32 len;
        in >> len;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
        for (qint32 i = 0; i < len; ++i) {
            QNrbf::ValueWithCode val;
            if (!readValueWithCode(val, in)) {
                return false;
            }
            out.values.append(val);
        }
        return true;
    };

    // Start reading file
    QDataStream in(&file);
    while (!in.atEnd()) {
        in >> recordType;
        switch (recordType) {
            case (quint8) QNrbf::RecordTypeEnumeration::SerializedStreamHeader: {
                QNrbf::SerializationHeaderRecord record;
                in >> record.rootId;
                in >> record.headerId;
                in >> record.majorVersion;
                in >> record.minorVersion;
                if (in.status() != QDataStream::Ok) {
                    goto out_101;
                }
                records.append(record);
                break;

            out_101:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ClassWithId: {
                QNrbf::ClassWithId record;
                in >> record.objectId;
                in >> record.metadataId;
                if (in.status() != QDataStream::Ok) {
                    goto out_102;
                }
                records.append(record);
                break;

            out_102:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::SystemClassWithMembers: {
                QNrbf::SystemClassWithMembers record;
                if (!readClassInfo(record, in)) {
                    goto out_103;
                }
                records.append(record);
                break;

            out_103:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ClassWithMembers: {
                QNrbf::ClassWithMembers record;
                if (!readClassInfo(record.classInfo, in)) {
                    goto out_104;
                }
                in >> record.libraryId;
                if (in.status() != QDataStream::Ok) {
                    goto out_104;
                }
                records.append(record);
                break;

            out_104:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::SystemClassWithMembersAndTypes: {
                QNrbf::SystemClassWithMembersAndTypes record;
                if (!readClassInfo(record.classInfo, in)) {
                    goto out_105;
                }
                if (!readMemberTypeInfo(record.memberTypeInfo, in, record.classInfo.memberCount)) {
                    goto out_105;
                }
                records.append(record);
                break;

            out_105:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ClassWithMembersAndTypes: {
                QNrbf::ClassWithMembersAndTypes record;
                if (!readClassInfo(record.classInfo, in)) {
                    goto out_106;
                }
                if (!readMemberTypeInfo(record.memberTypeInfo, in, record.classInfo.memberCount)) {
                    goto out_106;
                }
                in >> record.libraryId;
                if (in.status() != QDataStream::Ok) {
                    goto out_106;
                }
                records.append(record);
                break;

            out_106:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::BinaryObjectString: {
                QNrbf::BinaryObjectString record;
                in >> record.objectId;
                if (in.status() != QDataStream::Ok) {
                    goto out_107;
                }
                if (!readString(record.value, in)) {
                    goto out_107;
                }
                records.append(record);
                break;

            out_107:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::BinaryArray: {
                QNrbf::BinaryArray record;
                in >> record.objectId;

                quint8 binaryArrayTypeEnum;
                quint8 binaryTypeEnum;

                in >> binaryArrayTypeEnum;
                if (binaryArrayTypeEnum >
                    (quint8) QNrbf::BinaryArrayTypeEnumeration::SingleOffset) {
                    goto out_108;
                }

                in >> record.rank;
                if (in.status() != QDataStream::Ok) {
                    goto out_108;
                }

                for (qint32 i = 0; i < record.rank; ++i) {
                    qint32 num;
                    in >> num;
                    if (in.status() != QDataStream::Ok) {
                        goto out_108;
                    }
                    record.lengths.append(num);
                }

                for (qint32 i = 0; i < record.rank; ++i) {
                    qint32 num;
                    in >> num;
                    if (in.status() != QDataStream::Ok) {
                        goto out_108;
                    }
                    record.lowerBounds.append(num);
                }

                in >> binaryTypeEnum;
                if (in.status() != QDataStream::Ok) {
                    goto out_108;
                }
                if (!readAdditionalTypeInfo(record.additionInfo, in,
                                            (QNrbf::BinaryTypeEnumeration) binaryTypeEnum)) {
                    goto out_108;
                }

                records.append(record);
                break;

            out_108:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MemberPrimitiveTyped: {
                QNrbf::MemberPrimitiveTyped record;
                if (!readValueWithCode(record, in)) {
                    goto out_109;
                }
                records.append(record);
                break;

            out_109:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MemberReference: {
                QNrbf::MemberReference record;
                in >> record.idRef;
                if (record.idRef <= 0) {
                    goto out_110;
                }
                records.append(record);
                break;

            out_110:
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ObjectNull: {
                QNrbf::ObjectNull record;
                record.placeholder = 0;
                records.append(record);
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MessageEnd: {
                QNrbf::MessageEnd record;
                record.placeholder = 0;
                records.append(record);
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::BinaryLibrary: {
                QNrbf::BinaryLibrary record;
                in >> record.libraryId;
                if (in.status() != QDataStream::Ok) {
                    goto out_113;
                }
                if (!readString(record.libraryName, in)) {
                    goto out_113;
                }
                records.append(record);
                break;

            out_113:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ObjectNullMultiple256: {
                QNrbf::ObjectNullMultiple256 record;
                in >> record.nullCount;
                if (in.status() != QDataStream::Ok) {
                    goto out_114;
                }
                records.append(record);
                break;

            out_114:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ObjectNullMultiple: {
                QNrbf::ObjectNullMultiple record;
                in >> record.nullCount;
                if (in.status() != QDataStream::Ok) {
                    goto out_115;
                }
                records.append(record);
                break;

            out_115:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ArraySinglePrimitive: {
                QNrbf::ArraySinglePrimitive record;
                if (!readArrayInfo(record.arrayInfo, in)) {
                    goto out_116;
                }
                if (!readPrimitiveEnum(record.primitiveTypeEnum, in)) {
                    return false;
                }
                records.append(record);
                break;

            out_116:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ArraySingleObject: {
                QNrbf::ArraySingleObject record;
                if (!readArrayInfo(record, in)) {
                    goto out_117;
                }
                records.append(record);
                break;

            out_117:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ArraySingleString: {
                QNrbf::ArraySingleString record;
                if (!readArrayInfo(record, in)) {
                    goto out_118;
                }
                records.append(record);
                break;

            out_118:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MethodCall: {
                QNrbf::BinaryMethodCall record;
                in >> record.messageEnum;
                if (in.status() != QDataStream::Ok) {
                    goto out_119;
                }
                if (!readStringValueWithCode(record.methodName, in)) {
                    goto out_119;
                }
                if (!readStringValueWithCode(record.typeName, in)) {
                    goto out_119;
                }
                if (!readStringValueWithCode(record.callContext, in)) {
                    goto out_119;
                }
                if (record.messageEnum & (quint32) QNrbf::MessageFlags::ArgsInline) {
                    if (!readArrayOfValueWithCode(record.args, in)) {
                        goto out_119;
                    }
                }
                records.append(record);
                break;

            out_119:
                failed = true;
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MethodReturn: {
                QNrbf::BinaryMethodReturn record;
                in >> record.messageEnum;
                if (in.status() != QDataStream::Ok) {
                    goto out_120;
                }
                if (!readValueWithCode(record.returnValue, in)) {
                    goto out_120;
                }
                if (!readStringValueWithCode(record.callContext, in)) {
                    goto out_120;
                }
                if (record.messageEnum & (quint32) QNrbf::MessageFlags::ArgsInline) {
                    if (!readArrayOfValueWithCode(record.args, in)) {
                        goto out_120;
                    }
                }
                records.append(record);
                break;

            out_120:
                failed = true;
                break;
            }
            default:
                break;
        }
        if (failed) {
            break;
        }
    }

    file.close();

    if (failed) {
        return false;
    }

    // Maybe something need to be done later...

    return true;
}

bool QNrbfFile::save(const QString &filename) {
    return false;
}
