#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory_manager.h"

void initJVM(JVM *jvm) {
    jvm->heap = (u1*) calloc(HEAP_SIZE, sizeof(u1));
    if (jvm->heap == NULL) {
        fprintf(stderr, "Erro: Não foi possível alocar a Heap para a JVM.\n");
        exit(-1);
    }

    jvm->heap_ptr = 0;
    jvm->current_frame = NULL;

    jvm->method_area = (ClassFile**) calloc(128, sizeof(ClassFile*));
    jvm->method_area_count = 0;

    jvm->objects = (Object**) calloc(MAX_OBJECTS, sizeof(Object*));
    jvm->objects_count = 1;
}

void freeJVM(JVM *jvm) {
    if (jvm->heap) {
        free(jvm->heap);
    }

    if (jvm->method_area) {
        free(jvm->method_area);
    }

    if (jvm->objects) {
        for(u4 i = 1; i < jvm->objects_count; i++) {
            if (jvm->objects[i]) free(jvm->objects[i]);
        }
        free(jvm->objects);
    }
}


u1* allocHeap(JVM *jvm, u4 size) {
    if (jvm->heap_ptr + size > HEAP_SIZE) {
        fprintf(stderr, "Erro: Heap cheia!\n");
        exit(-1);
    }

    u1* address = jvm->heap + jvm->heap_ptr;
    jvm->heap_ptr += size;

    memset(address, 0, size);

    return address;
}

u4 registerObject(JVM *jvm, Object *obj) {
    if (jvm->objects_count >= MAX_OBJECTS) {
        fprintf(stderr, "Erro: Limite de objetos atingido!\n");
        exit(-1);
    }
    u4 index = jvm->objects_count++;
    jvm->objects[index] = obj;
    return index;
}

Object* createObject(JVM *jvm, u4 class_index, u4 fields_size) {
    Object *obj = (Object*) calloc(1, sizeof(Object));
    if (obj == NULL) {
        fprintf(stderr, "Erro: Não foi possível alocar um objeto.\n");
        exit(-1);
    }

    obj->class_index = class_index;
    obj->fields_size = fields_size * 4;
    obj->data = allocHeap(jvm, obj->fields_size);

    return obj;
}

Array* createArray(JVM *jvm, u4 length, u1 type, u1 element_size) {
    Array *arr = (Array*) calloc(1, sizeof(Array));
    if (arr == NULL) {
        fprintf(stderr, "Erro: Não foi possível alocar o array\n");
        exit(-1);
    }

    arr->type = type;
    arr->length = length;

    u4 total_size = length * element_size;
    arr->data = allocHeap(jvm, total_size);

    return arr;
}

void loadClass(JVM *jvm, ClassFile *cf) {
    jvm->method_area[jvm->method_area_count] = cf;
    jvm->method_area_count++;
}

ClassFile* getClass(JVM *jvm, u4 index) {
    if (index >= jvm->method_area_count) {
        return NULL;
    }

    return jvm->method_area[index];
}
