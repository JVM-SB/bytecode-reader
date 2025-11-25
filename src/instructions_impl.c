#include <stdio.h>
#include <stdlib.h>

#include "instructions_impl.h"

#define READ_U1(f) (f->bytecode[f->pc++])
#define READ_U2(f) (f->pc += 2, (f->bytecode[f->pc-2] << 8) | f->bytecode[f->pc-1])

void nop_impl(Frame *frame) {
    (void) frame;
    return;
}

// Placeholder
void unimpl(Frame *frame) {
    printf("Instrucao nao implementada (PC: %u)\n", frame->pc - 1);
    exit(-1);
}

// CONSTANTES 
void aconst_null_impl(Frame *frame) {
    pushOperand(frame, 0);
}

void iconst_m1_impl(Frame *frame) { 
    pushOperand(frame, -1);
}

void iconst_0_impl(Frame *frame)  { 
    pushOperand(frame, 0);
}

void iconst_1_impl(Frame *frame)  { 
    pushOperand(frame, 1);
}

void iconst_2_impl(Frame *frame)  { 
    pushOperand(frame, 2);
}

void iconst_3_impl(Frame *frame)  { 
    pushOperand(frame, 3);
}

void iconst_4_impl(Frame *frame)  { 
    pushOperand(frame, 4);
}

void iconst_5_impl(Frame *frame)  { 
    pushOperand(frame, 5);
}

void lconst_0_impl(Frame *frame) {
    pushOperand(frame, 0);          // High
    pushOperand(frame, 0);          // Low
}

void lconst_1_impl(Frame *frame) {
    pushOperand(frame, 0);
    pushOperand(frame, 1);
}

void bipush_impl(Frame *frame) {
    int8_t byte = (int8_t) READ_U1(frame);
    pushOperand(frame, (u4)byte);

    printf("BIPUSH FEITO\n");
}

void sipush_impl(Frame *frame) {
    int16_t sho = (int16_t) READ_U2(frame);
    pushOperand(frame, (u4)sho);
}


// LOADS
void iload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);
}

void iload_0_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[0]);
}

void iload_1_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[1]);

    printf("ILOAD_1 FEITO \n");
}

void iload_2_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[2]);
}

void iload_3_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[3]);
}

void lload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);      // High
    pushOperand(frame, frame->local_variables[index + 1]);  // Low
}

void lload_0_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[0]);
    pushOperand(frame, frame->local_variables[1]);
}

void lload_1_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[1]);
    pushOperand(frame, frame->local_variables[2]);
}

void lload_2_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[2]);
    pushOperand(frame, frame->local_variables[3]);
}

void lload_3_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[3]);
    pushOperand(frame, frame->local_variables[4]);
}


// STORES
void istore_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    frame->local_variables[index] = popOperand(frame);
}

void istore_0_impl(Frame *frame) {
    frame->local_variables[0] = popOperand(frame);
}

void istore_1_impl(Frame *frame) {
    frame->local_variables[1] = popOperand(frame);
}

void istore_2_impl(Frame *frame) {
    frame->local_variables[2] = popOperand(frame);
}

void istore_3_impl(Frame *frame) {
    frame->local_variables[3] = popOperand(frame);
}

void lstore_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    frame->local_variables[index + 1] = popOperand(frame); // Low
    frame->local_variables[index]     = popOperand(frame); // High
}

void lstore_0_impl(Frame *frame) {
    frame->local_variables[1] = popOperand(frame);
    frame->local_variables[0] = popOperand(frame);
}

void lstore_1_impl(Frame *frame) {
    frame->local_variables[2] = popOperand(frame);
    frame->local_variables[1] = popOperand(frame);
}

void lstore_2_impl(Frame *frame) {
    frame->local_variables[3] = popOperand(frame);
    frame->local_variables[2] = popOperand(frame);
}

void lstore_3_impl(Frame *frame) {
    frame->local_variables[4] = popOperand(frame);
    frame->local_variables[3] = popOperand(frame);
}


// Aritmética (int)
void iadd_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    u4 result = value1 + value2;

    pushOperand(frame, result);

    printf("IADD FEITO: %u + %u = %u", value1, value2, result);
}

void isub_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    u4 result = value1 - value2;

    pushOperand(frame, result);

    printf("ISUB FEITO: %u - %u = %u", value1, value2, result);
}

void imul_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    u4 result = value1 * value2;

    pushOperand(frame, result);
}

void idiv_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);
    
    if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u4 result = value1 / value2;
    
    pushOperand(frame, result);
}

void irem_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u4 result = value1 % value2;

    pushOperand(frame, result);
}

void ineg_impl(Frame *frame) {
    u4 value = (u4) popOperand(frame);

    u4 result = -value;

    pushOperand(frame, result);
}

// iinc
void iinc_impl(Frame *frame) {
    u1 index = READ_U1(frame);                  // Índice da variável local
    int8_t constant = (int8_t) READ_U1(frame);  // Valor a somar

    u4 value = (u4) frame->local_variables[index];
    value += constant;

    frame->local_variables[index] = (u4) value;
}

// LONGS
static u8 popLong(Frame *frame) {
    u4 low = popOperand(frame);
    u4 high = popOperand(frame);

    return ((u8)high << 32) | low;
}

static void pushLong(Frame *frame, u8 value) {
    u4 high = (u4)(value >> 32);
    u4 low = (u4)(value & 0xFFFFFFFF);

    pushOperand(frame, high);
    pushOperand(frame, low);
}

void ladd_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

    u8 result = value1 + value2;
    
    pushLong(frame, result);
}

void lsub_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

    u8 result = value1 - value2;

    pushLong(frame, result);
}

void lmul_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

    u8 result = value1 * value2;

    pushLong(frame, result);
}

void ldiv_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

     if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u8 result = value1 / value2;

    pushLong(frame, result);
}

void lrem_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

     if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u8 result = value1 % value2;

    pushLong(frame, result);
}

void lneg_impl(Frame *frame) {
    u8 value = popLong(frame);

    u8 result = -value;

    pushLong(frame, result);
}



// Return void
void return_impl(Frame *frame) {
    JVM *jvm = frame->jvm_ref;

    Frame *prev_frame = frame->previous;
    jvm->current_frame = prev_frame;

    deleteFrame(frame);
}


// Return com valor inteiro
void ireturn_impl(Frame *frame) {
    u4 return_value = popOperand(frame);

    JVM *jvm = frame->jvm_ref;
    Frame * prev_frame = frame->previous;

    if (prev_frame != NULL) {
        pushOperand(prev_frame, return_value);
    }

    jvm->current_frame = prev_frame;
    deleteFrame(frame);
}
