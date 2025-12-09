#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <helpers.h>
#include "instructions_impl.h"

#define READ_U1(f) (f->bytecode[f->pc++])
#define READ_U2(f) (f->pc += 2, (f->bytecode[f->pc-2] << 8) | f->bytecode[f->pc-1])

void nop_impl(Frame *frame) {
    (void) frame;
    return;
}

// Placeholder
void unimpl(Frame *frame) {
    u1 opcode = frame->bytecode[frame->pc - 1]; 
    printf("Instrucao nao implementada (PC: %u, Opcode: 0x%02x)\n", frame->pc - 1, opcode);
    // printf("Instrucao nao implementada (PC: %u)\n", frame->pc - 1);
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

void fconst_0_impl(Frame *frame) {
    float value = 0.0f;
    u4 intBits;
    memcpy(&intBits, &value, sizeof(u4));
    pushOperand(frame, intBits);
}

void fconst_1_impl(Frame *frame) {
    float value = 1.0f;
    u4 intBits;
    memcpy(&intBits, &value, sizeof(u4));
    pushOperand(frame, intBits);
}

void fconst_2_impl(Frame *frame) {
    float value = 2.0f;
    u4 intBits;
    memcpy(&intBits, &value, sizeof(u4));
    pushOperand(frame, intBits);
}

void dconst_0_impl(Frame *frame) {
    double value = 0.0;
    u4 highBits, lowBits;
    u8 longBits;
    memcpy(&longBits, &value, sizeof(u8));
    highBits = (u4)(longBits >> 32);
    lowBits = (u4)(longBits & 0xFFFFFFFF);
    pushOperand(frame, highBits);
    pushOperand(frame, lowBits);
}

void dconst_1_impl(Frame *frame) {
    double value = 1.0;
    u4 highBits, lowBits;
    u8 longBits;
    memcpy(&longBits, &value, sizeof(u8));
    highBits = (u4)(longBits >> 32);
    lowBits = (u4)(longBits & 0xFFFFFFFF);
    pushOperand(frame, highBits);
    pushOperand(frame, lowBits);
}

void bipush_impl(Frame *frame) {
    int8_t byte = (int8_t) READ_U1(frame);
    pushOperand(frame, (u4)byte);
}

void sipush_impl(Frame *frame) {
    int16_t sho = (int16_t) READ_U2(frame);
    pushOperand(frame, (u4)sho);
}


// LOADS

// Inteiros
void iload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);
}

void iload_0_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[0]);
}

void iload_1_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[1]);
}

void iload_2_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[2]);
}

void iload_3_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[3]);
}


// Float
void fload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);
}
void fload_0_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[0]);
}
void fload_1_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[1]);
}
void fload_2_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[2]);
}
void fload_3_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[3]);
}


// Long
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


// Double
void dload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);      // High
    pushOperand(frame, frame->local_variables[index + 1]);  // Low
}

void dload_0_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[0]);
    pushOperand(frame, frame->local_variables[1]);
}

void dload_1_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[1]);
    pushOperand(frame, frame->local_variables[2]);
}

void dload_2_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[2]);
    pushOperand(frame, frame->local_variables[3]);
}

void dload_3_impl(Frame *frame) {
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

void dstore_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    frame->local_variables[index + 1] = popOperand(frame); // Low
    frame->local_variables[index]     = popOperand(frame); // High
}

void dstore_0_impl(Frame *frame) {
    frame->local_variables[1] = popOperand(frame);
    frame->local_variables[0] = popOperand(frame);
}

void dstore_1_impl(Frame *frame) {
    frame->local_variables[2] = popOperand(frame);
    frame->local_variables[1] = popOperand(frame);
}

void dstore_2_impl(Frame *frame) {
    frame->local_variables[3] = popOperand(frame);
    frame->local_variables[2] = popOperand(frame);
}

void dstore_3_impl(Frame *frame) {
    frame->local_variables[4] = popOperand(frame);
    frame->local_variables[3] = popOperand(frame);
}

void fstore_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    frame->local_variables[index] = popOperand(frame);
}
void fstore_0_impl(Frame *frame) {
    frame->local_variables[0] = popOperand(frame);
}
void fstore_1_impl(Frame *frame) {
    frame->local_variables[1] = popOperand(frame);
}
void fstore_2_impl(Frame *frame) {
    frame->local_variables[2] = popOperand(frame);
}
void fstore_3_impl(Frame *frame) {
    frame->local_variables[3] = popOperand(frame);
}

// Aritmética (int)
void iadd_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    u4 result = value1 + value2;

    pushOperand(frame, result);
}

void isub_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    u4 result = value1 - value2;

    pushOperand(frame, result);
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

// POP e PUSH para long (u8)
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

// Aritmética (double)
void dadd_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);
    double dvalue1 = *(double*)&value1;
    double dvalue2 = *(double*)&value2;
    double dresult = dvalue1 + dvalue2;
    u8 result = *(u8*)&dresult;
    pushLong(frame, result);
}

void dsub_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);
    double dvalue1 = *(double*)&value1;
    double dvalue2 = *(double*)&value2;
    double dresult = dvalue1 - dvalue2;
    u8 result = *(u8*)&dresult;
    pushLong(frame, result);
}

void dmul_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);
    double dvalue1 = *(double*)&value1;
    double dvalue2 = *(double*)&value2;
    double dresult = dvalue1 * dvalue2;
    u8 result = *(u8*)&dresult;
    pushLong(frame, result);
}

void ddiv_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);
    double dvalue1 = *(double*)&value1;
    double dvalue2 = *(double*)&value2;

    if (dvalue2 == 0.0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0.0)\n");
        exit(1);
    }

    double dresult = dvalue1 / dvalue2;
    u8 result = *(u8*)&dresult;
    pushLong(frame, result);
}

void drem_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);
    double dvalue1 = *(double*)&value1;
    double dvalue2 = *(double*)&value2;

    if (dvalue2 == 0.0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0.0)\n");
        exit(1);
    }

    double dresult = fmod(dvalue1, dvalue2);
    u8 result = *(u8*)&dresult;
    pushLong(frame, result);
}

void dneg_impl(Frame *frame) {
    u8 value = popLong(frame);
    double dvalue = *(double*)&value;
    double dresult = -dvalue;
    u8 result = *(u8*)&dresult;
    pushLong(frame, result);
}

// Aritmética (float)

void fadd_impl(Frame *frame) {
    u4 value2 = popOperand(frame);
    u4 value1 = popOperand(frame);
    float fvalue1 = *(float*)&value1;
    float fvalue2 = *(float*)&value2;
    float fresult = fvalue1 + fvalue2;
    u4 result = *(u4*)&fresult;
    pushOperand(frame, result);
}

void fsub_impl(Frame *frame) {
    u4 value2 = popOperand(frame);
    u4 value1 = popOperand(frame);
    float fvalue1 = *(float*)&value1;
    float fvalue2 = *(float*)&value2;
    float fresult = fvalue1 - fvalue2;
    u4 result = *(u4*)&fresult;
    pushOperand(frame, result);
}

void fmul_impl(Frame *frame) {
    u4 value2 = popOperand(frame);
    u4 value1 = popOperand(frame);
    float fvalue1 = *(float*)&value1;
    float fvalue2 = *(float*)&value2;
    float fresult = fvalue1 * fvalue2;
    u4 result = *(u4*)&fresult;
    pushOperand(frame, result);
}

void fdiv_impl(Frame *frame) {
    u4 value2 = popOperand(frame);
    u4 value1 = popOperand(frame);
    float fvalue1 = *(float*)&value1;
    float fvalue2 = *(float*)&value2;

    if (fvalue2 == 0.0f) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0.0f)\n");
        exit(1);
    }

    float fresult = fvalue1 / fvalue2;
    u4 result = *(u4*)&fresult;
    pushOperand(frame, result);
}

void frem_impl(Frame *frame) {
    u4 value2 = popOperand(frame);
    u4 value1 = popOperand(frame);
    float fvalue1 = *(float*)&value1;
    float fvalue2 = *(float*)&value2;

    if (fvalue2 == 0.0f) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0.0f)\n");
        exit(1);
    }

    float fresult = fmodf(fvalue1, fvalue2);
    u4 result = *(u4*)&fresult;
    pushOperand(frame, result);
}

void fneg_impl(Frame *frame) {
    u4 value = popOperand(frame);
    float fvalue = *(float*)&value;
    float fresult = -fvalue;
    u4 result = *(u4*)&fresult;
    pushOperand(frame, result);
}

// Aritmética (long)

void ladd_impl(Frame *frame){
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

    u8 result = value1 + value2;
    
    pushLong(frame, result);
}

void lsub_impl(Frame *frame){
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

    u8 result = value1 - value2;

    pushLong(frame, result);
}

void lmul_impl(Frame *frame){
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

    u8 result = value1 * value2;

    pushLong(frame, result);
}

void ldiv_impl(Frame *frame){
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

     if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u8 result = value1 / value2;

    pushLong(frame, result);
}

void lrem_impl(Frame *frame){
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

     if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u8 result = value1 % value2;

    pushLong(frame, result);
}

void lneg_impl(Frame *frame){
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

// Return com valor long

void lreturn_impl(Frame *frame) {
    u8 return_value = popLong(frame);
    JVM *jvm = frame->jvm_ref;
    Frame * prev_frame = frame->previous;

    if (prev_frame != NULL) {
        pushLong(prev_frame, return_value);
    }

    jvm->current_frame = prev_frame;
    deleteFrame(frame);
}

// Return com valor double

void dreturn_impl(Frame *frame) {
    u8 return_value = popLong(frame);
    JVM *jvm = frame->jvm_ref;
    Frame * prev_frame = frame->previous;

    if (prev_frame != NULL) {
        pushLong(prev_frame, return_value);
    }

    jvm->current_frame = prev_frame;
    deleteFrame(frame);
}

// Return com valor float

void freturn_impl(Frame *frame) {
    u4 return_value = popOperand(frame);
    JVM *jvm = frame->jvm_ref;
    Frame * prev_frame = frame->previous;

    if (prev_frame != NULL) {
        pushOperand(prev_frame, return_value);
    }

    jvm->current_frame = prev_frame;
    deleteFrame(frame);
}

// Instruções de Pilha

void pop_impl(Frame *frame) {
    popOperand(frame);
}

void pop2_impl(Frame *frame) {
    popOperand(frame);
    popOperand(frame);
}

void dup_impl(Frame *frame) {
    u4 value = peekOperand(frame);
    pushOperand(frame, value);
}

void dup_x1_impl(Frame *frame) {
    u4 value1 = popOperand(frame);
    u4 value2 = popOperand(frame);

    pushOperand(frame, value1);
    pushOperand(frame, value2);
    pushOperand(frame, value1);
}

void dup_x2_impl(Frame *frame) {
    u4 value1 = popOperand(frame);
    u4 value2 = popOperand(frame);
    u4 value3 = popOperand(frame);

    pushOperand(frame, value1);
    pushOperand(frame, value3);
    pushOperand(frame, value2);
    pushOperand(frame, value1);
}

void dup2_impl(Frame *frame) {
    u4 v1 = popOperand(frame);
    u4 v2 = peekOperand(frame);
    // desempilha tudo e empilha de volta
    u4 v2_val = popOperand(frame);
    
    pushOperand(frame, v2_val);
    pushOperand(frame, v1);
    pushOperand(frame, v2_val);
    pushOperand(frame, v1);
}

void swap_impl(Frame *frame) {
    u4 value1 = popOperand(frame);
    u4 value2 = popOperand(frame);

    pushOperand(frame, value1);
    pushOperand(frame, value2);
}

// Instruções de Comparação

void lcmp_impl(Frame *frame) {
    u8 v2 = popLong(frame);
    u8 v1 = popLong(frame);
    
    if ((int64_t)v1 > (int64_t)v2) pushOperand(frame, 1);
    else if ((int64_t)v1 < (int64_t)v2) pushOperand(frame, -1); // Representação de -1 em u4
    else pushOperand(frame, 0);
}

void fcmpl_impl(Frame *frame) {
    u4 v2_bits = popOperand(frame);
    u4 v1_bits = popOperand(frame);
    float v1 = *(float*)&v1_bits;
    float v2 = *(float*)&v2_bits;

    if (isnan(v1) || isnan(v2)) {
        pushOperand(frame, -1);
    } else if (v1 > v2) {
        pushOperand(frame, 1);
    } else if (v1 < v2) {
        pushOperand(frame, -1);
    } else {
        pushOperand(frame, 0);
    }
}

void fcmpg_impl(Frame *frame) {
    u4 v2_bits = popOperand(frame);
    u4 v1_bits = popOperand(frame);
    float v1 = *(float*)&v1_bits;
    float v2 = *(float*)&v2_bits;

    if (isnan(v1) || isnan(v2)) {
        pushOperand(frame, 1);
    } else if (v1 > v2) {
        pushOperand(frame, 1);
    } else if (v1 < v2) {
        pushOperand(frame, -1);
    } else {
        pushOperand(frame, 0);
    }
}
void dcmpl_impl(Frame *frame) {
    u8 v2_bits = popLong(frame);
    u8 v1_bits = popLong(frame);
    double v1 = *(double*)&v1_bits;
    double v2 = *(double*)&v2_bits;

    if (isnan(v1) || isnan(v2)) {
        pushOperand(frame, -1);
    } else if (v1 > v2) {
        pushOperand(frame, 1);
    } else if (v1 < v2) {
        pushOperand(frame, -1);
    } else {
        pushOperand(frame, 0);
    }
}

void dcmpg_impl(Frame *frame) {
    u8 v2_bits = popLong(frame);
    u8 v1_bits = popLong(frame);
    double v1 = *(double*)&v1_bits;
    double v2 = *(double*)&v2_bits;

    if (isnan(v1) || isnan(v2)) {
        pushOperand(frame, 1);
    } else if (v1 > v2) {
        pushOperand(frame, 1);
    } else if (v1 < v2) {
        pushOperand(frame, -1);
    } else {
        pushOperand(frame, 0);
    }
}

// Instruções de Desvio

// Auxiliar para calcular salto
static void branch(Frame *frame, int16_t offset) {
    // O PC atual aponta para DEPOIS dos argumentos da instrução de branch.
    // O offset é relativo ao opcode da instrução de branch.
    // Como os branchs (if, goto) têm 2 bytes de offset + 1 byte opcode = 3 bytes.
    // pc_opcode = frame->pc - 3;
    // novo_pc = pc_opcode + offset;
    u4 pc_opcode = frame->pc - 3;
    frame->pc = pc_opcode + offset;
}

void ifeq_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val == 0) branch(frame, offset);
}

void ifne_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val != 0) branch(frame, offset);
}

void iflt_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val < 0) branch(frame, offset);
}

void ifge_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val >= 0) branch(frame, offset);
}

void ifgt_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val > 0) branch(frame, offset);
}

void ifle_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val <= 0) branch(frame, offset);
}

void if_icmpeq_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 == (int32_t)v2) branch(frame, offset);
}

void if_icmpne_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 != (int32_t)v2) branch(frame, offset);
}

void if_icmplt_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 < (int32_t)v2) branch(frame, offset);
}

void if_icmpge_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 >= (int32_t)v2) branch(frame, offset);
}

void if_icmpgt_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 > (int32_t)v2) branch(frame, offset);
}

void if_icmple_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 <= (int32_t)v2) branch(frame, offset);
}

void goto_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    branch(frame, offset);
}

// Instruções de Invocação e Campo Estático


// --- HELPER: Conta argumentos baseado no descritor (Ex: "(II)V" -> 2) ---
static int countArguments(const char *descriptor) {
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

// ----------------------------------------------------------------------------
// INSTRUÇÃO: INVOKESTATIC (0xB8)
// ----------------------------------------------------------------------------
void invokestatic_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;

    // 1. Resolver Método
    u2 name_type_index = cp[index-1].info.methodref_info.name_and_type_index;
    u2 name_idx = cp[name_type_index-1].info.nameandtype_info.name_index;
    u2 desc_idx = cp[name_type_index-1].info.nameandtype_info.descriptor_index;

    char *method_name = (char *)cp[name_idx-1].info.utf8_info.bytes;
    char *descriptor = (char *)cp[desc_idx-1].info.utf8_info.bytes;

    // 2. Buscar Método na Classe
    method_info *method = findMethod(frame->cf_ref, method_name, descriptor);
    if (!method) {
        fprintf(stderr, "Erro: Metodo estatico '%s' nao encontrado.\n", method_name);
        exit(1);
    }

    code_attribute *code = findCodeAttribute(frame->cf_ref, method);

    // 3. Criar Novo Frame
    // O frame atual torna-se o 'previous' do novo frame
    Frame *newFrame = createFrame(frame->jvm_ref, frame->cf_ref, code, frame);

    // 4. Passar Argumentos da Pilha para Locais
    int num_args = countArguments(descriptor);
    
    // A pilha é LIFO, então os argumentos estão invertidos.
    // Buffer para inverter a ordem antes de colocar nas variaveis locais.
    if (num_args > 0) {
        u4 *args_buffer = malloc(sizeof(u4) * num_args);
        
        // Retira da pilha (Ordem inversa: argN, argN-1 ... arg1)
        for(int k = num_args - 1; k >= 0; k--) {
            args_buffer[k] = popOperand(frame);
        }
        
        // Coloca nas locais (Ordem correta: Var0=arg1, Var1=arg2...)
        for(int k = 0; k < num_args; k++) {
            newFrame->local_variables[k] = args_buffer[k];
        }
        free(args_buffer);
    }

    // 5. Troca de Contexto
    frame->jvm_ref->current_frame = newFrame;
}

// ----------------------------------------------------------------------------
// INSTRUÇÃO: GETSTATIC (0xB2) - Focada em System.out
// ----------------------------------------------------------------------------
void getstatic_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;

    // Apenas verificamos se é System.out para simular
    u2 class_index = cp[index-1].info.fieldref_info.class_index;
    u2 name_type_index = cp[index-1].info.fieldref_info.name_and_type_index;

    u2 class_name_idx = cp[class_index-1].info.class_info.name_index;
    u2 field_name_idx = cp[name_type_index-1].info.nameandtype_info.name_index;

    char *class_name = (char *)cp[class_name_idx-1].info.utf8_info.bytes;
    char *field_name = (char *)cp[field_name_idx-1].info.utf8_info.bytes;

    if (strcmp(class_name, "java/lang/System") == 0 && strcmp(field_name, "out") == 0) {
        // Empilha um valor dummy (0) para representar a referência do objeto PrintStream
        pushOperand(frame, 0); 
    } else {
        // Para outros campos estáticos, empilha 0 (suporte futuro necessário para static fields reais)
        pushOperand(frame, 0);
    }
}

void ldc_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    cp_info *cp = frame->cf_ref->constant_pool;
    
    // O índice aponta para uma entrada no pool
    cp_info *item = &cp[index-1]; 

    if (item->tag == CONSTANT_Integer) {
        pushOperand(frame, item->info.integer_info.bytes);
    } 
    else if (item->tag == CONSTANT_Float) {
        pushOperand(frame, item->info.float_info.bytes);
    } 
    else if (item->tag == CONSTANT_String) {
        // Empilha o índice da String no Constant Pool.
        // O invokevirtual vai usar esse índice para buscar o texto.
        pushOperand(frame, index); 
    }
    else {
        fprintf(stderr, "Erro: ldc com tag nao suportada %d\n", item->tag);
        exit(1);
    }
}

void ldc2_w_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;
    cp_info *item = &cp[index-1];

    if (item->tag == CONSTANT_Long) {
        u8 val = ((u8)item->info.long_info.high_bytes << 32) | item->info.long_info.low_bytes;
        pushLong(frame, val);
    } 
    else if (item->tag == CONSTANT_Double) {
        u8 val = ((u8)item->info.double_info.high_bytes << 32) | item->info.double_info.low_bytes;
        pushLong(frame, val);
    }
    else {
        fprintf(stderr, "Erro: ldc2_w com tag invalida %d\n", item->tag);
        exit(1);
    }
}

// Invoke virtual mimetizando System.out.println e System.out.print

void invokevirtual_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;
    
    // ... resolve nome e tipo (mesma lógica anterior) ...
    u2 name_type_index = cp[index-1].info.methodref_info.name_and_type_index;
    u2 name_idx = cp[name_type_index-1].info.nameandtype_info.name_index;
    u2 desc_idx = cp[name_type_index-1].info.nameandtype_info.descriptor_index;
    
    char *method_name = (char *)cp[name_idx-1].info.utf8_info.bytes;
    char *descriptor = (char *)cp[desc_idx-1].info.utf8_info.bytes;

    if (strcmp(method_name, "println") == 0 || strcmp(method_name, "print") == 0) {
        
        if (strcmp(descriptor, "(Ljava/lang/String;)V") == 0) {
            // String: O valor na pilha é o índice no Constant Pool (vinda do LDC)
            u4 cp_index = popOperand(frame);
            popOperand(frame); // System.out
            // Busca a string real no CP
            cp_info *strInfo = &cp[cp_index-1];
            if (strInfo->tag == CONSTANT_String) {
                u2 utf8_idx = strInfo->info.string_info.string_index;
                char *strContent = (char *)cp[utf8_idx-1].info.utf8_info.bytes;
                printf("%s\n", strContent);
            } else {
                fprintf(stderr, "Erro: Esperado CONSTANT_String no CP para println, mas encontrou tag %d\n", strInfo->tag);
            }
        } 
        else if (strcmp(descriptor, "(I)V") == 0) { // Integer Print
            u4 val = popOperand(frame);
            popOperand(frame);
            printf("%d\n", (int32_t)val);
        }

        else if (strcmp(descriptor, "(Z)V") == 0) { // Boolean Print
            u4 val = popOperand(frame);
            popOperand(frame);
            printf("%s\n", val ? "true" : "false");
        }

        else if (strcmp(descriptor, "(D)V") == 0) { // Double Print
            u8 val = popLong(frame);
            popOperand(frame);
            double dval;
            memcpy(&dval, &val, sizeof(double));
            printf("%f\n", dval);
        }
        else if (strcmp(descriptor, "(J)V") == 0) { // Long Print
            u8 val = popLong(frame);
            popOperand(frame);
            printf("%lld\n", (long long)val);
        }
        else if (strcmp(descriptor, "(F)V") == 0) { // Float Print
            u4 val = popOperand(frame);
            popOperand(frame);
            float fval;
            memcpy(&fval, &val, sizeof(float));
            printf("%f\n", fval);
        }
        else if (strcmp(descriptor, "(Ljava/lang/Object;)V") == 0) { // Object Print (String)
            u4 ref = popOperand(frame);
            popOperand(frame);
            // Como não temos a implementação de String.toString() na JVM,
            // apenas indicamos que a chamada ocorreu e qual era a referência.
            printf("Chamada a System.out.println(Object) com referencia: %u\n", ref);
        }

        else {
            fprintf(stderr,"Metodo println/print com descritor nao suportado: %s\n", descriptor);
        }
    } else {
        fprintf(stderr, "Metodo virtual nao simulado: %s\n", method_name);
    }
}
// LOADS (Referência)
void aload_impl(Frame *frame) {
    printf("Execuntando: aload_impl\n");
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);
}

void aload_0_impl(Frame *frame) {
    printf("Execuntando: aload_0_impl\n");
    pushOperand(frame, frame->local_variables[0]);
}

void aload_1_impl(Frame *frame) {
    printf("Execuntando: aload_1_impl\n");
    pushOperand(frame, frame->local_variables[1]);
}

void aload_2_impl(Frame *frame) {
    printf("Execuntando: aload_2_impl\n");
    pushOperand(frame, frame->local_variables[2]);
}

void aload_3_impl(Frame *frame) {
    printf("Execuntando: aload_3_impl\n");
    pushOperand(frame, frame->local_variables[3]);
}

// STORES (Referência)
void astore_impl(Frame *frame) {
    printf("Execuntando: astore_impl\n");
    u1 index = READ_U1(frame);
    frame->local_variables[index] = popOperand(frame);
}

void astore_0_impl(Frame *frame) {
    printf("Execuntando: astore_0_impl\n");
    frame->local_variables[0] = popOperand(frame);
}

void astore_1_impl(Frame *frame) {
    printf("Execuntando: astore_1_impl\n");
    frame->local_variables[1] = popOperand(frame);
}

void astore_2_impl(Frame *frame) {
    printf("Execuntando: astore_2_impl\n");
    frame->local_variables[2] = popOperand(frame);
}

void astore_3_impl(Frame *frame) {
    printf("Execuntando: astore_3_impl\n");
    frame->local_variables[3] = popOperand(frame);
}

// STACK (dup2_x1 e dup2_x2)
void dup2_x1_impl(Frame *frame) {
    u4 value1 = popOperand(frame);
    u4 value2 = popOperand(frame);
    u4 value3 = popOperand(frame);

    pushOperand(frame, value2);
    pushOperand(frame, value1);
    pushOperand(frame, value3);
    pushOperand(frame, value2);
    pushOperand(frame, value1);
}

void dup2_x2_impl(Frame *frame) {
    u4 value1 = popOperand(frame);
    u4 value2 = popOperand(frame);
    u4 value3 = popOperand(frame);
    u4 value4 = popOperand(frame);

    pushOperand(frame, value2);
    pushOperand(frame, value1);
    pushOperand(frame, value4);
    pushOperand(frame, value3);
    pushOperand(frame, value2);
    pushOperand(frame, value1);
}

// Funções auxiliares para Array Load
#include "memory_manager.h"

// Macro para simplificar a implementação de array load para tipos de 4 bytes (int, float, ref)
#define ARRAY_LOAD_4BYTE(name, type) \
void name##_impl(Frame *frame) { \
    u4 index = popOperand(frame); \
    u4 arrayref = popOperand(frame); \
    \
    Array *array = getArrayFromRef(frame->jvm_ref, arrayref); \
    \
    if (array->type != type) { \
        fprintf(stderr, "Erro: Tipo de array inesperado para " #name ". Esperado %u, encontrado %u\n", type, array->type); \
        exit(1); \
    } \
    \
    if (index >= array->length) { \
        fprintf(stderr, "Erro: ArrayIndexOutOfBoundsException. Índice %u fora do limite %u\n", index, array->length); \
        exit(1); \
    } \
    \
    u4 offset = index * sizeof(u4); \
    u4 value; \
    memcpy(&value, array->data + offset, sizeof(u4)); \
    \
    pushOperand(frame, value); \
}

// Macro para simplificar a implementação de array load para tipos de 8 bytes (long, double)
#define ARRAY_LOAD_8BYTE(name, type) \
void name##_impl(Frame *frame) { \
    u4 index = popOperand(frame); \
    u4 arrayref = popOperand(frame); \
    \
    Array *array = getArrayFromRef(frame->jvm_ref, arrayref); \
    \
    if (array->type != type) { \
        fprintf(stderr, "Erro: Tipo de array inesperado para " #name ". Esperado %u, encontrado %u\n", type, array->type); \
        exit(1); \
    } \
    \
    if (index >= array->length) { \
        fprintf(stderr, "Erro: ArrayIndexOutOfBoundsException. Índice %u fora do limite %u\n", index, array->length); \
        exit(1); \
    } \
    \
    u4 offset = index * sizeof(u8); \
    u8 value; \
    memcpy(&value, array->data + offset, sizeof(u8)); \
    \
    pushLong(frame, value); \
}

// Macro para simplificar a implementação de array load para tipos de 1 byte (byte, boolean)
#define ARRAY_LOAD_1BYTE(name, type) \
void name##_impl(Frame *frame) { \
    u4 index = popOperand(frame); \
    u4 arrayref = popOperand(frame); \
    \
    Array *array = getArrayFromRef(frame->jvm_ref, arrayref); \
    \
    if (array->type != type) { \
        fprintf(stderr, "Erro: Tipo de array inesperado para " #name ". Esperado %u, encontrado %u\n", type, array->type); \
        exit(1); \
    } \
    \
    if (index >= array->length) { \
        fprintf(stderr, "Erro: ArrayIndexOutOfBoundsException. Índice %u fora do limite %u\n", index, array->length); \
        exit(1); \
    } \
    \
    u4 offset = index * sizeof(u1); \
    u1 byte_value = array->data[offset]; \
    \
    /* O valor é estendido para int (u4) e empilhado */ \
    pushOperand(frame, (u4) (int32_t) (int8_t) byte_value); \
}

// Macro para simplificar a implementação de array load para tipos de 2 bytes (char, short)
#define ARRAY_LOAD_2BYTE(name, type) \
void name##_impl(Frame *frame) { \
    u4 index = popOperand(frame); \
    u4 arrayref = popOperand(frame); \
    \
    Array *array = getArrayFromRef(frame->jvm_ref, arrayref); \
    \
    if (array->type != type) { \
        fprintf(stderr, "Erro: Tipo de array inesperado para " #name ". Esperado %u, encontrado %u\n", type, array->type); \
        exit(1); \
    } \
    \
    if (index >= array->length) { \
        fprintf(stderr, "Erro: ArrayIndexOutOfBoundsException. Índice %u fora do limite %u\n", index, array->length); \
        exit(1); \
    } \
    \
    u4 offset = index * sizeof(u2); \
    u2 short_value; \
    memcpy(&short_value, array->data + offset, sizeof(u2)); \
    \
    /* O valor é estendido para int (u4) e empilhado */ \
    pushOperand(frame, (u4) (int32_t) (int16_t) short_value); \
}

// Instruções de Criação de Array
void newarray_impl(Frame *frame) {
    u1 atype = READ_U1(frame);
    u4 count = popOperand(frame);

    u1 element_size;
    u1 array_type;

    // Mapeamento de atype para tamanho e tipo interno
    switch (atype) {
        case 4: // T_BOOLEAN
            element_size = 1;
            array_type = 4;
            break;
        case 5: // T_CHAR
            element_size = 2;
            array_type = 5;
            break;
        case 6: // T_FLOAT
            element_size = 4;
            array_type = 6;
            break;
        case 7: // T_DOUBLE
            element_size = 8;
            array_type = 7;
            break;
        case 8: // T_BYTE
            element_size = 1;
            array_type = 8;
            break;
        case 9: // T_SHORT
            element_size = 2;
            array_type = 9;
            break;
        case 10: // T_INT
            element_size = 4;
            array_type = 10;
            break;
        case 11: // T_LONG
            element_size = 8;
            array_type = 11;
            break;
        default:
            fprintf(stderr, "Erro: Tipo de array primitivo desconhecido para newarray: %u\n", atype);
            exit(1);
    }
    
    Array *arr = createArray(frame->jvm_ref, count, array_type, element_size);
    
    if (arr == NULL) {
        fprintf(stderr, "Erro: createArray retornou NULL.\n");
        exit(1);
    }

    u4 arrayref = (u4)((u1*)arr - frame->jvm_ref->heap);
    printf("DEBUG: Array alocado em heap_ptr: %u, Referencia: %u\n", frame->jvm_ref->heap_ptr, arrayref); // Adicione este print
    pushOperand(frame, arrayref);
}

// Instruções de Armazenamento em Array
void iastore_impl(Frame *frame) {
    // Ordem de desempilhamento: valor, índice, referência do array
    u4 value = popOperand(frame);
    u4 index = popOperand(frame);
    u4 arrayref = popOperand(frame);

    // A função getArrayFromRef deve estar no seu memory_manager.c
    Array *array = getArrayFromRef(frame->jvm_ref, arrayref);

    // 10 é o código para T_INT (array de inteiros)
    if (array->type != 10) { 
        fprintf(stderr, "Erro: Tipo de array inesperado para iastore. Esperado 10, encontrado %u\n", array->type);
        exit(1);
    }

    // Verificação de limites (ArrayIndexOutOfBoundsException)
    if (index >= array->length) {
        fprintf(stderr, "Erro: ArrayIndexOutOfBoundsException. Índice %u fora do limite %u\n", index, array->length);
        exit(1);
    }

    // Calcula o offset e armazena o valor
    u4 offset = index * sizeof(u4);
    memcpy(array->data + offset, &value, sizeof(u4));
}

// Instruções de Armazenamento em Array
void dastore_impl(Frame *frame) {
    // Ordem de desempilhamento: valor (2 slots), índice (1 slot), referência do array (1 slot)
    u8 value = popLong(frame);
    u4 index = popOperand(frame);
    u4 arrayref = popOperand(frame);

    Array *array = getArrayFromRef(frame->jvm_ref, arrayref);

    // 7 é o código para T_DOUBLE (array de doubles)
    if (array->type != 7) { 
        fprintf(stderr, "Erro: Tipo de array inesperado para dastore. Esperado 7, encontrado %u\n", array->type);
        exit(1);
    }

    // Verificação de limites (ArrayIndexOutOfBoundsException)
    if (index >= array->length) {
        fprintf(stderr, "Erro: ArrayIndexOutOfBoundsException. Índice %u fora do limite %u\n", index, array->length);
        exit(1);
    }

    // Calcula o offset e armazena o valor (8 bytes)
    u4 offset = index * sizeof(u8);
    memcpy(array->data + offset, &value, sizeof(u8));
}
