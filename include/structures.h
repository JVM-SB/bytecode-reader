#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdint.h> 


// Constantes
#define JAVA_CLASS_MAGIC 0xCAFEBABE


// Typedefs
typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;


// Enums Access Flags
enum ClassAccessFlags {
    CLASS_ACC_PUBLIC        = (u2) 0x0001,
    CLASS_ACC_FINAL         = (u2) 0x0010,
    CLASS_ACC_SUPER         = (u2) 0x0020,
    CLASS_ACC_INTERFACE     = (u2) 0x0200,
    CLASS_ACC_ABSTRACT      = (u2) 0x0400,
    CLASS_ACC_SYNTHETIC     = (u2) 0x1000,
    CLASS_ACC_ANNOTATION    = (u2) 0x2000,
    CLASS_ACC_ENUM          = (u2) 0x4000
};

enum FieldAccessFlags {
    FIELD_ACC_PUBLIC        = (u2) 0x0001,
    FIELD_ACC_PRIVATE       = (u2) 0x0002,
    FIELD_ACC_PROTECTED     = (u2) 0x0004,
    FIELD_ACC_STATIC        = (u2) 0x0008,
    FIELD_ACC_FINAL         = (u2) 0x0010,
    FIELD_ACC_VOLATILE      = (u2) 0x0040,
    FIELD_ACC_TRANSIENT     = (u2) 0x0080,
    FIELD_ACC_SYNTHETIC     = (u2) 0x1000,
    FIELD_ACC_ENUM          = (u2) 0x4000
};

enum MethodAccessFlags {
    METHOD_ACC_PUBLIC        = (u2) 0x0001,
    METHOD_ACC_PRIVATE       = (u2) 0x0002,
    METHOD_ACC_PROTECTED     = (u2) 0x0004,
    METHOD_ACC_STATIC        = (u2) 0x0008,
    METHOD_ACC_FINAL         = (u2) 0x0010,
    METHOD_ACC_SYNCHRONIZED  = (u2) 0x0020,
    METHOD_ACC_BRIDGE        = (u2) 0x0040,
    METHOD_ACC_VARARGS       = (u2) 0x0080,
    METHOD_ACC_NATIVE        = (u2) 0x0100,
    METHOD_ACC_ABSTRACT      = (u2) 0x0400,
    METHOD_ACC_STRICT        = (u2) 0x0800,
    METHOD_ACC_SYNTHETIC     = (u2) 0x1000
};

enum InnerClassAccessFlags {
    INNER_CLASS_ACC_PUBLIC        = (u2) 0x0001,
    INNER_CLASS_ACC_PRIVATE       = (u2) 0x0002,
    INNER_CLASS_ACC_PROTECTED     = (u2) 0x0004,
    INNER_CLASS_ACC_STATIC        = (u2) 0x0008,
    INNER_CLASS_ACC_FINAL         = (u2) 0x0010,
    INNER_CLASS_ACC_INTERFACE     = (u2) 0x0200,
    INNER_CLASS_ACC_ABSTRACT      = (u2) 0x0400,
    INNER_CLASS_ACC_SYNTHETIC     = (u2) 0x1000,
    INNER_CLASS_ACC_ANNOTATION    = (u2) 0x2000,
    INNER_CLASS_ACC_ENUM          = (u2) 0x4000
};


// Constant Pool Tags
enum tag {
    CONSTANT_Class              = (u1) 7,
    CONSTANT_Fieldref           = (u1) 9,
    CONSTANT_Methodref          = (u1) 10,
    CONSTANT_InterfaceMethodref = (u1) 11,
    CONSTANT_String             = (u1) 8,
    CONSTANT_Integer            = (u1) 3,
    CONSTANT_Float              = (u1) 4,
    CONSTANT_Long               = (u1) 5,
    CONSTANT_Double             = (u1) 6,
    CONSTANT_NameAndType        = (u1) 12,
    CONSTANT_Utf8               = (u1) 1,
    CONSTANT_MethodHandle       = (u1) 15,
    CONSTANT_MethodType         = (u1) 16,
    CONSTANT_InvokeDynamic      = (u1) 18
};

enum array_type {
    T_BOOLEAN = (u1) 4,
    T_CHAR    = (u1) 5,
    T_FLOAT   = (u1) 6,
    T_DOUBLE  = (u1) 7,
    T_BYTE    = (u1) 8,
    T_SHORT   = (u1) 9,
    T_INT     = (u1) 10,
    T_LONG    = (u1) 11
};


// Structs do Constant Pool Info
typedef struct CONSTANT_Class_info {
    u1 tag;
    u2 name_index;
} CONSTANT_Class_info;

typedef struct CONSTANT_Fieldref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_Fieldref_info;

typedef struct CONSTANT_Methodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_Methodref_info;

typedef struct CONSTANT_InterfaceMethodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_InterfaceMethodref_info;

typedef struct CONSTANT_String_info {
    u1 tag;
    u2 string_index;
} CONSTANT_String_info;

typedef struct CONSTANT_Integer_info {
    u1 tag;
    u4 bytes;
} CONSTANT_Integer_info;

typedef struct CONSTANT_Float_info {
    u1 tag;
    u4 bytes;
} CONSTANT_Float_info;

typedef struct CONSTANT_Long_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
} CONSTANT_Long_info;

typedef struct CONSTANT_Double_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
} CONSTANT_Double_info;

typedef struct CONSTANT_NameAndType_info {
    u1 tag;
    u2 name_index;
    u2 descriptor_index;
} CONSTANT_NameAndType_info;

typedef struct CONSTANT_Utf8_info {
    u1 tag;
    u2 length;
    u1 *bytes;
} CONSTANT_Utf8_info;

typedef struct CONSTANT_MethodHandle_info {
    u1 tag;
    u1 reference_kind;
    u2 reference_index;
} CONSTANT_MethodHandle_info;

typedef struct CONSTANT_MethodType_info {
    u1 tag;
    u2 descriptor_index;
} CONSTANT_MethodType_info;

typedef struct CONSTANT_InvokeDynamic_info {
    u1 tag;
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
} CONSTANT_InvokeDynamic_info;


// Field, Attribute e Method Info
typedef struct attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 *info;
} attribute_info;

typedef struct field_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info *attributes;
} field_info;

typedef struct method_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info *attributes;
} method_info;

typedef struct exception_table{
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
} exception_table;

// Attribute Code
typedef struct code_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 *code;
    u2 exception_table_length;
    exception_table *exception_table;
    u2 attributes_count;
    attribute_info *attributes;
} code_attribute;


// Verification Type Info
typedef struct verification_type_info {
    u1 tag;

    union {
        struct {
            // u1 tag;  /* 0 */
        } top_variable_info;

        struct {
            // u1 tag;  /* 1 */
        } integer_variable_info;

        struct {
            // u1 tag;  /* 2 */
        } float_variable_info;

        struct {
            // u1 tag;  /* 4*/
        } long_variable_info;

        struct {
            // u1 tag;  /* 3 */
        } double_variable_info;

        struct {
            // u1 tag;  /* 5 */
        } null_variable_info;

        struct {
            // u1 tag;  /* 6 */
        } uninitialized_this_variable_info;

        struct {
            // u1 tag;  /* 7 */
            u2 cpool_index;
        } object_variable_info;

        struct {
            // u1 tag;  /* 8 */
            u2 offset;
        } uninitialized_variable_info;

    } type_info;

} verification_type_info;


// Class File Attribute
typedef struct constantValue_attribute {
    u2 constant_value_index;
    u4 attribute_length;
    u2 constantvalue_index;
} constantValue_attribute;

typedef struct stack_map_frame {
    u1 frame_type;
    union {
        struct {
            // u1 frame_type;  /* = SAME (0-63) */
        } same_frame;

        struct {
            // u1 frame_type;  /* = SAME_LOCALS_1_STACK_ITEM_FRAME (64-127) */
            verification_type_info** stack;
        } same_locals_1_stack_item_frame;

        struct {
            // u1 frame_type; /* = SAME_LOCALS_1_STACK_ITEM_FRAME_EXTENDED (247) */
            u2 offset_delta;
            verification_type_info** stack;
        } same_locals_1_stack_item_frame_extended;

        struct {
            // u1 frame_type; /* = CHOP_FRAME (248-250) */
            u2 offset_delta;
        } chop_frame;

        struct {
            // u1 frame_type; /* = SAME_FRAME_EXTENDED (251) */
            u2 offset_delta;
        } same_frame_extended;

        struct {
            // u1 frame_type; /* = APPEND_FRAME (252-254) */
            u2 offset_delta;
            verification_type_info** locals;    /* [frame_type - 251]*/
        } append_frame;

        struct {
            // u1 frame_type; /* = FULL_FRAME (255) */
            u2 offset_delta;
            u2 number_of_locals;
            verification_type_info** locals;

            u2 number_of_stack_items;
            verification_type_info** stack;
        } full_frame;

    } map_frame_type;

} stack_map_frame;

typedef struct stackMapTable_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_entries;
    stack_map_frame** entries;
} stackMapTable_attribute;



typedef struct exceptions_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_exceptions;
    u2 *exception_index_table;
} exceptions_attribute;

typedef struct classes {
    u2 inner_class_info_index;
    u2 outer_class_info_index;
    u2 inner_name_index;
    u2 inner_class_access_flags;
} classes;

typedef struct innerClasses_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_classes;
    classes *classes_vec;
} innerClasses_attribute;

typedef struct enclosingMethod_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 class_index;
    u2 method_index;
} enclosingMethod_attribute;

typedef struct synthetic_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
} synthetic_attribute;

typedef struct signature_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 signature_index;
} signature_attribute;

typedef struct sourceFile_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 sourcefile_index;
} sourceFile_attribute;

typedef struct sourceDebugExtension_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 *debug_extension;
} sourceDebugExtension_attribute;

typedef struct line_number_table{
    u2 start_pc;
    u2 line_number;
} line_number_table;

typedef struct lineNumberTable_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 line_number_table_length;
    line_number_table* line_number_table_vec;
} lineNumberTable_attribute;

typedef struct localVariableTable_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 local_variable_table_length;
    struct {
        u2 start_pc;
        u2 length;
        u2 name_index;
        u2 descriptor_index;
        u2 index;
    } *local_variable_table;
} localVariableTable_attribute;

typedef struct localVariableTypeTable_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 local_variable_type_table_length;
    struct {
        u2 start_pc;
        u2 length;
        u2 name_index;
        u2 signature_index;
        u2 index;
    } *local_variable_type_table;
} localVariableTypeTable_attribute;

typedef struct deprecated_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
} deprecated_attribute;

typedef struct annotation annotation;
struct element_value;

typedef struct element_value {
    u1 tag;
    union {
        u2 const_value_index;

        struct {   
            u2 type_name_index;
            u2 const_name_index;
        } enum_const_value;

        u2 class_info_index;

        annotation* annotation_value;

        struct {   
            u2 num_values;
            struct element_value* values;
        } array_value;

    } value;

} element_value;

typedef struct annotation {
    u2 type_index;
    u2 num_element_value_pairs;
    struct {
        u2 element_name_index;
        element_value value;
    } *element_value_pairs;
} annotation;


typedef struct runtimeVisibleAnnotations_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 num_annotations;
    annotation *annotations;
} runtimeVisibleAnnotations_attribute;

typedef struct runtimeInvisibleAnnotations_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 num_annotations;
    annotation *annotations;
} runtimeInvisibleAnnotations_attribute;

typedef struct parameter_annotations{
        u2 num_annotations;
        annotation *annotations;
} parameter_annotations;

typedef struct runtimeVisibleParameterAnnotations_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 num_parameters;
    parameter_annotations *parameter_annotations;
} runtimeVisibleParameterAnnotations_attribute;

typedef struct runtimeInvisibleParameterAnnotations_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 num_parameters;
    parameter_annotations *parameter_annotations;
} runtimeInvisibleParameterAnnotations_attribute;

typedef struct annotation_default_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    element_value default_value;
} annotation_default_attribute;

typedef struct bootstrapMethods_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 num_bootstrap_methods;
    struct {
        u2 bootstrap_method_ref;
        u2 num_bootstrap_arguments;
        u2 *bootstrap_arguments;
    } *bootstrap_methods;
} bootstrap_methods_attribute;

#endif  // STRUCTURES_H
