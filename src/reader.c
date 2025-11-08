#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "reader.h"


// Protótipos para conhecimento do compilador

static cp_info* readConstantPool(FILE *file, u2 cp_count);
static u2* readInterfaces(FILE *file, u2 interfaces_count);
static attribute_info* readAttributes(FILE *fp, cp_info *cp, u2 attributes_count);
static field_info* readFields(FILE *file, cp_info *cp, u2 fields_count);
static method_info* readMethods(FILE *file, cp_info *cp, u2 methods_count);
static code_attribute* readCode(FILE *fp, cp_info *cp);
static exceptions_attribute* readExceptionsAttribute(FILE *fp);
static constantValue_attribute* readConstantValue(FILE *fp);
static exception_table* readExceptionTable(FILE *fp, u2 size);
static innerClasses_attribute* readInnerClasses(FILE *fp);
static lineNumberTable_attribute* readLineNumberTableAttribute(FILE *fp);
static sourceFile_attribute* readSourceFile(FILE *fp);

static void freeAttributes(attribute_info *attributes, u2 attributes_count, cp_info *cp);
static void freeVerificationTypeInfo(verification_type_info *info);
static void freeStackMapFrame(stack_map_frame *frame);
static void freeStackMapTable(stackMapTable_attribute *table);
static void freeBootstrapMethods(bootstrap_methods_attribute *bsm_attr);



u1 u1Read(FILE *file) {
    return getc(file);
}

u2 u2Read(FILE *file) {
    u2 toReturn = getc(file);

    toReturn = (toReturn << 8) | getc(file);
    
    return toReturn;
}

static u4 u4Read(FILE *file) {
    u4 toReturn = getc(file);

    toReturn = (toReturn << 8) | getc(file);
    toReturn = (toReturn << 8) | getc(file);
    toReturn = (toReturn << 8) | getc(file);

    return toReturn;
}

static char* getUtf8StringFromCp(cp_info *cp, u2 index) {
    if (index == 0) {
        fprintf(stderr, "Erro: Índice de CP inválido (0) ao buscar string UTF-8.\n");

        return NULL;
    }
    
    cp_info *cp_entry = &cp[index - 1];

    if (cp_entry->tag != CONSTANT_Utf8) {
        fprintf(stderr, "Erro: Índice de CP (%d) não aponta para CONSTANT_Utf8 (Tag: %d).\n", index, cp_entry->tag);

        return NULL;
    }

    CONSTANT_Utf8_info utf8_info = cp_entry->info.utf8_info;

    char *str = malloc(utf8_info.length + 1);
    if (str == NULL) {
        fprintf(stderr, "Erro ao alocar string para UTF-8.\n");

        return NULL;
    }

    memcpy(str, utf8_info.bytes, utf8_info.length);
    str[utf8_info.length] = '\0';

    return str;
}


ClassFile* readFile(const char *filename) {
    printf("--- Lendo arquivo: %s ---\n", filename);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);

        return NULL;
    }

    struct ClassFile *cf = malloc(sizeof(struct ClassFile));
    if (cf == NULL) {
        fprintf(stderr, "Erro ao alocar ClassFile.\n");

        fclose(file);

        return NULL;
    }

    cf->magic = u4Read(file);
    if (cf->magic != JAVA_CLASS_MAGIC) {
        fprintf(stderr, "Erro Magic Number inválido (%x) esperado 0xCAFEBABE.\n", cf->magic);

        fclose(file);
        free(cf);

        return NULL;
    }

    cf->minor_version = u2Read(file);
    cf->major_version = u2Read(file);

    cf->constant_pool_count = u2Read(file);
    cf->constant_pool = readConstantPool(file, cf->constant_pool_count);
    if (cf->constant_pool == NULL) {
        fclose(file);
        
        free(cf);

        return NULL;
    }

    cf->access_flags = u2Read(file);
    cf->this_class = u2Read(file);
    cf->super_class = u2Read(file);
    
    cf->interfaces_count = u2Read(file);
    cf->interfaces = readInterfaces(file, cf->interfaces_count); 

    cf->fields_count = u2Read(file);
    cf->fields = readFields(file, cf->constant_pool, cf->fields_count);
    
    cf->methods_count = u2Read(file);
    cf->methods = readMethods(file, cf->constant_pool, cf->methods_count);

    cf->attributes_count = u2Read(file);
    cf->attributes = readAttributes(file, cf->constant_pool, cf->attributes_count); 

    fclose(file);

    return cf;
}

static cp_info* readConstantPool(FILE *file, u2 cp_count) {
    cp_info *cp = calloc(cp_count - 1, sizeof(cp_info));
    if (cp == NULL) {
        fprintf(stderr, "Erro ao alocar Constant Pool.\n");
        return NULL;
    }

    for (int i = 0; i < cp_count - 1; i++) {
        cp[i].tag = (enum tag) u1Read(file);

        switch (cp[i].tag) {
            case CONSTANT_Class:
                cp[i].info.class_info.name_index = u2Read(file);
                break;

            case CONSTANT_Fieldref:
                cp[i].info.fieldref_info.class_index = u2Read(file);
                cp[i].info.fieldref_info.name_and_type_index = u2Read(file);
                break;

            case CONSTANT_Methodref:
                cp[i].info.methodref_info.class_index = u2Read(file);
                cp[i].info.methodref_info.name_and_type_index = u2Read(file);
                break;

            case CONSTANT_InterfaceMethodref:
                cp[i].info.interface_methodref_info.class_index = u2Read(file);
                cp[i].info.interface_methodref_info.name_and_type_index = u2Read(file);
                break;

            case CONSTANT_String:
                cp[i].info.string_info.string_index = u2Read(file);
                break;
            
            case CONSTANT_Integer:
                cp[i].info.integer_info.bytes = u4Read(file);
                break;
            
            case CONSTANT_Float:
                cp[i].info.float_info.bytes = u4Read(file);
                break;
            
            case CONSTANT_Long:
                cp[i].info.long_info.high_bytes = u4Read(file);
                cp[i].info.long_info.low_bytes = u4Read(file);
                i++;
                break;
            
            case CONSTANT_Double:
                cp[i].info.double_info.high_bytes = u4Read(file);
                cp[i].info.double_info.low_bytes = u4Read(file);
                i++;
                break;

            case CONSTANT_NameAndType:
                cp[i].info.nameandtype_info.name_index = u2Read(file);
                cp[i].info.nameandtype_info.descriptor_index = u2Read(file);
                break;

            case CONSTANT_Utf8:
                cp[i].info.utf8_info.length = u2Read(file);

                cp[i].info.utf8_info.bytes = malloc(cp[i].info.utf8_info.length * sizeof(u1));
                if (cp[i].info.utf8_info.bytes == NULL) {
                    fprintf(stderr, "Erro ao alocar bytes do CONSTANT_Utf8_info.\n");

                    free(cp);
                    return NULL;
                }

                for (int j = 0; j < cp[i].info.utf8_info.length; j++) {
                    cp[i].info.utf8_info.bytes[j] = u1Read(file);
                }
                break;
            
            case CONSTANT_MethodHandle:
                cp[i].info.methodhandle_info.reference_kind = u1Read(file);
                cp[i].info.methodhandle_info.reference_index = u2Read(file);
                break;

            case CONSTANT_MethodType:
                cp[i].info.methodtype_info.descriptor_index = u2Read(file);
                break;

            case CONSTANT_InvokeDynamic:
                cp[i].info.invokedynamic_info.bootstrap_method_attr_index = u2Read(file);
                cp[i].info.invokedynamic_info.name_and_type_index = u2Read(file);
                break;

            default:
                fprintf(stderr, "Erro: Tag desconhecida: %d\n", cp[i].tag);

                free(cp);
                return NULL;
        }
    }

    return cp;
}


static u2* readInterfaces(FILE *file, u2 interfaces_count) {
    if (interfaces_count == 0) {
        return NULL;
    }

    printf("Lendo %u interfaces...\n", interfaces_count);
    u2 *interfaces = malloc(interfaces_count * sizeof(u2));
    if (interfaces == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o array de interfaces.\n");
        return NULL;
    }

    for (u2 i = 0; i < interfaces_count; i++) {
        interfaces[i] = u2Read(file);
    }

    return interfaces;
}

static exception_table* readExceptionTable(FILE *fp, u2 size) {
    if (size == 0) return NULL;

    exception_table *table = malloc(size * sizeof(exception_table));
    if (table == NULL) {
        fprintf(stderr, "Erro ao alocar exception_table.\n");
        return NULL;
    }
    for (u2 i = 0; i < size; i++) {
        table[i].start_pc = u2Read(fp);
        table[i].end_pc = u2Read(fp);
        table[i].handler_pc = u2Read(fp);
        table[i].catch_type = u2Read(fp);
    }
    return table;
}

static code_attribute* readCode(FILE *fp, cp_info *cp) {
    code_attribute *code_attr = malloc(sizeof(code_attribute));
    if (code_attr == NULL) {
        fprintf(stderr, "Erro ao alocar code_attribute.\n");
        return NULL;
    }

    code_attr->max_stack = u2Read(fp);
    code_attr->max_locals = u2Read(fp);
    code_attr->code_length = u4Read(fp);

    if (code_attr->code_length > 0) {
        code_attr->code = malloc(code_attr->code_length * sizeof(u1));
        if (code_attr->code == NULL) {
            fprintf(stderr, "Erro ao alocar code array.\n");

            free(code_attr);
            return NULL;
        }
        
        if (fread(code_attr->code, sizeof(u1), code_attr->code_length, fp) != code_attr->code_length) {
            fprintf(stderr, "Erro ao ler bytecodes.\n");

            free(code_attr->code);
            free(code_attr);
            return NULL;
        }
    } else {
        code_attr->code = NULL;
    }

    code_attr->exception_table_length = u2Read(fp);
    code_attr->exception_table = readExceptionTable(fp, code_attr->exception_table_length);

    code_attr->attributes_count = u2Read(fp);
    code_attr->attributes = readAttributes(fp, cp, code_attr->attributes_count); 

    return code_attr;
}

static constantValue_attribute* readConstantValue(FILE *fp) {
    constantValue_attribute *cv_attr = malloc(sizeof(constantValue_attribute));
    if (cv_attr == NULL) {
         fprintf(stderr, "Erro ao alocar constantValue_attribute.\n");
         return NULL;
    }

    cv_attr->constantvalue_index = u2Read(fp);
    return cv_attr;
}

static sourceFile_attribute* readSourceFile(FILE *fp) {
     sourceFile_attribute *sf_attr = malloc(sizeof(sourceFile_attribute));
     if (sf_attr == NULL) {
         fprintf(stderr, "Erro ao alocar sourceFile_attribute.\n");
         return NULL;
     }

     sf_attr->sourcefile_index = u2Read(fp);
     return sf_attr;
}

static exceptions_attribute* readExceptionsAttribute(FILE *fp) {
    exceptions_attribute *ex_attr = malloc(sizeof(exceptions_attribute));
    if (ex_attr == NULL) {
        fprintf(stderr, "Erro ao alocar exceptions_attribute.\n");
        return NULL;
    }
    ex_attr->number_of_exceptions = u2Read(fp);

    if (ex_attr->number_of_exceptions > 0) {
        ex_attr->exception_index_table = malloc(ex_attr->number_of_exceptions * sizeof(u2));
        if (ex_attr->exception_index_table == NULL) {
            fprintf(stderr, "Erro ao alocar exception_index_table.\n");

            free(ex_attr);
            return NULL;
        }

        for (u2 i = 0; i < ex_attr->number_of_exceptions; i++) {
            ex_attr->exception_index_table[i] = u2Read(fp);
        }

    } else {
        ex_attr->exception_index_table = NULL;
    }

    return ex_attr;
}

static lineNumberTable_attribute* readLineNumberTableAttribute(FILE *fp) {
    lineNumberTable_attribute *ln_attr = malloc(sizeof(lineNumberTable_attribute));
    if (ln_attr == NULL) {
         fprintf(stderr, "Erro ao alocar lineNumberTable_attribute.\n");
         return NULL;
    }

    ln_attr->line_number_table_length = u2Read(fp);

    if (ln_attr->line_number_table_length > 0) {
        u2 table_size = ln_attr->line_number_table_length;

        ln_attr->line_number_table_vec = malloc(table_size * sizeof(line_number_table));
        if (ln_attr->line_number_table_vec == NULL) {
            fprintf(stderr, "Erro ao alocar line_number_table.\n");

            free(ln_attr);
            return NULL;
        }

        for (u2 i = 0; i < table_size; i++) {
            ln_attr->line_number_table_vec[i].start_pc = u2Read(fp);
            ln_attr->line_number_table_vec[i].line_number = u2Read(fp);
        }

    } else {
        ln_attr->line_number_table_vec = NULL;
    }

    return ln_attr;
}

static innerClasses_attribute* readInnerClasses(FILE *fp) {
    innerClasses_attribute *ic_attr = malloc(sizeof(innerClasses_attribute));
    if (ic_attr == NULL) {
        fprintf(stderr, "Erro ao alocar innerClasses_attribute.\n");
        return NULL;
    }

    ic_attr->number_of_classes = u2Read(fp);
    ic_attr->classes_vec = malloc(ic_attr->number_of_classes * sizeof(classes));
    if (ic_attr->classes_vec == NULL) {
        fprintf(stderr, "Erro ao alocar classes_vec.\n");
        free(ic_attr);
        return NULL;
    }

    for (u2 i = 0; i < ic_attr->number_of_classes; i++) {
        ic_attr->classes_vec[i].inner_class_info_index = u2Read(fp);
        ic_attr->classes_vec[i].outer_class_info_index = u2Read(fp);
        ic_attr->classes_vec[i].inner_name_index = u2Read(fp);
        ic_attr->classes_vec[i].inner_class_access_flags = u2Read(fp);
    }

    return ic_attr;
}

static attribute_info* readAttributes(FILE *fp, cp_info *cp, u2 attributes_count) {
    if (attributes_count == 0) {
        return NULL;
    }
    
    attribute_info *attributes = calloc(attributes_count, sizeof(attribute_info));
    if (attributes == NULL) {
        fprintf(stderr, "Erro ao alocar array de attributes.\n");
        return NULL;
    }

    for (u2 i = 0; i < attributes_count; i++) {
        attributes[i].attribute_name_index = u2Read(fp);
        attributes[i].attribute_length = u4Read(fp);

        char *attr_name = getUtf8StringFromCp(cp, attributes[i].attribute_name_index);
        
        if (attr_name == NULL) {
            fprintf(stderr, "    Erro: Atributo [%u] com name_index inválido #%d. Pulando %u bytes.\n", i, attributes[i].attribute_name_index, attributes[i].attribute_length);

            fseek(fp, attributes[i].attribute_length, SEEK_CUR);
            attributes[i].info = NULL; 
            
            continue;
        }

        if (strcmp(attr_name, "Code") == 0) {
            attributes[i].info = (u1*)readCode(fp, cp);

        } else if (strcmp(attr_name, "ConstantValue") == 0) {
            attributes[i].info = (u1*)readConstantValue(fp);

        } else if (strcmp(attr_name, "SourceFile") == 0) {
            attributes[i].info = (u1*)readSourceFile(fp);

        } else if (strcmp(attr_name, "Exceptions") == 0) {
            attributes[i].info = (u1*)readExceptionsAttribute(fp);

        } else if (strcmp(attr_name, "LineNumberTable") == 0) {
            attributes[i].info = (u1*)readLineNumberTableAttribute(fp);

        } else if (strcmp(attr_name, "InnerClasses") == 0) {
            attributes[i].info = (u1*)readInnerClasses(fp);
       
        } else if (strcmp(attr_name, "StackMapTable") == 0) {
            attributes[i].info = (u1*)readStackMapTable(fp);
        
        } else if (strcmp(attr_name, "BootstrapMethods") == 0) {
            attributes[i].info = (u1*)readBootstrapMethods(fp);

        } else if (strcmp(attr_name, "Synthetic") == 0) {
            // A especificação diz que o 'attribute_length' DEVE ser 0.
            if (attributes[i].attribute_length != 0) {
                fprintf(stderr, "Warning: Atributo Synthetic com tamanho %u != 0\n", attributes[i].attribute_length);

                fseek(fp, attributes[i].attribute_length, SEEK_CUR);
            }

            attributes[i].info = calloc(1, sizeof(synthetic_attribute));

        } else if (strcmp(attr_name, "Deprecated") == 0) {
            // Atributo marcador, 'attribute_length' DEVE ser 0.
             if (attributes[i].attribute_length != 0) {
                fprintf(stderr, "Warning: Atributo Deprecated com tamanho %u != 0\n", attributes[i].attribute_length);

                fseek(fp, attributes[i].attribute_length, SEEK_CUR);
            }

            attributes[i].info = calloc(1, sizeof(deprecated_attribute));

        } else {
            printf("      -> Atributo %s não implementado. Lendo %u bytes brutos.\n", attr_name, attributes[i].attribute_length);

            attributes[i].info = malloc(attributes[i].attribute_length);
            if (attributes[i].info == NULL) {
                 fprintf(stderr, "Erro ao alocar info para atributo %s. Pulando.\n", attr_name);
                 fseek(fp, attributes[i].attribute_length, SEEK_CUR);

            } else {
                if (attributes[i].attribute_length > 0) {
                    if (fread(attributes[i].info, sizeof(u1), attributes[i].attribute_length, fp) != attributes[i].attribute_length) {
                        fprintf(stderr, "Erro ao ler info para atributo %s.\n", attr_name);

                        free(attributes[i].info);
                        attributes[i].info = NULL;
                    }

                } else {
                    free(attributes[i].info);
                    attributes[i].info = NULL; 
                }
            }
        }

        free(attr_name);
    }

    return attributes;
}

static field_info* readFields(FILE *file, cp_info *cp, u2 fields_count) {
    if (fields_count == 0) {
        return NULL;
    }

    field_info *fields = calloc(fields_count, sizeof(field_info));
    if (fields == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o array de fields.\n");
        return NULL;
    }

    for (u2 i = 0; i < fields_count; i++) {
        fields[i].access_flags = u2Read(file);
        fields[i].name_index = u2Read(file);
        fields[i].descriptor_index = u2Read(file);
        fields[i].attributes_count = u2Read(file);
        
        fields[i].attributes = readAttributes(file, cp, fields[i].attributes_count);
    }

    return fields;
}

static method_info* readMethods(FILE *file, cp_info *cp, u2 methods_count) {
    if (methods_count == 0) {
        return NULL;
    }

    method_info *methods = calloc(methods_count, sizeof(method_info));
    if (methods == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o array de methods.\n");
        return NULL;
    }

    for (u2 i = 0; i < methods_count; i++) {
        methods[i].access_flags = u2Read(file);
        methods[i].name_index = u2Read(file);
        methods[i].descriptor_index = u2Read(file);
        methods[i].attributes_count = u2Read(file);
        
        methods[i].attributes = readAttributes(file, cp, methods[i].attributes_count);
    }

    return methods;
}


static void freeVerificationTypeInfo(verification_type_info *info) {
    if (info == NULL) return;
   
    free(info);
}

static void freeStackMapFrame(stack_map_frame *frame) {
    if (frame == NULL) return;

    u1 type = frame->frame_type;

    if (type >= 64 && type <= 127) { // SAME_LOCALS_1_STACK_ITEM
        if (frame->map_frame_type.same_locals_1_stack_item_frame.stack != NULL) {
            freeVerificationTypeInfo(frame->map_frame_type.same_locals_1_stack_item_frame.stack[0]);
            
            free(frame->map_frame_type.same_locals_1_stack_item_frame.stack);
        }
    
    } else if (type == 247) { // SAME_LOCALS_1_STACK_ITEM_EXTENDED
        if (frame->map_frame_type.same_locals_1_stack_item_frame_extended.stack != NULL) {
            freeVerificationTypeInfo(frame->map_frame_type.same_locals_1_stack_item_frame_extended.stack[0]);
            
            free(frame->map_frame_type.same_locals_1_stack_item_frame_extended.stack);
        }
    
    } else if (type >= 252 && type <= 254) { // APPEND
        if (frame->map_frame_type.append_frame.locals != NULL) {
            u1 num_locals_to_append = type - 251;
           
            for (int i = 0; i < num_locals_to_append; i++) {
                freeVerificationTypeInfo(frame->map_frame_type.append_frame.locals[i]);
            }
            
            free(frame->map_frame_type.append_frame.locals);
        }
    
    } else if (type == 255) { // FULL_FRAME
        u2 num_locals = frame->map_frame_type.full_frame.number_of_locals;
        
        if (frame->map_frame_type.full_frame.locals != NULL) {
            
            for (int i = 0; i < num_locals; i++) {
                freeVerificationTypeInfo(frame->map_frame_type.full_frame.locals[i]);
            
            }
            
            free(frame->map_frame_type.full_frame.locals);
        }
        
        u2 num_stack = frame->map_frame_type.full_frame.number_of_stack_items;
        if (frame->map_frame_type.full_frame.stack != NULL) {
           
            for (int i = 0; i < num_stack; i++) {
                freeVerificationTypeInfo(frame->map_frame_type.full_frame.stack[i]);
            }
            
            free(frame->map_frame_type.full_frame.stack);
        }
    }

    free(frame);
}

static void freeStackMapTable(stackMapTable_attribute *table) {
    if (table == NULL) return;
    
    if (table->entries != NULL) {
        
        for (u2 i = 0; i < table->number_of_entries; i++) {
            freeStackMapFrame(table->entries[i]);
        }

        free(table->entries);
    }
}

static void freeBootstrapMethods(bootstrap_methods_attribute *bsm_attr) {
    if (bsm_attr == NULL) return;

    if (bsm_attr->bootstrap_methods != NULL) {
        for (u2 i = 0; i < bsm_attr->num_bootstrap_methods; i++) {

            if (bsm_attr->bootstrap_methods[i].bootstrap_arguments != NULL) {
                free(bsm_attr->bootstrap_methods[i].bootstrap_arguments);
            }
        }

        free(bsm_attr->bootstrap_methods);
    }
}

static void freeAttributes(attribute_info *attributes, u2 attributes_count, cp_info *cp) {
    if (attributes == NULL || attributes_count == 0 || cp == NULL) {
        return;
    }

    for (u2 i = 0; i < attributes_count; i++) {
        if (attributes[i].info != NULL) {
            
            char *attr_name = getUtf8StringFromCp(cp, attributes[i].attribute_name_index);
            
            if (attr_name != NULL) {
                
                if (strcmp(attr_name, "Code") == 0) {
                    code_attribute *code_attr = (code_attribute*)attributes[i].info;
                    
                    if (code_attr != NULL) {
                        free(code_attr->code);
                        free(code_attr->exception_table);

                        freeAttributes(code_attr->attributes, code_attr->attributes_count, cp);
                    }

                } else if (strcmp(attr_name, "Exceptions") == 0) {
                    exceptions_attribute *ex_attr = (exceptions_attribute*)attributes[i].info;
                    
                    if (ex_attr != NULL) {
                        free(ex_attr->exception_index_table);
                    }

                } else if (strcmp(attr_name, "LineNumberTable") == 0) {
                    lineNumberTable_attribute *ln_attr = (lineNumberTable_attribute*)attributes[i].info;
                    
                    if (ln_attr != NULL) {
                        free(ln_attr->line_number_table_vec);
                    }

                } else if (strcmp(attr_name, "InnerClasses") == 0) {
                    innerClasses_attribute *ic_attr = (innerClasses_attribute*)attributes[i].info;
                    
                    if (ic_attr != NULL) {
                        free(ic_attr->classes_vec);
                    }
                
                } else if (strcmp(attr_name, "StackMapTable") == 0) {
                    freeStackMapTable((stackMapTable_attribute*)attributes[i].info);
                
                } else if (strcmp(attr_name, "BootstrapMethods") == 0) {
                    freeBootstrapMethods((bootstrap_methods_attribute*)attributes[i].info);

                } else if (strcmp(attr_name, "Synthetic") == 0) {
                    // Nada a fazer, apenas um marcador.                
                } else if (strcmp(attr_name, "Deprecated") == 0) {
                    // Nada a fazer, apenas um marcador.                
                }
           
                free(attr_name);
            }
            
            free(attributes[i].info); 
        }
    }
    
    free(attributes);
}


void freeClassFile(ClassFile *cf) {
    if (cf == NULL) return;
    
    if (cf->interfaces != NULL) {
        free(cf->interfaces);
    }

    if (cf->fields != NULL) {
        for (u2 i = 0; i < cf->fields_count; i++) {
            freeAttributes(cf->fields[i].attributes, cf->fields[i].attributes_count, cf->constant_pool);
        }

        free(cf->fields);
    }

    if (cf->methods != NULL) {
        for (u2 i = 0; i < cf->methods_count; i++) {
            freeAttributes(cf->methods[i].attributes, cf->methods[i].attributes_count, cf->constant_pool);
        }
        
        free(cf->methods);
    }

    if (cf->attributes != NULL) {
        freeAttributes(cf->attributes, cf->attributes_count, cf->constant_pool);
    }

    if (cf->constant_pool != NULL) {
        for(int i = 0; i < cf->constant_pool_count - 1; i++) {
            
            if (cf->constant_pool[i].tag == CONSTANT_Utf8) {
                free(cf->constant_pool[i].info.utf8_info.bytes);
            }
        }

        free(cf->constant_pool); 
    }

    free(cf);
}
