#include <stdlib.h>

#include "reader.h"

// Leitores de Inteiros
u1 u1Read(FILE *file) {
    return getc(file);
}

u2 u2Read(FILE *file) {
    u2 toReturn = getc(file);

    toReturn = (toReturn << 8) | getc(file);
    
    return toReturn;
}

u4 u4Read(FILE *file) {
    u4 toReturn = getc(file);

    toReturn = (toReturn << 8) | getc(file);
    toReturn = (toReturn << 8) | getc(file);
    toReturn = (toReturn << 8) | getc(file);

    return toReturn;
}


// Leitor do arquivo de classe
ClassFile* readFile(char *filename) {
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

    cf->constant_pool = readConstantPoolInternal(file, cf->constant_pool_count);
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
