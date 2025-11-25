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


// STORE (Guardar da Pilha para Locais)
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


// Aritmética (int)
void iadd_impl(Frame *frame);
void isub_impl(Frame *frame);
void imul_impl(Frame *frame);
void idiv_impl(Frame *frame);
void irem_impl(Frame *frame);
void ineg_impl(Frame *frame);

void iinc_impl(Frame *frame);

void ladd_impl(Frame *frame);
void lsub_impl(Frame *frame);
void lmul_impl(Frame *frame);
void ldiv_impl(Frame *frame);
void lrem_impl(Frame *frame);
void lneg_impl(Frame *frame);


// RETURN
void return_impl(Frame *frame);
void ireturn_impl(Frame *frame);

#endif // INSTRUCTIONS_IMPL_H
