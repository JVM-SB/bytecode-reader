#include <stdlib.h>

#include "reader.h"

// Leitores de Inteiros
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


// Leitor do arquivo de classe
ClassFile* readFile(const char *filename) {
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
    cf->constant_pool_count = u2Read(file);

    cf->constant_pool = readConstantPool(file, cf->constant_pool_count);
    if (cf->constant_pool == NULL) {
        free(cf);
        fclose(file);
        return NULL;
    }

    cf->access_flags = u2Read(file);
    cf->this_class = u2Read(file);
    cf->super_class = u2Read(file);
    
    cf->interfaces_count = u2Read(file);
    // TODO:
    // cf->interfaces = readInterfaces(file, cf->interfaces_count); 

    cf->fields_count = u2Read(file);
    // TODO:
    // cf->fields = readFields(file, cf->constant_pool, cf->fields_count);
    
    cf->methods_count = u2Read(file);
    // TODO:
    // cf->methods = readMethods(file, cf->constant_pool, cf->methods_count);

    cf->attributes_count = u2Read(file);
    // TODO:
    // cf->attributes = readAttributes(file, cf->constant_pool, cf->attributes_count);

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

void freeClassFile(ClassFile *cf) {
    if (cf == NULL) return;
    
    // TODO: Liberar todos os campos internos (ex: constant_pool, fields, etc.)
    if (cf->constant_pool != NULL) {
        for(int i = 0; i < cf->constant_pool_count - 1; i++) {
            if (cf->constant_pool[i].tag == CONSTANT_Utf8) {
                free(cf->constant_pool[i].info.utf8_info.bytes);
            }
        }
        free(cf->constant_pool);
    }
    
    // TODO: Liberar cf->interfaces, cf->fields (e seus atributos), 
    // cf->methods (e seus atributos), cf->attributes
    
    free(cf); // Liberar a struct principal
}
