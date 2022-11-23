#include "QNrbfRecord.h"

#include "QNrbfVariants.h"

QNrbfRecord::QNrbfRecord() {
    _type = Type::SerializedStreamHeader;
}

QNrbfRecord::~QNrbfRecord() {
}

QNrbf::RecordTypeEnumeration QNrbfRecord::type() const {
    return _type;
}

bool QNrbfRecord::isNull() const {
    return _type == Type::SerializedStreamHeader;
}

void QNrbfRecord::updateType() {

#define GoOn(TYPE)                                                                                 \
    if (_data.type() == typeid(QNrbf::TYPE)) {                                                     \
        _type = QNrbf::RecordTypeEnumeration::TYPE;                                                \
        goto out;                                                                                  \
    }

#define GoOn2(CLASS, TYPE)                                                                         \
    if (_data.type() == typeid(QNrbf::TYPE)) {                                                     \
        _type = QNrbf::RecordTypeEnumeration::CLASS;                                               \
        goto out;                                                                                  \
    }

    GoOn(ClassWithId);
    GoOn(SystemClassWithMembers);
    GoOn(ClassWithMembers);
    GoOn(SystemClassWithMembersAndTypes);
    GoOn(ClassWithMembersAndTypes);

    GoOn(BinaryObjectString);
    GoOn(BinaryArray);
    GoOn(MemberPrimitiveTyped);
    GoOn(MemberReference);

    GoOn(ObjectNull);
    GoOn(MessageEnd);
    GoOn(BinaryLibrary);

    GoOn(ObjectNullMultiple);
    GoOn(ObjectNullMultiple256);

    GoOn(ArraySingleObject);
    GoOn(ArraySinglePrimitive);
    GoOn(ArraySingleString);

    GoOn2(MethodCall, BinaryMethodCall);
    GoOn2(MethodReturn, BinaryMethodReturn);

    Q_ASSERT(false);

#undef GoOn
#undef GoOn2

out:
    return;
}
