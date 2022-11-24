#include "QNrbfFile.h"

#include "private/QNrbfRecordReader_p.h"

#include <QDataStream>
#include <QFile>

using RR = QNrbfPrivate::RecordReader;

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

    // Start reading file
    QDataStream in(&file);
    while (!in.atEnd()) {
        in >> recordType;
        switch (recordType) {
            case (quint8) QNrbf::RecordTypeEnumeration::SerializedStreamHeader: {
                QNrbf::SerializationHeader record;
                if (!RR::readSerializationHeader(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ClassWithId: {
                QNrbf::ClassWithId record;
                if (!RR::readClassWithId(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::SystemClassWithMembers: {
                QNrbf::SystemClassWithMembers record;
                if (!RR::readSystemClassWithMembers(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ClassWithMembers: {
                QNrbf::ClassWithMembers record;
                if (!RR::readClassWithMembers(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::SystemClassWithMembersAndTypes: {
                QNrbf::SystemClassWithMembersAndTypes record;
                if (!RR::readSystemClassWithMembersAndTypes(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ClassWithMembersAndTypes: {
                QNrbf::ClassWithMembersAndTypes record;
                if (!RR::readClassWithMembersAndTypes(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::BinaryObjectString: {
                QNrbf::BinaryObjectString record;
                if (!RR::readBinaryObjectString(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::BinaryArray: {
                QNrbf::BinaryArray record;
                if (!RR::readBinaryArray(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MemberPrimitiveTyped: {
                QNrbf::MemberPrimitiveTyped record;
                if (!RR::readMemberPrimitiveTyped(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MemberReference: {
                QNrbf::MemberReference record;
                if (!RR::readMemberReference(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
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
                if (!RR::readBinaryLibrary(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ObjectNullMultiple256: {
                QNrbf::ObjectNullMultiple256 record;
                if (!RR::readObjectNullMultiple256(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ObjectNullMultiple: {
                QNrbf::ObjectNullMultiple record;
                if (!RR::readObjectNullMultiple(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ArraySinglePrimitive: {
                QNrbf::ArraySinglePrimitive record;
                if (!RR::readArraySinglePrimitive(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ArraySingleObject: {
                QNrbf::ArraySingleObject record;
                if (!RR::readArraySingleObject(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::ArraySingleString: {
                QNrbf::ArraySingleString record;
                if (!RR::readArraySingleString(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MethodCall: {
                QNrbf::BinaryMethodCall record;
                if (!RR::readMethodCall(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
                break;
            }
            case (quint8) QNrbf::RecordTypeEnumeration::MethodReturn: {
                QNrbf::BinaryMethodReturn record;
                if (!RR::readMethodReturn(record, in)) {
                    failed = true;
                } else {
                    records.append(record);
                }
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
    Q_UNUSED(filename);
    return false;
}
