#ifndef INSTRUCTIONS_IMPL_H
#define INSTRUCTIONS_IMPL_H

#include "jvm_structures.h"
#include "frame.h"

// Função  para instruções não implementadas
void nop_impl(Frame *frame);
void unimpl(Frame *frame);


// CONST (empilhar constantes na pilha)
void aconst_null_impl(Frame *frame);
void iconst_m1_impl(Frame *frame);
void iconst_0_impl(Frame *frame);
void iconst_1_impl(Frame *frame);
void iconst_2_impl(Frame *frame);
void iconst_3_impl(Frame *frame);
void iconst_4_impl(Frame *frame);
void iconst_5_impl(Frame *frame);

void lconst_0_impl(Frame *frame);
void lconst_1_impl(Frame *frame);

void fconst_0_impl(Frame *frame);
void fconst_1_impl(Frame *frame);
void fconst_2_impl(Frame *frame);

void dconst_0_impl(Frame *frame);
void dconst_1_impl(Frame *frame);

void bipush_impl(Frame *frame);   // Byte Imediato
void sipush_impl(Frame *frame);   // Short Imediato


// LOAD (Carregar de Locais para Pilha)
void iload_impl(Frame *frame);
void iload_0_impl(Frame *frame);
void iload_1_impl(Frame *frame);
void iload_2_impl(Frame *frame);
void iload_3_impl(Frame *frame);

void lload_impl(Frame *frame);
void lload_0_impl(Frame *frame);
void lload_1_impl(Frame *frame);
void lload_2_impl(Frame *frame);
void lload_3_impl(Frame *frame);

void dload_impl(Frame *frame);
void dload_0_impl(Frame *frame);
void dload_1_impl(Frame *frame);
void dload_2_impl(Frame *frame);
void dload_3_impl(Frame *frame);

void fload_impl(Frame *frame);
void fload_0_impl(Frame *frame);
void fload_1_impl(Frame *frame);
void fload_2_impl(Frame *frame);
void fload_3_impl(Frame *frame);

// LOAD FROM ARRAY
void iaload_impl(Frame *frame);
void laload_impl(Frame *frame);
void faload_impl(Frame *frame);
void daload_impl(Frame *frame);
void aaload_impl(Frame *frame);
void baload_impl(Frame *frame);
void caload_impl(Frame *frame);
void saload_impl(Frame *frame);

void aload_impl(Frame *frame);
void aload_0_impl(Frame *frame);
void aload_1_impl(Frame *frame);
void aload_2_impl(Frame *frame);
void aload_3_impl(Frame *frame);

// STORE (Guardar da Pilha para Locais)
void astore_impl(Frame *frame);
void astore_0_impl(Frame *frame);
void astore_1_impl(Frame *frame);
void astore_2_impl(Frame *frame);
void astore_3_impl(Frame *frame);
void istore_impl(Frame *frame);
void istore_0_impl(Frame *frame);
void istore_1_impl(Frame *frame);
void istore_2_impl(Frame *frame);
void istore_3_impl(Frame *frame);

void lstore_impl(Frame *frame);
void lstore_0_impl(Frame *frame);
void lstore_1_impl(Frame *frame);
void lstore_2_impl(Frame *frame);
void lstore_3_impl(Frame *frame);

void dstore_impl(Frame *frame);
void dstore_0_impl(Frame *frame);
void dstore_1_impl(Frame *frame);
void dstore_2_impl(Frame *frame);
void dstore_3_impl(Frame *frame);

void fstore_impl(Frame *frame);
void fstore_0_impl(Frame *frame);
void fstore_1_impl(Frame *frame);
void fstore_2_impl(Frame *frame);
void fstore_3_impl(Frame *frame);

void newarray_impl(Frame *frame);
void iastore_impl(Frame *frame);
void dastore_impl(Frame *frame);

// Aritmética (int)
void iadd_impl(Frame *frame);
void isub_impl(Frame *frame);
void imul_impl(Frame *frame);
void idiv_impl(Frame *frame);
void irem_impl(Frame *frame);
void ineg_impl(Frame *frame);

void iinc_impl(Frame *frame);

// Aritmética (double)
void dadd_impl(Frame *frame);
void dsub_impl(Frame *frame);
void dmul_impl(Frame *frame);
void ddiv_impl(Frame *frame);
void drem_impl(Frame *frame);
void dneg_impl(Frame *frame);

// Artimética (float)
void fadd_impl(Frame *frame);
void fsub_impl(Frame *frame);
void fmul_impl(Frame *frame);
void fdiv_impl(Frame *frame);
void frem_impl(Frame *frame);
void fneg_impl(Frame *frame);

// Aritmética (Long)
void ladd_impl(Frame *frame);
void lsub_impl(Frame *frame);
void lmul_impl(Frame *frame);
void ldiv_impl(Frame *frame);
void lrem_impl(Frame *frame);
void lneg_impl(Frame *frame);


// RETURN
void return_impl(Frame *frame);
void ireturn_impl(Frame *frame);
void lreturn_impl(Frame *frame);
void freturn_impl(Frame *frame);
void dreturn_impl(Frame *frame);

// STACK
void pop_impl(Frame *frame);
void pop2_impl(Frame *frame);
void dup_impl(Frame *frame);
void dup_x1_impl(Frame *frame);
void dup_x2_impl(Frame *frame);
void dup2_impl(Frame *frame);
void dup2_x1_impl(Frame *frame);
void dup2_x2_impl(Frame *frame);
void swap_impl(Frame *frame);

// COMPARE
void lcmp_impl(Frame *frame);
void fcmpl_impl(Frame *frame);
void fcmpg_impl(Frame *frame);
void dcmpl_impl(Frame *frame);
void dcmpg_impl(Frame *frame);

// BRANCH
void ifeq_impl(Frame *frame);
void ifne_impl(Frame *frame);
void iflt_impl(Frame *frame);
void ifge_impl(Frame *frame);
void ifgt_impl(Frame *frame);
void ifle_impl(Frame *frame);
void if_icmpeq_impl(Frame *frame);
void if_icmpne_impl(Frame *frame);
void if_icmplt_impl(Frame *frame);
void if_icmpge_impl(Frame *frame);
void if_icmpgt_impl(Frame *frame);
void if_icmple_impl(Frame *frame);
void if_acmpeq_impl(Frame *frame);
void if_acmpne_impl(Frame *frame);
void goto_impl(Frame *frame);

// Métodos de invocação
void getstatic_impl(Frame *frame);
void invokestatic_impl(Frame *frame);
void invokevirtual_impl(Frame *frame);

// LDC
void ldc_impl(Frame *frame);
void ldc2_w_impl(Frame *frame);

#endif // INSTRUCTIONS_IMPL_H
