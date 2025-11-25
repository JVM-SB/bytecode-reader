#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "jvm_structures.h"


// Gerenciamento de Memória da JVM
void initJVM(JVM *jvm);
void freeJVM(JVM *jvm);


//  Heap
u1* allocHeap(JVM *jvm, u4 size);


//  Objetos e Arrays
Object* createObject(JVM *jvm, u4 class_index, u4 fields_size);
Array* createArray(JVM *jvm, u4 length, u1 type, u1 element_size);


// Carregador de Classes
void loadClass(JVM *jvm, ClassFile *cf);
ClassFile* getClass(JVM *jvm, u4 class_index);

#endif // MEMORY_MANAGER_H
