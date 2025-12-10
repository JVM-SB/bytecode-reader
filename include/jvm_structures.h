#ifndef JVM_STRUCTURES_H
#define JVM_STRUCTURES_H

#include "structures.h"
#include "classfile.h"

// Configurações da JVM
#define MAX_STACK_DEPTH 1024            // Maximo de chamadas de método
#define MAX_OP_STACK 256                // Tamanho da pilha de operandos por frame
#define MAX_LOCALS 256                  // Máximo de variáveis locais
#define HEAP_SIZE 32 * 1024 * 1024      // 32MB de Heap
#define MAX_OBJECTS 10000             // Máximo de objetos na heap

// Tipos de Referência
typedef struct Object {
    u4 class_index;             // Index na área de métodos 
    u4 fields_size;             // Tamanho do array de dados
    u1 *data;                   // Dados dos campos do objeto
} Object;

typedef struct Array {
    u1 type;                    // Tipo do array (int, float, ref, etc)
    u4 length;                  // Tamanho do array
    u1 *data;                   // Dados do array
} Array;


// Estrutura do Frame da JVM
typedef struct Frame {
    u4 pc;
    u1 *bytecode;                   // Array de bytecodes do método
    u4 code_length;

    u4 local_variables[MAX_LOCALS];
    u4 operand_stack[MAX_OP_STACK];
    u4 stack_pointer;                   // Índice do topo da pilha

    ClassFile *cf_ref;              // Ponteiro para o ClassFile
    struct Frame *previous;         // Ponteiro para o frame anterior
    struct JVM *jvm_ref;
} Frame;


// Estrutura da JVM
typedef struct JVM {
    Frame *current_frame;
    u1 *heap;
    u4 heap_ptr;

    ClassFile **method_area;
    u4 method_area_count;

    Object **objects;
    u4 objects_count;
} JVM;

#endif // JVM_STRUCTURES_H
