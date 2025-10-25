#ifndef CLASSFILE_H
#define CLASSFILE_H

#include "structures.h"


// Union do Constant Pool
typedef struct cp_info {
    enum tag tag;
    union {
        CONSTANT_Class_info class_info;

        CONSTANT_Fieldref_info fieldref_info;
        CONSTANT_Methodref_info methodref_info;
        CONSTANT_InterfaceMethodref_info interface_methodref_info;
        
        CONSTANT_String_info string_info;
        CONSTANT_Integer_info integer_info;
        CONSTANT_Float_info float_info;
        CONSTANT_Long_info long_info;
        CONSTANT_Double_info double_info;

        CONSTANT_NameAndType_info nameandtype_info;
        CONSTANT_Utf8_info utf8_info;

        // Java 8+
        CONSTANT_MethodHandle_info methodhandle_info;
        CONSTANT_MethodType_info methodtype_info;
        CONSTANT_InvokeDynamic_info invokedynamic_info;
    } info;
} cp_info;


// Struct do ClassFile
typedef struct ClassFile {
    u4 magic;

    u2 minor_version;
    u2 major_version;

    u2 constant_pool_count;
    cp_info *constant_pool;

    u2 access_flags;
    u2 this_class;
    u2 super_class;

    u2 interfaces_count;
    u2 *interfaces;

    u2 fields_count;
    field_info *fields;

    u2 methods_count;
    method_info *methods;

    u2 attributes_count;
    attribute_info *attributes;
} ClassFile;

#endif // CLASSFILE_H
