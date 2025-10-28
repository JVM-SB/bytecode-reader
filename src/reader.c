#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "reader.h"

// Leitores de Inteiros e Funções auxiliares
static u1 u1Read(FILE *file) {
    return getc(file);
}

static u2 u2Read(FILE *file) {
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

// Leitor do arquivo de classe
ClassFile* readFile(const char *filename) {
    printf("--- Lendo arquivo: %s ---\n", filename);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);

        return NULL;
    }

    struct ClassFile *cf = malloc(sizeof(struct ClassFile));
    if (cf == NULL) {
        fclose(file);
        fprintf(stderr, "Erro ao alocar ClassFile.\n");

        return NULL;
    }

    cf->magic = u4Read(file);
    if (cf->magic != JAVA_CLASS_MAGIC) {
        fprintf(stderr, "Erro Magic Number inválido (%x) esperado 0xCAFEBABE.\n", cf->magic);
        free(cf);
        fclose(file);

        return NULL;
    }

    cf->minor_version = u2Read(file);
    cf->major_version = u2Read(file);
    printf("Version: %u.%u (Major %u, Minor %u)\n", cf->major_version, cf->minor_version, cf->major_version, cf->minor_version);

    cf->constant_pool_count = u2Read(file);
    printf("Constant Pool Count: %u\n", cf->constant_pool_count);
    

    cf->constant_pool = readConstantPool(file, cf->constant_pool_count);
    if (cf->constant_pool == NULL) {
        free(cf);
        fclose(file);
        return NULL;
    }

    cf->access_flags = u2Read(file);
    printf("Access Flags: 0x%04X\n", cf->access_flags);
    cf->this_class = u2Read(file);
    printf("This Class (CP Index): #%u\n", cf->this_class);

    cf->super_class = u2Read(file);
    printf("Super Class (CP Index): #%u\n", cf->super_class);
    
    cf->interfaces_count = u2Read(file);
    printf("Interfaces Count: %u\n", cf->interfaces_count);
    cf->interfaces = readInterfaces(file, cf->interfaces_count); 

    cf->fields_count = u2Read(file);
    printf("Fields Count: %u\n", cf->fields_count);
    cf->fields = readFields(file, cf->constant_pool, cf->fields_count);
    
    cf->methods_count = u2Read(file);
    printf("Methods Count: %u\n", cf->methods_count);
    cf->methods = readMethods(file, cf->constant_pool, cf->methods_count);

    cf->attributes_count = u2Read(file);
    printf("Class Attributes Count: %u\n", cf->attributes_count);
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

u2* readInterfaces(FILE *file, u2 interfaces_count) {
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
        printf("  Interface #%u: CP Index %u\n", i + 1, interfaces[i]);
    }

    return interfaces;
}

exception_table* readExceptionTable(FILE *fp, u2 size) {
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

code_attribute* readCode(FILE *fp, cp_info *cp) {
    code_attribute *code_attr = malloc(sizeof(code_attribute));
    if (code_attr == NULL) {
        fprintf(stderr, "Erro ao alocar code_attribute.\n");
        return NULL;
    }

    code_attr->max_stack = u2Read(fp);
    code_attr->max_locals = u2Read(fp);
    code_attr->code_length = u4Read(fp);
    printf("      -> Lendo Code: max_stack=%u, max_locals=%u, code_length=%u\n", 
           code_attr->max_stack, code_attr->max_locals, code_attr->code_length);

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
    if(code_attr->exception_table_length > 0)
        printf("      -> Code: exception_table_length=%u\n", code_attr->exception_table_length);
    code_attr->exception_table = readExceptionTable(fp, code_attr->exception_table_length);

    code_attr->attributes_count = u2Read(fp);
    if(code_attr->attributes_count > 0)
        printf("      -> Code: attributes_count=%u\n", code_attr->attributes_count);
    
    code_attr->attributes = readAttributes(fp, cp, code_attr->attributes_count); 

    return code_attr;
}

constantValue_attribute* readConstantValue(FILE *fp) {
    constantValue_attribute *cv_attr = malloc(sizeof(constantValue_attribute));
    if (cv_attr == NULL) {
         fprintf(stderr, "Erro ao alocar constantValue_attribute.\n");
         return NULL;
    }
    cv_attr->constantvalue_index = u2Read(fp);
    printf("      -> Lendo ConstantValue: constantvalue_index=#%u\n", cv_attr->constantvalue_index);
    return cv_attr;
}

sourceFile_attribute* readSourceFile(FILE *fp) {
     sourceFile_attribute *sf_attr = malloc(sizeof(sourceFile_attribute));
     if (sf_attr == NULL) {
         fprintf(stderr, "Erro ao alocar sourceFile_attribute.\n");
         return NULL;
     }
     sf_attr->sourcefile_index = u2Read(fp);
     printf("      -> Lendo SourceFile: sourcefile_index=#%u\n", sf_attr->sourcefile_index);
     return sf_attr;
}

exceptions_attribute* readExceptionsAttribute(FILE *fp) {
    exceptions_attribute *ex_attr = malloc(sizeof(exceptions_attribute));
    if (ex_attr == NULL) {
        fprintf(stderr, "Erro ao alocar exceptions_attribute.\n");
        return NULL;
    }
    ex_attr->number_of_exceptions = u2Read(fp);
    printf("      -> Lendo Exceptions: number_of_exceptions=%u\n", ex_attr->number_of_exceptions);

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

lineNumberTable_attribute* readLineNumberTableAttribute(FILE *fp) {
    lineNumberTable_attribute *ln_attr = malloc(sizeof(lineNumberTable_attribute));
    if (ln_attr == NULL) {
         fprintf(stderr, "Erro ao alocar lineNumberTable_attribute.\n");
         return NULL;
    }

    ln_attr->line_number_table_length = u2Read(fp);
    printf("      -> Lendo LineNumberTable: line_number_table_length=%u\n", ln_attr->line_number_table_length);

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
            printf("        -> start_pc=%u\n", ln_attr->line_number_table_vec[i].start_pc);
            ln_attr->line_number_table_vec[i].line_number = u2Read(fp);
            printf("        -> line_number=%u\n", ln_attr->line_number_table_vec[i].line_number);
        }
    } else {
        ln_attr->line_number_table_vec = NULL;
    }
    return ln_attr;
}

innerClasses_attribute* readInnerClasses(FILE *fp) {
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
    printf("      -> Lendo InnerClasses: number_of_classes=%u\n", ic_attr->number_of_classes);
    for (u2 i = 0; i < ic_attr->number_of_classes; i++) {
        printf("        Lendo InnerClass [%u]:\n", i);
        ic_attr->classes_vec[i].inner_class_info_index = u2Read(fp);
        printf("          -> inner_class_info_index=#%u\n", ic_attr->classes_vec[i].inner_class_info_index);
        ic_attr->classes_vec[i].outer_class_info_index = u2Read(fp);
        printf("          -> outer_class_info_index=#%u\n", ic_attr->classes_vec[i].outer_class_info_index);
        ic_attr->classes_vec[i].inner_name_index = u2Read(fp);
        printf("          -> inner_name_index=#%u\n", ic_attr->classes_vec[i].inner_name_index);
        ic_attr->classes_vec[i].inner_class_access_flags = u2Read(fp);
        printf("          -> inner_class_access_flags=0x%04X\n", ic_attr->classes_vec[i].inner_class_access_flags);
    }
    return ic_attr;
}

attribute_info* readAttributes(FILE *fp, cp_info *cp, u2 attributes_count) {
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

        printf("    Lendo Atributo [%u]: %s (name_index=#%u), length=%u\n", 
               i, attr_name, attributes[i].attribute_name_index, attributes[i].attribute_length);

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
        }
        // TODO: Adicionar 'else if' para StackMapTable, etc.
        else {
            // Atributo desconhecido.
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

field_info* readFields(FILE *file, cp_info *cp, u2 fields_count) {
    if (fields_count == 0) {
        return NULL;
    }

    field_info *fields = calloc(fields_count, sizeof(field_info));
    if (fields == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o array de fields.\n");
        return NULL;
    }

    printf("  Lendo %u fields...\n", fields_count);

    for (u2 i = 0; i < fields_count; i++) {
        fields[i].access_flags = u2Read(file);
        fields[i].name_index = u2Read(file);
        fields[i].descriptor_index = u2Read(file);
        fields[i].attributes_count = u2Read(file);
        
        printf("    Field [%u]: name_index=#%u, descriptor_index=#%u, flags=0x%04X, attributes_count=%u\n", 
               i, fields[i].name_index, fields[i].descriptor_index, fields[i].access_flags, fields[i].attributes_count);

        fields[i].attributes = readAttributes(file, cp, fields[i].attributes_count);
    }
    return fields;
}

method_info* readMethods(FILE *file, cp_info *cp, u2 methods_count) {
    if (methods_count == 0) {
        return NULL;
    }

    method_info *methods = calloc(methods_count, sizeof(method_info));
    if (methods == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o array de methods.\n");
        return NULL;
    }

    printf("  Lendo %u methods...\n", methods_count);

    for (u2 i = 0; i < methods_count; i++) {
        methods[i].access_flags = u2Read(file);
        methods[i].name_index = u2Read(file);
        methods[i].descriptor_index = u2Read(file);
        methods[i].attributes_count = u2Read(file);
        
        printf("    Method [%u]: name_index=#%u, descriptor_index=#%u, flags=0x%04X, attributes_count=%u\n", 
               i, methods[i].name_index, methods[i].descriptor_index, methods[i].access_flags, methods[i].attributes_count);

        methods[i].attributes = readAttributes(file, cp, methods[i].attributes_count);
    }
    return methods;
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
                }
                else if (strcmp(attr_name, "LineNumberTable") == 0) {
                    lineNumberTable_attribute *ln_attr = (lineNumberTable_attribute*)attributes[i].info;
                    if (ln_attr != NULL) {
                        free(ln_attr->line_number_table_vec); // Libera o array da tabela
                    }
                }
                else if (strcmp(attr_name, "InnerClasses") == 0) {
                    innerClasses_attribute *ic_attr = (innerClasses_attribute*)attributes[i].info;
                    if (ic_attr != NULL) {
                        free(ic_attr->classes_vec); // Libera o array de classes
                    }
                }
                // TODO: Adicionar liberações para os outros tipos de atributos
                free(attr_name);
            }
            free(attributes[i].info); 
        }
    }
    free(attributes);
}

void freeClassFile(ClassFile *cf) {
    if (cf == NULL) return;

    // Liberar Constant Pool
    if (cf->constant_pool != NULL) {
        for(int i = 0; i < cf->constant_pool_count - 1; i++) {
            if (cf->constant_pool[i].tag == CONSTANT_Utf8) {
                free(cf->constant_pool[i].info.utf8_info.bytes);
            }
        }
        free(cf->constant_pool);
    }
    
    // Liberar Interfaces
    if (cf->interfaces != NULL) {
        free(cf->interfaces);
    }

    // Liberar Fields e seus atributos
    if (cf->fields != NULL) {
        for (u2 i = 0; i < cf->fields_count; i++) {
            freeAttributes(cf->fields[i].attributes, cf->fields[i].attributes_count, cf->constant_pool);
        }
        free(cf->fields);
    }

    // Liberar Methods e seus atributos
    if (cf->methods != NULL) {
        for (u2 i = 0; i < cf->methods_count; i++) {
            freeAttributes(cf->methods[i].attributes, cf->methods[i].attributes_count, cf->constant_pool);
        }
        free(cf->methods);
    }

    // Liberar Attributes da ClassFile
    if (cf->attributes != NULL) {
        freeAttributes(cf->attributes, cf->attributes_count, cf->constant_pool);
    }

    free(cf); // Liberar a struct principal
}
