#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <helpers.h>
#include "reader.h"
#include "memory_manager.h"
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

// ALOAD e ASTORE
void aload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);
}

void aload_0_impl(Frame *frame) { pushOperand(frame, frame->local_variables[0]); }
void aload_1_impl(Frame *frame) { pushOperand(frame, frame->local_variables[1]); }
void aload_2_impl(Frame *frame) { pushOperand(frame, frame->local_variables[2]); }
void aload_3_impl(Frame *frame) { pushOperand(frame, frame->local_variables[3]); }

void astore_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    frame->local_variables[index] = popOperand(frame);
}

void astore_0_impl(Frame *frame) { frame->local_variables[0] = popOperand(frame); }
void astore_1_impl(Frame *frame) { frame->local_variables[1] = popOperand(frame); }
void astore_2_impl(Frame *frame) { frame->local_variables[2] = popOperand(frame); }
void astore_3_impl(Frame *frame) { frame->local_variables[3] = popOperand(frame); }

void areturn_impl(Frame *frame) {
    u4 ref = popOperand(frame);
    JVM *jvm = frame->jvm_ref;
    Frame *prev_frame = frame->previous;
    
    if (prev_frame != NULL) {
        pushOperand(prev_frame, ref);
    }
    
    jvm->current_frame = prev_frame;
    deleteFrame(frame);
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

// Casts
// Int to Long
void i2l_impl(Frame *frame) {
    u4 ival = popOperand(frame);
    int32_t intVal = (int32_t)ival;
    int64_t longVal = (int64_t)intVal;
    pushLong(frame, (u8)longVal);
}

// Int to Float
void i2f_impl(Frame *frame) {
    u4 ival = popOperand(frame);
    int32_t intVal = (int32_t)ival;
    float fVal = (float)intVal;
    u4 result;
    memcpy(&result, &fVal, sizeof(u4));
    pushOperand(frame, result);
}

// Int to Double
void i2d_impl(Frame *frame) {
    u4 ival = popOperand(frame);
    int32_t intVal = (int32_t)ival;
    double dVal = (double)intVal;
    u8 result;
    memcpy(&result, &dVal, sizeof(u8));
    pushLong(frame, result);
}

// Int to Byte
void i2b_impl(Frame *frame) {
    u4 val = popOperand(frame);
    int32_t i = (int32_t)val;
    int8_t b = (int8_t)i; 
    int32_t res = (int32_t)b; // Extende sinal
    pushOperand(frame, (u4)res);
}

// Int to Char
void i2c_impl(Frame *frame) {
    u4 val = popOperand(frame);
    int32_t i = (int32_t)val;
    uint16_t c = (uint16_t)i; 
    int32_t res = (int32_t)c; 
    pushOperand(frame, (u4)res);
}

// Int to Short
void i2s_impl(Frame *frame) {
    u4 val = popOperand(frame);
    int32_t i = (int32_t)val;
    int16_t s = (int16_t)i; 
    int32_t res = (int32_t)s; // Extende sinal
    pushOperand(frame, (u4)res);
}

// Long to Int
void l2i_impl(Frame *frame) {
    u8 lval = popLong(frame);
    int64_t longVal = (int64_t)lval;
    int32_t intVal = (int32_t)longVal; // Trunca
    pushOperand(frame, (u4)intVal);
}

// Long to Float
void l2f_impl(Frame *frame) {
    u8 lval = popLong(frame);
    int64_t longVal = (int64_t)lval;
    float fVal = (float)longVal;
    u4 result;
    memcpy(&result, &fVal, sizeof(u4));
    pushOperand(frame, result);
}

// Long to Double
void l2d_impl(Frame *frame) {
    u8 lval = popLong(frame);
    int64_t longVal = (int64_t)lval;
    double dVal = (double)longVal;
    u8 result;
    memcpy(&result, &dVal, sizeof(u8));
    pushLong(frame, result);
}

// Float to Int
void f2i_impl(Frame *frame) {
    u4 fbits = popOperand(frame);
    float fVal;
    memcpy(&fVal, &fbits, sizeof(u4));
    int32_t intVal = (int32_t)fVal; // Cast direto (truncamento simples)
    pushOperand(frame, (u4)intVal);
}

// Float to Long
void f2l_impl(Frame *frame) {
    u4 fbits = popOperand(frame);
    float fVal;
    memcpy(&fVal, &fbits, sizeof(u4));
    int64_t longVal = (int64_t)fVal;
    pushLong(frame, (u8)longVal);
}

// Float to Double
void f2d_impl(Frame *frame) {
    u4 fbits = popOperand(frame);
    float fVal;
    memcpy(&fVal, &fbits, sizeof(u4));
    double dVal = (double)fVal;
    u8 result;
    memcpy(&result, &dVal, sizeof(u8));
    pushLong(frame, result);
}

// Double

// Double to Int
void d2i_impl(Frame *frame) {
    u8 dbits = popLong(frame);
    double dVal;
    memcpy(&dVal, &dbits, sizeof(u8));
    int32_t intVal = (int32_t)dVal;
    pushOperand(frame, (u4)intVal);
}

// Double to Long
void d2l_impl(Frame *frame) {
    u8 dbits = popLong(frame);
    double dVal;
    memcpy(&dVal, &dbits, sizeof(u8));
    int64_t longVal = (int64_t)dVal;
    pushLong(frame, (u8)longVal);
}

// Double to Float
void d2f_impl(Frame *frame) {
    u8 dbits = popLong(frame);
    double dVal;
    memcpy(&dVal, &dbits, sizeof(u8));
    float fVal = (float)dVal;
    u4 result;
    memcpy(&result, &fVal, sizeof(u4));
    pushOperand(frame, result);
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

// ----------------------------------------------------------------------------
// Switches
// ----------------------------------------------------------------------------

void tableswitch_impl(Frame *frame) {
    
    u4 opcode_pc = frame->pc - 1; 

    u4 current_addr = frame->pc;
    u4 padding = (4 - (current_addr % 4)) % 4;
    frame->pc += padding;

    int32_t default_offset = (int32_t)READ_U4(frame);
    int32_t low            = (int32_t)READ_U4(frame);
    int32_t high           = (int32_t)READ_U4(frame);

    int32_t index = (int32_t)popOperand(frame);
    int32_t jump_offset;

    // Se o índice está fora de [low, high], vai para default
    if (index < low || index > high) {
        jump_offset = default_offset;
    } else {
        u4 table_offset_pos = (u4)(index - low) * 4;
        
        u1* ptr = &frame->bytecode[frame->pc + table_offset_pos];
        
        jump_offset = (int32_t)((ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3]);
    }

    // Realiza o salto relativo ao opcode original
    frame->pc = opcode_pc + jump_offset;
}

void lookupswitch_impl(Frame *frame) {
    u4 opcode_pc = frame->pc - 1;

    u4 padding = (4 - (frame->pc % 4)) % 4;
    frame->pc += padding;

    // Lê default e número de pares
    int32_t default_offset = (int32_t)READ_U4(frame);
    int32_t npairs         = (int32_t)READ_U4(frame);
    int32_t key = (int32_t)popOperand(frame);
    
    int32_t jump_offset = default_offset;

    for (int i = 0; i < npairs; i++) {
        int32_t match  = (int32_t)READ_U4(frame);
        int32_t offset = (int32_t)READ_U4(frame);

        if (key == match) {
            jump_offset = offset;
            break;
        }
    }
    // Realiza o salto relativo ao opcode original
    frame->pc = opcode_pc + jump_offset;
}


// Instruções de Invocação e Campo Estático

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
        else if (strcmp(descriptor, "(C)V") == 0) {
            u4 val = popOperand(frame);
            printf("%c", (char)val);
        }

        else if (strcmp(descriptor, "()V") == 0) {
            printf("\n");
        }

        else {
            // fprintf(stderr,"Metodo println/print com descritor nao suportado: %s\n", descriptor);
        }
    } else {
        //fprintf(stderr, "Metodo virtual nao simulado: %s\n", method_name);
    }
}

// Auxiliares invokes


// Busca uma classe já carregada na Área de Métodos
static ClassFile* findClassInMethodArea(JVM *jvm, const char *classname) {
    for (int i = 0; i < jvm->method_area_count; i++) {
        ClassFile *cf = jvm->method_area[i];
        // Nome da classe está no índice apontado por this_class
        u2 this_class_idx = cf->this_class;
        u2 name_idx = cf->constant_pool[this_class_idx - 1].info.class_info.name_index;
        char *name = (char *)cf->constant_pool[name_idx - 1].info.utf8_info.bytes;
        if (strcmp(name, classname) == 0) {
            return cf;
        }
    }
    return NULL;
}

// Calcula tamanho dos campos de instância (não estáticos) para alocar memória
static u4 calculateFieldSize(ClassFile *cf) {
    u4 size = 0;
    for(int i = 0; i < cf->fields_count; i++) {
        // Ignora campos estáticos (ACC_STATIC = 0x0008)
        if (!(cf->fields[i].access_flags & 0x0008)) {
            // Verifica se é long/double (ocupa 2 slots)
            u2 desc_idx = cf->fields[i].descriptor_index;
            char *desc = (char *)cf->constant_pool[desc_idx-1].info.utf8_info.bytes;
            if (desc[0] == 'J' || desc[0] == 'D') size += 2;
            else size += 1;
        }
    }
    return size;
}

// Helper para recuperar ponteiro a partir do índice
static Object* getObjectRef(JVM *jvm, u4 index) {
    if (index == 0) return NULL;
    if (index >= jvm->objects_count) {
        fprintf(stderr, "Erro: Referencia de objeto invalida (%u)\n", index);
        exit(1);
    }
    return jvm->objects[index];
}

// Objetos

void new_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;
    
    // 1. Obter nome da classe a ser instanciada
    u2 class_info_idx = index;
    u2 name_index = cp[class_info_idx-1].info.class_info.name_index;
    char *classname = (char *)cp[name_index-1].info.utf8_info.bytes;

    // 2. Verificar se já está carregada
    JVM *jvm = frame->jvm_ref;
    ClassFile *targetClass = findClassInMethodArea(jvm, classname);
    // 3. Simulação de ClassLoader: Se não carregada, tenta ler do disco
    if (targetClass == NULL) {
        printf("Tentando carregar classe %s do disco...\n", classname);
        char filename[256];
        // Tenta no diretório local ou na pasta exemplos
        sprintf(filename, "%s.class", classname);
        targetClass = readFile(filename);
        
        if (targetClass == NULL) {
             sprintf(filename, "exemplos/%s.class", classname);
             targetClass = readFile(filename);
        }

        if (targetClass == NULL) {
            fprintf(stderr, "Erro Fatal: Nao foi possivel encontrar a classe '%s' para instanciar.\n", classname);
            exit(1);
        }
        loadClass(jvm, targetClass);
    }

    // 4. Alocar Objeto
    u4 fieldsSize = calculateFieldSize(targetClass);
    
    // Encontrar índice da classe na method_area
    u4 class_area_index = 0;
    for(int i=0; i < jvm->method_area_count; i++) {
        if(jvm->method_area[i] == targetClass) {
            class_area_index = i;
            break;
        }
    }

    Object *obj = createObject(jvm, class_area_index, fieldsSize);

    // 5. Empilhar Referência
    u4 obj_index = registerObject(jvm, obj);
    pushOperand(frame, obj_index);
}

void putfield_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;

    // Resolver Nome e Descritor do Campo
    u2 name_type_index = cp[index-1].info.fieldref_info.name_and_type_index;
    u2 name_idx = cp[name_type_index-1].info.nameandtype_info.name_index;
    u2 desc_idx = cp[name_type_index-1].info.nameandtype_info.descriptor_index;
    
    char *field_name = (char *)cp[name_idx-1].info.utf8_info.bytes;
    char *descriptor = (char *)cp[desc_idx-1].info.utf8_info.bytes;

    // Valor a ser gravado (Wide ou Normal)
    int is_wide = (descriptor[0] == 'J' || descriptor[0] == 'D');
    u8 val_wide = 0;
    u4 val_single = 0;

    if (is_wide) val_wide = popLong(frame);
    else val_single = popOperand(frame);

    u4 object_idx = popOperand(frame);
    if (object_idx == 0) {
        fprintf(stderr, "NullPointerException em putfield\n");
        exit(1);
    }

    Object *obj = getObjectRef(frame->jvm_ref, object_idx);
    
    ClassFile *cf = getClass(frame->jvm_ref, obj->class_index);

    // Calcular offset do campo dentro do objeto
    int offset = 0;
    int found = 0;
    for (int i = 0; i < cf->fields_count; i++) {
        // Pula estáticos (putfield só mexe em instância)
        if (cf->fields[i].access_flags & 0x0008) continue;

        u2 fname_idx = cf->fields[i].name_index;
        char *fname = (char *)cf->constant_pool[fname_idx-1].info.utf8_info.bytes;
        
        if (strcmp(fname, field_name) == 0) {
            found = 1;
            break;
        }

        // Soma tamanho do campo anterior ao offset
        u2 fdesc_idx = cf->fields[i].descriptor_index;
        char *fdesc = (char *)cf->constant_pool[fdesc_idx-1].info.utf8_info.bytes;
        if (fdesc[0] == 'J' || fdesc[0] == 'D') offset += 2;
        else offset += 1;
    }

    if (!found) {
        fprintf(stderr, "Erro: Campo '%s' nao encontrado na classe.\n", field_name);
        exit(1);
    }

    // Gravar na "memória" do objeto (tratando u1* como u4*)
    u4 *fields_data = (u4*)obj->data;
    if (is_wide) {
        u4 low = (u4)(val_wide & 0xFFFFFFFF);
        u4 high = (u4)(val_wide >> 32);
        fields_data[offset] = high; 
        fields_data[offset+1] = low;
    } else {
        fields_data[offset] = val_single;
    }
}

void getfield_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;

    // Resolver Field
    u2 name_type_index = cp[index-1].info.fieldref_info.name_and_type_index;
    u2 name_idx = cp[name_type_index-1].info.nameandtype_info.name_index;
    u2 desc_idx = cp[name_type_index-1].info.nameandtype_info.descriptor_index;

    char *field_name = (char *)cp[name_idx-1].info.utf8_info.bytes;
    char *descriptor = (char *)cp[desc_idx-1].info.utf8_info.bytes;

    u4 object_idx = popOperand(frame);
    if (object_idx == 0) {
        fprintf(stderr, "NullPointerException em getfield\n");
        exit(1);
    }

    Object *obj = getObjectRef(frame->jvm_ref, object_idx);
    ClassFile *cf = getClass(frame->jvm_ref, obj->class_index);

    // Calcular offset (mesma lógica do putfield)
    int offset = 0;
    int found = 0;
    for (int i = 0; i < cf->fields_count; i++) {
        if (cf->fields[i].access_flags & 0x0008) continue;

        u2 fname_idx = cf->fields[i].name_index;
        char *fname = (char *)cf->constant_pool[fname_idx-1].info.utf8_info.bytes;
        if (strcmp(fname, field_name) == 0) {
            found = 1;
            break;
        }
        u2 fdesc_idx = cf->fields[i].descriptor_index;
        char *fdesc = (char *)cf->constant_pool[fdesc_idx-1].info.utf8_info.bytes;
        if (fdesc[0] == 'J' || fdesc[0] == 'D') offset += 2;
        else offset += 1;
    }

    if (!found) {
        fprintf(stderr, "Erro: Campo '%s' nao encontrado.\n", field_name);
        exit(1);
    }

    u4 *fields_data = (u4*)obj->data;
    if (descriptor[0] == 'J' || descriptor[0] == 'D') {
        u4 high = fields_data[offset];
        u4 low = fields_data[offset+1];
        u8 val = ((u8)high << 32) | low;
        pushLong(frame, val);
    } else {
        pushOperand(frame, fields_data[offset]);
    }
}

// Invocações

void invokespecial_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;

    // 1. Resolver Classe e Método
    u2 class_index = cp[index-1].info.methodref_info.class_index;
    u2 name_type_index = cp[index-1].info.methodref_info.name_and_type_index;
    
    u2 class_name_idx = cp[class_index-1].info.class_info.name_index;
    char *classname = (char *)cp[class_name_idx-1].info.utf8_info.bytes;

    u2 name_idx = cp[name_type_index-1].info.nameandtype_info.name_index;
    u2 desc_idx = cp[name_type_index-1].info.nameandtype_info.descriptor_index;
    char *method_name = (char *)cp[name_idx-1].info.utf8_info.bytes;
    char *descriptor = (char *)cp[desc_idx-1].info.utf8_info.bytes;

    // *** SIMULAÇÃO DE OBJECT.<INIT> ***
    // Como especificado, podemos usar Object.class, mas é mais simples mockar
    // o construtor padrão para não precisar ler o arquivo .class do Java.
    if (strcmp(classname, "java/lang/Object") == 0 && strcmp(method_name, "<init>") == 0) {
        popOperand(frame); // Remove 'this' da pilha
        return;
    }

    // Carregar classe do usuário se necessário
    JVM *jvm = frame->jvm_ref;
    ClassFile *targetClass = findClassInMethodArea(jvm, classname);
    if (!targetClass) {
         // Tenta carregar
         char filename[256];
         sprintf(filename, "exemplos/%s.class", classname); // Tenta pasta exemplos
         targetClass = readFile(filename);
         if(!targetClass) { // Tenta local
             sprintf(filename, "%s.class", classname);
             targetClass = readFile(filename);
         }
         if (!targetClass) {
             fprintf(stderr, "Erro: Classe '%s' nao encontrada (invokespecial).\n", classname);
             exit(1);
         }
         loadClass(jvm, targetClass);
    }

    method_info *method = findMethod(targetClass, method_name, descriptor);
    if (!method) {
        fprintf(stderr, "Erro: Metodo '%s' nao encontrado.\n", method_name);
        exit(1);
    }

    // Preparar novo frame
    code_attribute *code = findCodeAttribute(targetClass, method);
    Frame *newFrame = createFrame(jvm, targetClass, code, frame);

    // Passagem de argumentos (contagem manual simplificada)
    // +1 para o 'this'
    int num_args = 0;
    int i = 1; 
    while (descriptor[i] != ')') {
        if (descriptor[i] == 'L') { while(descriptor[i] != ';') i++; num_args++; }
        else if (descriptor[i] == '[') { while(descriptor[i] == '[') i++; if(descriptor[i]=='L') while(descriptor[i]!=';') i++; num_args++; }
        else if (descriptor[i] == 'J' || descriptor[i] == 'D') { num_args += 2; i++; }
        else { num_args++; i++; }
    }
    
    int total_slots = num_args + 1; // +1 'this'
    u4 *args = malloc(sizeof(u4) * total_slots);
    
    // Desempilha na ordem inversa
    for(int k = total_slots - 1; k >= 0; k--) {
        args[k] = popOperand(frame);
    }
    
    // O 'this' está em args[0]
    if (args[0] == 0) {
        fprintf(stderr, "NullPointerException no invokespecial (this == null)\n");
        exit(1);
    }

    // Coloca nas locais do novo frame
    for(int k = 0; k < total_slots; k++) {
        newFrame->local_variables[k] = args[k];
    }
    free(args);

    jvm->current_frame = newFrame;
}

void checkcast_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    // Para simplificar, não validamos a hierarquia de tipos real aqui.
    // Apenas garantimos que a instrução passa sem erro se não for nulo.
    (void)index;
}

void instanceof_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    u4 ref = popOperand(frame);
    if (ref == 0) pushOperand(frame, 0);
    else pushOperand(frame, 1); // Simplificação: assume true se não null
    (void)index;
}

void ifnull_impl(Frame *frame) {
    // Leitura manual do offset com sinal
    u1 b1 = frame->bytecode[frame->pc];
    u1 b2 = frame->bytecode[frame->pc+1];
    frame->pc += 2;
    int16_t offset = (int16_t)((b1 << 8) | b2);

    u4 ref = popOperand(frame);
    if (ref == 0) {
        frame->pc = (frame->pc - 3) + offset;
    }
}

void ifnonnull_impl(Frame *frame) {
    u1 b1 = frame->bytecode[frame->pc];
    u1 b2 = frame->bytecode[frame->pc+1];
    frame->pc += 2;
    int16_t offset = (int16_t)((b1 << 8) | b2);

    u4 ref = popOperand(frame);
    if (ref != 0) {
        frame->pc = (frame->pc - 3) + offset;
    }
}

// ... includes anteriores ...

// ============================================================================
// ARRAYS - AUXILIARES E CONSTANTES
// ============================================================================

#define T_BOOLEAN 4
#define T_CHAR    5
#define T_FLOAT   6
#define T_DOUBLE  7
#define T_BYTE    8
#define T_SHORT   9
#define T_INT     10
#define T_LONG    11

// Helper para pegar Array da tabela de objetos
static Array* getArrayRef(JVM *jvm, u4 index) {
    if (index == 0) return NULL;
    if (index >= jvm->objects_count) {
        fprintf(stderr, "Erro: Referencia de array invalida (%u)\n", index);
        exit(1);
    }
    return (Array*)jvm->objects[index]; 
}

// Macro para validação de limites
#define CHECK_ARRAY_BOUNDS(arr, idx) \
    if (arr == NULL) { fprintf(stderr, "NullPointerException (Array access)\n"); exit(1); } \
    if (idx < 0 || idx >= (int32_t)arr->length) { \
        fprintf(stderr, "ArrayIndexOutOfBoundsException: Index %d, Length %d\n", idx, arr->length); \
        exit(1); \
    }

// ============================================================================
// CRIAÇÃO DE ARRAYS
// ============================================================================

void newarray_impl(Frame *frame) {
    u1 atype = READ_U1(frame);
    int32_t count = (int32_t)popOperand(frame);

    if (count < 0) {
        fprintf(stderr, "NegativeArraySizeException\n");
        exit(1);
    }

    u1 element_size = 0;
    switch (atype) {
        case T_BOOLEAN: case T_BYTE:  element_size = 1; break;
        case T_CHAR:    case T_SHORT: element_size = 2; break;
        case T_FLOAT:   case T_INT:   element_size = 4; break;
        case T_DOUBLE:  case T_LONG:  element_size = 8; break;
        default:
            fprintf(stderr, "Erro: Tipo de array invalido (%d)\n", atype);
            exit(1);
    }

    // Cria e registra (memory_manager.c deve ter registerObject e createArray)
    Array *arr = createArray(frame->jvm_ref, (u4)count, atype, element_size);
    u4 index = registerObject(frame->jvm_ref, (Object*)arr);
    
    pushOperand(frame, index);
}

void anewarray_impl(Frame *frame) {
    u2 index = READ_U2(frame); // Índice do tipo (class), não usado na alocação física
    int32_t count = (int32_t)popOperand(frame);
    
    if (count < 0) {
        fprintf(stderr, "NegativeArraySizeException\n");
        exit(1);
    }

    // Array de referências armazena IDs (u4) -> 4 bytes
    Array *arr = createArray(frame->jvm_ref, (u4)count, 0, 4);
    u4 obj_index = registerObject(frame->jvm_ref, (Object*)arr);
    
    pushOperand(frame, obj_index);
    (void)index;
}

void arraylength_impl(Frame *frame) {
    u4 array_idx = popOperand(frame);
    if (array_idx == 0) {
        fprintf(stderr, "NullPointerException em arraylength\n");
        exit(1);
    }
    // Recupera o array da tabela
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    pushOperand(frame, arr->length);
}

// ============================================================================
// LOADS (CARREGAR DO ARRAY PARA A PILHA USANDO MEMCPY)
// ============================================================================

void iaload_impl(Frame *frame) {
    int32_t index = (int32_t)popOperand(frame);
    u4 array_idx = popOperand(frame);
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    CHECK_ARRAY_BOUNDS(arr, index);
    
    u4 val;
    // Copia 4 bytes da posição calculada para a variável val
    memcpy(&val, arr->data + (index * 4), 4);
    pushOperand(frame, val);
}

void faload_impl(Frame *frame) {
    iaload_impl(frame); // Float também tem 4 bytes, mesma lógica
}

void laload_impl(Frame *frame) {
    int32_t index = (int32_t)popOperand(frame);
    u4 array_idx = popOperand(frame);
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    CHECK_ARRAY_BOUNDS(arr, index);
    
    u8 val;
    // Copia 8 bytes
    memcpy(&val, arr->data + (index * 8), 8);
    
    // Divide em High e Low para empilhar (Big Endian logic na pilha)
    u4 high = (u4)(val >> 32);
    u4 low  = (u4)(val & 0xFFFFFFFF);
    
    pushOperand(frame, high);
    pushOperand(frame, low);
}

void daload_impl(Frame *frame) {
    laload_impl(frame); // Double também tem 8 bytes, mesma lógica
}

void aaload_impl(Frame *frame) {
    iaload_impl(frame); // Referência é um ID u4, mesma lógica do int
}

void baload_impl(Frame *frame) { // Byte e Boolean
    int32_t index = (int32_t)popOperand(frame);
    u4 array_idx = popOperand(frame);
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    CHECK_ARRAY_BOUNDS(arr, index);
    
    int8_t val; // Lê 1 byte com sinal
    memcpy(&val, arr->data + (index * 1), 1);
    
    // Expande sinal para 32 bits e empilha
    pushOperand(frame, (int32_t)val);
}

void caload_impl(Frame *frame) { // Char (unsigned short)
    int32_t index = (int32_t)popOperand(frame);
    u4 array_idx = popOperand(frame);
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    CHECK_ARRAY_BOUNDS(arr, index);
    
    u2 val; // Lê 2 bytes sem sinal
    memcpy(&val, arr->data + (index * 2), 2);
    
    pushOperand(frame, (u4)val); // Zero-extend
}

void saload_impl(Frame *frame) { // Short (signed short)
    int32_t index = (int32_t)popOperand(frame);
    u4 array_idx = popOperand(frame);
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    CHECK_ARRAY_BOUNDS(arr, index);
    
    int16_t val; // Lê 2 bytes com sinal
    memcpy(&val, arr->data + (index * 2), 2);
    
    pushOperand(frame, (int32_t)val); // Sign-extend
}

// ============================================================================
// STORES (GRAVAR DA PILHA NO ARRAY USANDO MEMCPY)
// ============================================================================

void iastore_impl(Frame *frame) {
    u4 val = popOperand(frame);
    int32_t index = (int32_t)popOperand(frame);
    u4 array_idx = popOperand(frame);
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    CHECK_ARRAY_BOUNDS(arr, index);
    
    // Copia da variável local para a heap
    memcpy(arr->data + (index * 4), &val, 4);
}

void fastore_impl(Frame *frame) {
    iastore_impl(frame);
}

void lastore_impl(Frame *frame) {
    u4 low = popOperand(frame);
    u4 high = popOperand(frame);
    int32_t index = (int32_t)popOperand(frame);
    u4 array_idx = popOperand(frame);
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    CHECK_ARRAY_BOUNDS(arr, index);
    
    // Reconstrói o u8
    u8 val = ((u8)high << 32) | low;
    
    // Copia 8 bytes para a heap
    memcpy(arr->data + (index * 8), &val, 8);
}

void dastore_impl(Frame *frame) {
    lastore_impl(frame);
}

void aastore_impl(Frame *frame) {
    iastore_impl(frame); // Grava ID (u4)
}

void bastore_impl(Frame *frame) {
    u4 val_u4 = popOperand(frame);
    int32_t index = (int32_t)popOperand(frame);
    u4 array_idx = popOperand(frame);
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    CHECK_ARRAY_BOUNDS(arr, index);
    
    // Trunca para 1 byte
    u1 val = (u1)val_u4;
    memcpy(arr->data + (index * 1), &val, 1);
}

void castore_impl(Frame *frame) {
    u4 val_u4 = popOperand(frame);
    int32_t index = (int32_t)popOperand(frame);
    u4 array_idx = popOperand(frame);
    Array *arr = getArrayRef(frame->jvm_ref, array_idx);
    CHECK_ARRAY_BOUNDS(arr, index);
    
    // Trunca para 2 bytes
    u2 val = (u2)val_u4;
    memcpy(arr->data + (index * 2), &val, 2);
}

void sastore_impl(Frame *frame) {
    castore_impl(frame); // Short e Char ocupam 2 bytes, mesma cópia
}