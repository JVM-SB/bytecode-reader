#include <stdio.h>

#include "execution_engine.h"
#include "instructions.h"
#include "instructions_impl.h"

// Vetor de Ponteiros de Função Global
instruction_func instructions[256];

void initExecutionEngine() {
    for (int i = 0; i < 256; i++) {
        instructions[i] = unimpl;
    }

    instructions[nop]         = nop_impl;
    instructions[aconst_null] = aconst_null_impl;

    instructions[iconst_m1] = iconst_m1_impl;
    instructions[iconst_0]  = iconst_0_impl;
    instructions[iconst_1]  = iconst_1_impl;
    instructions[iconst_2]  = iconst_2_impl;
    instructions[iconst_3]  = iconst_3_impl;
    instructions[iconst_4]  = iconst_4_impl;
    instructions[iconst_5]  = iconst_5_impl;

    instructions[lconst_0]  = lconst_0_impl;
    instructions[lconst_1]  = lconst_1_impl;

    instructions[bipush]    = bipush_impl;
    instructions[sipush]    = sipush_impl;

    instructions[iload]     = iload_impl;
    instructions[iload_0]   = iload_0_impl;
    instructions[iload_1]   = iload_1_impl;
    instructions[iload_2]   = iload_2_impl;
    instructions[iload_3]   = iload_3_impl;

    instructions[lload]     = lload_impl;
    instructions[lload_0]   = lload_0_impl;
    instructions[lload_1]   = lload_1_impl;
    instructions[lload_2]   = lload_2_impl;
    instructions[lload_3]   = lload_3_impl;

    instructions[dload]     = dload_impl;
    instructions[dload_0]   = dload_0_impl;
    instructions[dload_1]   = dload_1_impl;
    instructions[dload_2]   = dload_2_impl;
    instructions[dload_3]   = dload_3_impl;

    instructions[fload]     = fload_impl;
    instructions[fload_0]   = fload_0_impl;
    instructions[fload_1]   = fload_1_impl;
    instructions[fload_2]   = fload_2_impl;
    instructions[fload_3]   = fload_3_impl;

    instructions[istore]    = istore_impl;
    instructions[istore_0]  = istore_0_impl;
    instructions[istore_1]  = istore_1_impl;
    instructions[istore_2]  = istore_2_impl;
    instructions[istore_3]  = istore_3_impl;

    instructions[lstore]    = lstore_impl;
    instructions[lstore_0]  = lstore_0_impl;
    instructions[lstore_1]  = lstore_1_impl;
    instructions[lstore_2]  = lstore_2_impl;
    instructions[lstore_3]  = lstore_3_impl;

    instructions[dstore]    = dstore_impl;
    instructions[dstore_0]  = dstore_0_impl;
    instructions[dstore_1]  = dstore_1_impl;
    instructions[dstore_2]  = dstore_2_impl;
    instructions[dstore_3]  = dstore_3_impl;

    instructions[fstore]    = fstore_impl;
    instructions[fstore_0]  = fstore_0_impl;
    instructions[fstore_1]  = fstore_1_impl;
    instructions[fstore_2]  = fstore_2_impl;
    instructions[fstore_3]  = fstore_3_impl;

    // TODO: Terminar o vetor de instrução e sua implementação

    // Aritmética (int)
    instructions[iadd] = iadd_impl;
    instructions[isub] = isub_impl;
    instructions[imul] = imul_impl;
    instructions[idiv] = idiv_impl;
    instructions[irem] = irem_impl;
    instructions[ineg] = ineg_impl;

    // Incremento
    instructions[iinc] = iinc_impl;

    instructions[ladd] = ladd_impl;
    instructions[lsub] = lsub_impl;
    instructions[lmul] = lmul_impl;
    instructions[ldiv_] = ldiv_impl;
    instructions[lrem] = lrem_impl;
    instructions[lneg] = lneg_impl;

    // Retorno
    instructions[ireturn] = ireturn_impl;
    instructions[return_] = return_impl;

    // Stack Operations
    instructions[pop]       = pop_impl;
    instructions[pop2]      = pop2_impl;
    instructions[dup]       = dup_impl;
    instructions[dup_x1]    = dup_x1_impl;
    instructions[dup_x2]    = dup_x2_impl;
    instructions[dup2]      = dup2_impl;
    instructions[swap]      = swap_impl;

    // Comparação
    instructions[lcmp]      = lcmp_impl;

    // Branching (IFs)
    instructions[ifeq]      = ifeq_impl;
    instructions[ifne]      = ifne_impl;
    instructions[iflt]      = iflt_impl;
    instructions[ifge]      = ifge_impl;
    instructions[ifgt]      = ifgt_impl;
    instructions[ifle]      = ifle_impl;

    instructions[if_icmpeq] = if_icmpeq_impl;
    instructions[if_icmpne] = if_icmpne_impl;
    instructions[if_icmplt] = if_icmplt_impl;
    instructions[if_icmpge] = if_icmpge_impl;
    instructions[if_icmpgt] = if_icmpgt_impl;
    instructions[if_icmple] = if_icmple_impl;

    instructions[goto_]     = goto_impl;


    // Instruções de Invocação e Campo Estático
    instructions[getstatic]     = getstatic_impl;
    instructions[invokestatic]  = invokestatic_impl;
    instructions[invokevirtual] = invokevirtual_impl;

    // LDC
    instructions[ldc]       = ldc_impl;
    instructions[ldc2_w]    = ldc2_w_impl;
}

void execute(JVM *jvm) {
    while (jvm->current_frame != NULL) {
        Frame *frame = jvm->current_frame;

        if (frame->pc >= frame->code_length) {
            fprintf(stderr, "Erro: PC passou do fim do código!");
            break; 
        }

        // FETCH
        u1 opcode = frame->bytecode[frame->pc];
        frame->pc++; 

        // EXECUTE
        // Os argumentos são lidos dentro da função da instrução
        instructions[opcode](frame);
    }
}
