#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "helpers.h"

int countArguments(const char *descriptor) {
    int count = 0;
    int i = 1; // Pula o '('
    while (descriptor[i] != ')') {
        if (descriptor[i] == 'L') {
            // Objetos (Ljava/lang/String;)
            count++;
            while (descriptor[i] != ';') i++;
        } else if (descriptor[i] == '[') {
            // Arrays
            while (descriptor[i] == '[') i++;
            if (descriptor[i] == 'L') while (descriptor[i] != ';') i++;
            count++;
        } else if (descriptor[i] == 'J' || descriptor[i] == 'D') {
            // Long e Double contam como 2 slots na pilha
            count += 2;
        } else {
            // Tipos primitivos (I, F, B, C, S, Z)
            count++;
        }
        i++;
    }
    return count;
}

int constantPoolStringIsEqual(const ClassFile *cf, u2 index, const char *str) {
    if (index == 0 || index >= cf->constant_pool_count) {
        fprintf(stderr, "Erro: Índice inválido na constant pool.\n");
        exit(-1);
    }

    cp_info *entry = &cf->constant_pool[index-1];
    if (entry->tag != CONSTANT_Utf8) {
        return 0;
    }

    u2 length = entry->info.utf8_info.length;
    const char *entryStr = (const char *) entry->info.utf8_info.bytes;

    if (strlen(str) != length) {
        return 0;
    }

    return (strncmp(entryStr, str, length) == 0);
}

method_info* findMethod(const ClassFile *cf, const char *name, const char *descriptor) {
     for (int i = 0; i < cf->methods_count; i++) {
        method_info *method = &cf->methods[i];

        if(constantPoolStringIsEqual(cf, method->name_index, name ) &&
           constantPoolStringIsEqual(cf, method->descriptor_index, descriptor)) {
            return method;
        }
    }
    return NULL;
}

code_attribute* findCodeAttribute(const ClassFile *cf, method_info *method) {
    for (int i = 0; i < method->attributes_count; i++) {
        attribute_info *attribute = &method->attributes[i];

        if(constantPoolStringIsEqual(cf, attribute->attribute_name_index, "Code")) {
            return (code_attribute*) attribute->info;
        }
    }
    return NULL;
}
