#include "QNrbfRecord.h"

#include "QNrbfVariants.h"

QNrbfRecord::QNrbfRecord() {
    _type = QNrbf::RecordTypeEnumeration::SerializedStreamHeader;
}

QNrbfRecord::~QNrbfRecord() {
}

QNrbf::RecordTypeEnumeration QNrbfRecord::type() const {
    return _type;
}

void QNrbfRecord::updateType() {

#define GoOn(TYPE)                                                                                 \
    if (_data.type() == typeid(QNrbf::TYPE)) {                                                     \
        _type = QNrbf::RecordTypeEnumeration::TYPE;                                                \
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

//    GoOn(MethodCall);
//    GoOn(MethodReturn);


#undef GoOn

out:
    return;
}
