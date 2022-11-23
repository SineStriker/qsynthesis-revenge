#ifndef __QNRBFVARIANTS_H__
#define __QNRBFVARIANTS_H__

#include <QIODevice>
#include <QStringList>
#include <QVector>

#include <any>

#include "qsutils_global.h"

namespace QNrbf {

    // 2.1 Common Definitions
    // 2.1.1 Common Data Types
    // 2.1.1.5 DateTime
    class DateTime {
    public:
        int tick() const;
        int kind() const;

        quint64 _data;
    };

    // 2.1.1.8 ClassTypeInfo
    class QSUTILS_API ClassTypeInfo {
    public:
        QString typeName;
        int libraryId;
    };

    // 2.1.2 Enumerations
    // 2.1.2.1 RecordTypeEnumeration
    enum class RecordTypeEnumeration : quint8 {
        SerializedStreamHeader = 0,
        ClassWithId = 1,
        SystemClassWithMembers = 2,
        ClassWithMembers = 3,
        SystemClassWithMembersAndTypes = 4,
        ClassWithMembersAndTypes = 5,
        BinaryObjectString = 6,
        BinaryArray = 7,
        MemberPrimitiveTyped = 8,
        MemberReference = 9,
        ObjectNull = 10,
        MessageEnd = 11,
        BinaryLibrary = 12,
        ObjectNullMultiple256 = 13,
        ObjectNullMultiple = 14,
        ArraySinglePrimitive = 15,
        ArraySingleObject = 16,
        ArraySingleString = 17,
        MethodCall = 21,
        MethodReturn = 22,
    };

    // 2.1.2.2 BinaryTypeEnumeration
    enum class BinaryTypeEnumerator : quint8 {
        Primitive = 0,
        String = 1,
        Object = 2,
        SystemClass = 3,
        Class = 4,
        ObjectArray = 5,
        StringArray = 6,
        PrimitiveArray = 7,
    };

    // 2.1.2.3 PrimitiveTypeEnumeration
    enum class PrimitiveTypeEnumeration : quint8 {
        Boolean = 1,
        Byte = 2,
        Char = 3,
        Unused = 4,
        Decimal = 5,
        Double = 6,
        Int16 = 7,
        Int32 = 8,
        Int64 = 9,
        SByte = 10,
        Single = 11,
        TimeSpan = 12,
        DateTime = 13,
        UInt16 = 14,
        UInt32 = 15,
        UInt64 = 16,
        Null = 17,
        String = 18,
    };

    // 2.2 Method Invocation Records
    // 2.2.1 Enumerations
    // 2.2.1.1 MessageFlags
    enum class MessageFlags : quint32 {
        NoArgs = 0x00000001,
        ArgsInline = 0x00000002,
        ArgsIsArray = 0x00000004,
        ArgsInArray = 0x00000008,
        NoContext = 0x00000010,
        ContextInline = 0x00000020,
        ContextInArray = 0x00000040,
        MethodSignatureInArray = 0x00000080,
        PropertiesInArray = 0x00000100,
        NoReturnValue = 0x00000200,
        ReturnValueVoid = 0x00000400,
        ReturnValueInline = 0x00000800,
        ReturnValueInArray = 0x00001000,
        ExceptionInArray = 0x00002000,
        GenericMethod = 0x00008000,
    };

    // 2.2.2 Common Structures
    // 2.2.2.1 ValueWithCode
    class QSUTILS_API ValueWithCode {
    public:
        PrimitiveTypeEnumeration primitiveTypeEnum;

        bool toBool() const;
        quint8 toByte() const;
        QChar toChar() const;
        QString toDecimal() const;
        double toDouble() const;
        qint16 toInt16() const;
        qint32 toInt32() const;
        qint64 toInt64() const;
        qint8 toSByte() const;
        float toSingle() const;
        qint64 toTimeSpan() const;
        DateTime toDateTime() const;
        quint16 toUInt16() const;
        quint32 toUInt32() const;
        quint64 toUInt64() const;
        QString toString() const;

        std::any _data;
    };

    // 2.2.2.2 StringValueWithCode
    class QSUTILS_API StringValueWithCode {
    public:
        PrimitiveTypeEnumeration primitiveTypeEnum;
        QString stringValue;
    };

    // 2.2.2.3 ArrayOfValueWithCode
    class QSUTILS_API ArrayOfValueWithCode {
    public:
        QList<ValueWithCode> values;
    };

    // 2.2.3 Record Definitions
    // 2.2.3.1 BinaryMethodCall
    class QSUTILS_API BinaryMethodCall {
    public:
    };

    // 2.2.3.2 MethodCallArray
    class QSUTILS_API MethodCallArray {
    public:
    };

    // 2.2.3.3 BinaryMethodReturn
    class QSUTILS_API BinaryMethodReturn {
    public:
    };

    // 2.2.3.4 MethodReturnCallArray
    class QSUTILS_API MethodReturnCallArray {
    public:
    };

    // 2.3 Class Records
    // 2.3.1 Common Structures
    // 2.3.1.1 ClassInfo
    class QSUTILS_API ClassInfo {
    public:
        qint32 objectId;
        QString name;
        qint32 memberCount;
        QStringList memberNames;
    };

    // 2.3.1.2 MemberTypeInfo
    class QSUTILS_API MemberTypeInfo {
    public:
        class QSUTILS_API Element {
            BinaryTypeEnumerator binaryTypeEnum;

            PrimitiveTypeEnumeration toPrimitiveTypeEnum() const;
            QString toString() const;
            ClassTypeInfo toClassTypeInfo() const;

            std::any _data;
        };

        QList<Element> additionInfos;
    };

    // 2.3.2 Record Definitions
    // 2.3.2.1 ClassWithMembersAndTypes
    class QSUTILS_API ClassWithMembersAndTypes {
    public:
        ClassInfo classInfo;
        MemberTypeInfo memberTypeInfo;
        qint32 libraryId;
    };

    // 2.3.2.2 ClassWithMembers
    class QSUTILS_API ClassWithMembers {
    public:
        ClassInfo classInfo;
        qint32 libraryId;
    };

    // 2.3.2.3 SystemClassWithMembersAndTypes
    class QSUTILS_API SystemClassWithMembersAndTypes {
    public:
        ClassInfo classInfo;
        MemberTypeInfo memberTypeInfo;
    };

    // 2.3.2.4 SystemClassWithMembers
    class QSUTILS_API SystemClassWithMembers : public ClassInfo {
    public:
    };

    // 2.3.2.5 ClassWithId
    class QSUTILS_API ClassWithId {
    public:
        qint32 objectId;
        qint32 metadataId;
    };

    // 2.4 Array Records
    // 2.4.1 Enumerations
    // 2.4.1.1 BinaryArrayTypeEnumeration
    enum class BinaryArrayTypeEnumeration : quint8 {
        Single = 0,
        Jagged = 1,
        Rectangular = 2,
        SingleOffset = 3,
        JaggedOffset = 4,
        RectangularOffset = 5,
    };

    // 2.4.2 Common Definitions
    // 2.4.2.1 ArrayInfo
    class QSUTILS_API ArrayInfo {
    public:
        qint32 objectId;
        qint32 length;
    };

    // 2.4.3 Record Definitions
    // 2.4.3.1 BinaryArray
    class QSUTILS_API BinaryArray {
    public:
    };

    // 2.4.3.2 ArraySingleObject
    class QSUTILS_API ArraySingleObject : public ArrayInfo {
    public:
    };

    // 2.4.3.3 ArraySinglePrimitive
    class QSUTILS_API ArraySinglePrimitive {
    public:
        ArrayInfo arrayInfo;
        PrimitiveTypeEnumeration primitiveTypeEnum;
    };

    // 2.4.3.4 ArraySingleString
    class QSUTILS_API ArraySingleString : public ArrayInfo {
    public:
    };

    // 2.5 Member Reference Records
    // 2.5.1 MemberPrimitiveTyped
    class QSUTILS_API MemberPrimitiveTyped {
    public:
    };

    // 2.5.2 MemberPrimitiveUnTyped
    class QSUTILS_API MemberPrimitiveUnTyped {
    public:
    };

    // 2.5.3 MemberReference
    class QSUTILS_API MemberReference {
    public:
    };

    // 2.5.4 ObjectNull
    class QSUTILS_API ObjectNull {
    public:
    };

    // 2.5.5 ObjectNullMultiple
    class QSUTILS_API ObjectNullMultiple {
    public:
    };

    // 2.5.6 ObjectNullMultiple256
    class QSUTILS_API ObjectNullMultiple256 {
    public:
    };

    // 2.5.7 BinaryObjectString
    class QSUTILS_API BinaryObjectString {
    public:
    };

    // 2.6 Other Records
    // 2.6.1 SerializationHeaderRecord
    class QSUTILS_API SerializationHeaderRecord {
    public:
        qint32 rootId;
        qint32 headerId;
        qint32 majorVersion;
        qint32 minorVersion;
    };

    // 2.6.2 BinaryLibrary
    class QSUTILS_API BinaryLibrary {
    public:
        qint32 libraryId;
        QString libraryName;
    };

    // 2.6.3 MessageEnd
    class QSUTILS_API MessageEnd {
    public:
        int placeholder;
    };

}; // namespace QNrbf

#endif // __QNRBFVARIANTS_H__
