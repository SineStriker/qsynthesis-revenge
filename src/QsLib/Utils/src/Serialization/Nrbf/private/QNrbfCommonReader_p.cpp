#include "QNrbfCommonReader_p.h"

#include <QDataStream>

using namespace QNrbfPrivate;

bool CommonReader::readLengthPrefix(quint32 &size, QDataStream &in) {
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
    // High 5 bits must be 0
    if (byte & (~mask2)) {
        return false;
    }

    return true;
}

bool CommonReader::readString(QString &out, QDataStream &in) {
    quint32 size;
    // Parse size
    if (!readLengthPrefix(size, in)) {
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
}

bool CommonReader::readArrayInfo(QNrbf::ArrayInfo &out, QDataStream &in) {
    in >> out.objectId;
    in >> out.length;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

bool CommonReader::readClassInfo(QNrbf::ClassInfo &out, QDataStream &in) {
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
}

bool CommonReader::readClassTypeInfo(QNrbf::ClassTypeInfo &out, QDataStream &in) {
    if (!readString(out.typeName, in)) {
        return false;
    }
    in >> out.libraryId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

bool CommonReader::readMemberTypeInfo(QNrbf::MemberTypeInfo &out, QDataStream &in, qint32 count) {
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
        if (!readRemotingTypeInfo(info, in,
                                    (QNrbf::BinaryTypeEnumeration) binaryTypeEnums.at(i))) {
            return false;
        }
        out.additionalInfos.append(info);
    }

    return true;
}

bool CommonReader::readBinaryArrayTypeEnum(QNrbf::BinaryArrayTypeEnumeration &out,
                                           QDataStream &in) {
    quint8 binArrayTypeEnum;
    in >> binArrayTypeEnum;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (binArrayTypeEnum > (quint8) QNrbf::BinaryArrayTypeEnumeration::RectangularOffset) {
        return false;
    }
    out = (QNrbf::BinaryArrayTypeEnumeration) binArrayTypeEnum;
    return true;
}

bool CommonReader::readBinaryTypeEnum(QNrbf::BinaryTypeEnumeration &out, QDataStream &in) {
    in >> out;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

bool CommonReader::readPrimitiveTypeEnum(QNrbf::PrimitiveTypeEnumeration &out, QDataStream &in) {
    quint8 primitiveTypeEnum;
    in >> primitiveTypeEnum; // set as 0 if failed
    if (primitiveTypeEnum < (quint8) QNrbf::PrimitiveTypeEnumeration::Boolean ||
        primitiveTypeEnum >= (quint8) QNrbf::PrimitiveTypeEnumeration::Null) {
        return false;
    }
    out = (QNrbf::PrimitiveTypeEnumeration) primitiveTypeEnum;
    return true;
}

bool CommonReader::readPrimitive(QNrbf::ValueWithCode &out, QDataStream &in,
                                 QNrbf::PrimitiveTypeEnumeration typeEnum) {
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
            int size = (head & 0xD0) != 0 ? 4 : (head & 0xC0) != 0 ? 3 : (head & 0x80) != 0 ? 2 : 1;
            char buf[4];
            if (in.readRawData(buf, size) < 0) {
                return false;
            }
            ushort value = (head & 0xD0) != 0 ? ((head & 0x3F) << 18) + ((buf[0] & 0x3F) << 12) +
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
}

bool CommonReader::readValueWithCode(QNrbf::ValueWithCode &out, QDataStream &in) {
    QNrbf::PrimitiveTypeEnumeration primitiveTypeEnum;
    if (!readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
        return false;
    }
    if (!readPrimitive(out, in, primitiveTypeEnum)) {
        return false;
    }
    return true;
}

bool CommonReader::readRemotingTypeInfo(QNrbf::RemotingTypeInfo &out, QDataStream &in,
                                          QNrbf::BinaryTypeEnumeration typeEnum) {
    out.binaryTypeEnum = typeEnum;

    switch (typeEnum) {
        case QNrbf::BinaryTypeEnumeration::Primitive:
        case QNrbf::BinaryTypeEnumeration::PrimitiveArray: {
            QNrbf::PrimitiveTypeEnumeration primitiveTypeEnum;
            if (!readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
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
            QNrbf::ClassTypeInfo info;
            if (!readClassTypeInfo(info, in)) {
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
}

bool CommonReader::readStringValueWithCode(QNrbf::StringValueWithCode &out, QDataStream &in) {
    if (!readPrimitiveTypeEnum(out.primitiveTypeEnum, in)) {
        return false;
    }
    if (!readString(out.stringValue, in)) {
        return false;
    }
    return true;
}

bool CommonReader::readArrayOfValueWithCode(QNrbf::ArrayOfValueWithCode &out, QDataStream &in) {
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
}
