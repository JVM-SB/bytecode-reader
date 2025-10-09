#include <stdio.h>
#include <stdint.h>

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

// Access Flags

enum ClassAccessFlags {
    CLASS_ACC_PUBLIC = (u2) 0x0001,
    CLASS_ACC_FINAL = (u2) 0x0010,
    CLASS_ACC_SUPER = (u2) 0x0020,
    CLASS_ACC_INTERFACE = (u2) 0x0200,
    CLASS_ACC_ABSTRACT = (u2) 0x0400,
    CLASS_ACC_SYNTHETIC = (u2) 0x1000,
    CLASS_ACC_ANNOTATION = (u2) 0x2000,
    CLASS_ACC_ENUM = (u2) 0x4000
};

enum FieldAccessFlags {
    FIELD_ACC_PUBLIC = (u2) 0x0001,
    FIELD_ACC_PRIVATE = (u2) 0x0002,
    FIELD_ACC_PROTECTED = (u2) 0x0004,
    FIELD_ACC_STATIC = (u2) 0x0008,
    FIELD_ACC_FINAL = (u2) 0x0010,
    FIELD_ACC_VOLATILE = (u2) 0x0040,
    FIELD_ACC_TRANSIENT = (u2) 0x0080,
    FIELD_ACC_SYNTHETIC = (u2) 0x1000,
    FIELD_ACC_ENUM = (u2) 0x4000
};

enum MethodAccessFlags {
    METHOD_ACC_PUBLIC = (u2) 0x0001,
    METHOD_ACC_PRIVATE = (u2) 0x0002,
    METHOD_ACC_PROTECTED = (u2) 0x0004,
    METHOD_ACC_STATIC = (u2) 0x0008,
    METHOD_ACC_FINAL = (u2) 0x0010,
    METHOD_ACC_SYNCHRONIZED = (u2) 0x0020,
    METHOD_ACC_BRIDGE = (u2) 0x0040,
    METHOD_ACC_VARARGS = (u2) 0x0080,
    METHOD_ACC_NATIVE = (u2) 0x0100,
    METHOD_ACC_ABSTRACT = (u2) 0x0400,
    METHOD_ACC_STRICT = (u2) 0x0800,
    METHOD_ACC_SYNTHETIC = (u2) 0x1000
};

// Constant Pool Tags

enum tag {
    CONSTANT_Class = (u1) 7,
    CONSTANT_Fieldref = (u1) 9,
    CONSTANT_Methodref = (u1) 10,
    CONSTANT_InterfaceMethodref = (u1) 11,
    CONSTANT_String = (u1) 8,
    CONSTANT_Integer = (u1) 3,
    CONSTANT_Float = (u1) 4,
    CONSTANT_Long = (u1) 5,
    CONSTANT_Double = (u1) 6,
    CONSTANT_NameAndType = (u1) 12,
    CONSTANT_Utf8 = (u1) 1,
    CONSTANT_MethodHandle = (u1) 15,
    CONSTANT_MethodType = (u1) 16,
    CONSTANT_InvokeDynamic = (u1) 18
};

struct CONSTANT_Utf8_info {
    u1 tag;
    u2 length;
    u1 *bytes;
};

struct CONSTANT_Fieldref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
};

struct CONSTANT_Methodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
};

struct CONSTANT_InterfaceMethodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
};

struct CONSTANT_NameAndType_info {
    u1 tag;
    u2 name_index;
    u2 descriptor_index;
};

struct CONSTANT_String_info {
    u1 tag;
    u2 string_index;
};

struct CONSTANT_Integer_info {
    u1 tag;
    u4 bytes;
};

struct CONSTANT_Float_info {
    u1 tag;
    u4 bytes;
};

struct CONSTANT_Long_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
};

struct CONSTANT_Double_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
};

struct CONSTANT_Class_info {
    u1 tag;
    u2 name_index;
};

// Union of all constant pool info types

struct cp_info {
    enum tag tag;
    union {
        struct CONSTANT_Utf8_info utf8_info;
        struct CONSTANT_Fieldref_info fieldref_info;
        struct CONSTANT_Methodref_info methodref_info;
        struct CONSTANT_InterfaceMethodref_info interface_methodref_info;
        struct CONSTANT_NameAndType_info nameandtype_info;

        struct CONSTANT_String_info string_info;
        struct CONSTANT_Integer_info integer_info;
        struct CONSTANT_Float_info float_info;
        struct CONSTANT_Long_info long_info;
        struct CONSTANT_Double_info double_info;

        struct CONSTANT_Class_info class_info;
    } info;
};

// Attribute types

struct attribute_code {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 *code;
    u2 exception_table_length;
    struct exception_table *exception_table;
    u2 attributes_count;
    struct attribute_info *attributes;
};

struct exception_table{
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
};


// Attribute, Field, Method, and ClassFile Structures

struct attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 *info;
};

struct field_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    struct attribute_info *attributes;
};

struct ConstantValue_attribute {
    u2 constant_value_index;
};

struct method_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    struct attribute_info *attributes;
};


struct ClassFile {
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    struct cp_info *constant_pool;
    enum ClassAccessFlags access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces;
    u2 fields_count;
    struct field_info *fields;
    u2 methods_count;
    struct method_info *methods;
    u2 attributes_count;
    struct attribute_info *attributes;
};

