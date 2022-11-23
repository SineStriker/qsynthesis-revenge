#ifndef __QNRBFVALUE_H__
#define __QNRBFVALUE_H__

#include <QStringList>

#include "qsutils_global.h"

class QSUTILS_API QNrbfValue {
public:
    enum Type {
        // 2.1 Common Definitions
        // 2.1.1 Common Data Types
        Boolean,
        Byte,
        Int8,
        Int16,
        Int32,
        Int64,
        UInt16,
        UInt32,
        UInt64,

        Char,
        Double,
        Single,
        TimeSpan,
        DateTime,
        LengthPrefixedString,
        Decimal,
        ClassTypeInfo,

        // 2.1.2 Enumerations
        RecordTypeEnumeration,
        BinaryTypeEnumeration,
        PrimitiveTypeEnumeration,

        // 2.2 Method Invocation Records
        // 2.2.1 Enumerations
        MessageFlags,

        // 2.2.2 Common Structures
        ValueWithCode,
        StringValueWithCode,
        ArrayOfValueWithCode,

        // 2.2.3 Record Definitions
        BinaryMethodCall,
        MethodCallArray,
        BinaryMethodReturn,
        MethodReturnCallArray,

        // 2.3 Class Records
        // 2.3.1 Common Structures
        ClassInfo,
        MemberTypeInfo,

        // 2.3.2 Record Definitions
        ClassWithMembersAndTypes,
        ClassWithMembers,
        SystemClassWithMembersAndTypes,
        SystemClassWithMembers,
        ClassWithId,

        // 2.4 Array Records
        // 2.4.1 Enumerations
        BinaryArrayTypeEnumeration,

        // 2.4.2 Common Definitions
        ArrayInfo,

        // 2.4.3 Record Definitions
        BinaryArray,
        ArraySingleObject,
        ArraySinglePrimitive,
        ArraySingleString,

        // 2.5 Member Reference Records
        MemberPrimitiveTyped,
        MemberPrimitiveUnTyped,
        MemberReference,
        ObjectNull,
        ObjectNullMultiple,
        ObjectNullMultiple256,
        BinaryObjectString,

        // 2.6 Other Records
        SerializationHeaderRecord,
        BinaryLibrary,
        MessageEnd,
    };

private:
};

#endif // __QNRBFVALUE_H__
