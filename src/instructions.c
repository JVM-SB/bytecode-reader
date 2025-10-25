#include <string.h>
#include <stdlib.h>

#include "instructions.h"


const instr_data instruction_data[totalEnumOpCodes] = {
    [nop]               = {"nop", 0},
    [aconst_null]       = {"aconst_null", 0},

    [iconst_m1]         = {"iconst_m1", 0},
    [iconst_0]          = {"iconst_0", 0},
    [iconst_1]          = {"iconst_1", 0},
    [iconst_2]          = {"iconst_2", 0},
    [iconst_3]          = {"iconst_3", 0},
    [iconst_4]          = {"iconst_4", 0},
    [iconst_5]          = {"iconst_5", 0},
    [lconst_0]          = {"lconst_0", 0},
    [lconst_1]          = {"lconst_1", 0},
    [fconst_0]          = {"fconst_0", 0},
    [fconst_1]          = {"fconst_1", 0},
    [fconst_2]          = {"fconst_2", 0},
    [dconst_0]          = {"dconst_0", 0},
    [dconst_1]          = {"dconst_1", 0},

    [bipush]            = {"bipush", 1},
    [sipush]            = {"sipush", 2},

    [ldc]               = {"ldc", 1},
    [ldc_w]             = {"ldc_w", 2},
    [ldc2_w]            = {"ldc2_w", 2},

    [iload]             = {"iload", 1},
    [lload]             = {"lload", 1},
    [fload]             = {"fload", 1},
    [dload]             = {"dload", 1},
    [aload]             = {"aload", 1},

    [iload_0]           = {"iload_0", 0},
    [iload_1]           = {"iload_1", 0},
    [iload_2]           = {"iload_2", 0},
    [iload_3]           = {"iload_3", 0},
    [lload_0]           = {"lload_0", 0},
    [lload_1]           = {"lload_1", 0},
    [lload_2]           = {"lload_2", 0},
    [lload_3]           = {"lload_3", 0},
    [fload_0]           = {"fload_0", 0},
    [fload_1]           = {"fload_1", 0},
    [fload_2]           = {"fload_2", 0},
    [fload_3]           = {"fload_3", 0},
    [dload_0]           = {"dload_0", 0},
    [dload_1]           = {"dload_1", 0},
    [dload_2]           = {"dload_2", 0},
    [dload_3]           = {"dload_3", 0},
    [aload_0]           = {"aload_0", 0},
    [aload_1]           = {"aload_1", 0},
    [aload_2]           = {"aload_2", 0},
    [aload_3]           = {"aload_3", 0},

    [iaload]            = {"iaload", 0},
    [laload]            = {"laload", 0},
    [faload]            = {"faload", 0},
    [daload]            = {"daload", 0},
    [aaload]            = {"aaload", 0},
    [baload]            = {"baload", 0},
    [caload]            = {"caload", 0},
    [saload]            = {"saload", 0},

    [istore]            = {"istore", 1},
    [lstore]            = {"lstore", 1},
    [fstore]            = {"fstore", 1},
    [dstore]            = {"dstore", 1},
    [astore]            = {"astore", 1},

    [istore_0]          = {"istore_0", 0},
    [istore_1]          = {"istore_1", 0},
    [istore_2]          = {"istore_2", 0},
    [istore_3]          = {"istore_3", 0},
    [lstore_0]          = {"lstore_0", 0},
    [lstore_1]          = {"lstore_1", 0},
    [lstore_2]          = {"lstore_2", 0},
    [lstore_3]          = {"lstore_3", 0},
    [fstore_0]          = {"fstore_0", 0},
    [fstore_1]          = {"fstore_1", 0},
    [fstore_2]          = {"fstore_2", 0},
    [fstore_3]          = {"fstore_3", 0},
    [dstore_0]          = {"dstore_0", 0},
    [dstore_1]          = {"dstore_1", 0},
    [dstore_2]          = {"dstore_2", 0},
    [dstore_3]          = {"dstore_3", 0},
    [astore_0]          = {"astore_0", 0},
    [astore_1]          = {"astore_1", 0},
    [astore_2]          = {"astore_2", 0},
    [astore_3]          = {"astore_3", 0},

    [iastore]           = {"iastore", 0},
    [lastore]           = {"lastore", 0},
    [fastore]           = {"fastore", 0},
    [dastore]           = {"dastore", 0},
    [aastore]           = {"aastore", 0},
    [bastore]           = {"bastore", 0},
    [castore]           = {"castore", 0},
    [sastore]           = {"sastore", 0},

    [pop]               = {"pop", 0},
    [pop2]              = {"pop2", 0},

    [dup]               = {"dup", 0},
    [dup_x1]            = {"dup_x1", 0},
    [dup_x2]            = {"dup_x2", 0},
    [dup2]              = {"dup2", 0},
    [dup2_x1]           = {"dup2_x1", 0},
    [dup2_x2]           = {"dup2_x2", 0},

    [swap]              = {"swap", 0},

    [iadd]              = {"iadd", 0},
    [ladd]              = {"ladd", 0},
    [fadd]              = {"fadd", 0},
    [dadd]              = {"dadd", 0},

    [isub]              = {"isub", 0},
    [lsub]              = {"lsub", 0},
    [fsub]              = {"fsub", 0},
    [dsub]              = {"dsub", 0},

    [imul]              = {"imul", 0},
    [lmul]              = {"lmul", 0},
    [fmul]              = {"fmul", 0},
    [dmul]              = {"dmul", 0},

    [idiv]              = {"idiv", 0},
    [ldiv_]              = {"ldiv_", 0},
    [fdiv]              = {"fdiv", 0},
    [ddiv]              = {"ddiv", 0},

    [irem]              = {"irem", 0},
    [lrem]              = {"lrem", 0},
    [frem]              = {"frem", 0},
    [drem]              = {"drem", 0},

    [ineg]              = {"ineg", 0},
    [lneg]              = {"lneg", 0},

    [fneg]              = {"fneg", 0},
    [dneg]              = {"dneg", 0},

    [ishl]              = {"ishl", 0},
    [lshl]              = {"lshl", 0},

    [ishr]              = {"ishr", 0},
    [lshr]              = {"lshr", 0},

    [iushr]             = {"iushr", 0},
    [lushr]             = {"lushr", 0},

    [iand]              = {"iand", 0},
    [land]              = {"land", 0},
    [ior]               = {"ior", 0},
    [lor]               = {"lor", 0},
    [ixor]              = {"ixor", 0},
    [lxor]              = {"lxor", 0},
    
    [iinc]              = {"iinc", 2},

    [i2l]               = {"i2l", 0},
    [i2f]               = {"i2f", 0},
    [i2d]               = {"i2d", 0},
    [l2i]               = {"l2i", 0},
    [l2f]               = {"l2f", 0},
    [l2d]               = {"l2d", 0},
    [f2i]               = {"f2i", 0},
    [f2l]               = {"f2l", 0},
    [f2d]               = {"f2d", 0},
    [d2i]               = {"d2i", 0},
    [d2l]               = {"d2l", 0},
    [d2f]               = {"d2f", 0},

    [i2b]               = {"i2b", 0},
    [i2c]               = {"i2c", 0},
    [i2s]               = {"i2s", 0},

    [lcmp]              = {"lcmp", 0},
    [fcmpl]             = {"fcmpl", 0},
    [fcmpg]             = {"fcmpg", 0},
    [dcmpl]             = {"dcmpl", 0},
    [dcmpg]             = {"dcmpg", 0},

    [ifeq]              = {"ifeq", 2},
    [ifne]              = {"ifne", 2},
    [iflt]              = {"iflt", 2},
    [ifge]              = {"ifge", 2},
    [ifgt]              = {"ifgt", 2},
    [ifle]              = {"ifle", 2},

    [if_icmpeq]         = {"if_icmpeq", 2},
    [if_icmpne]         = {"if_icmpne", 2},
    [if_icmplt]         = {"if_icmplt", 2},
    [if_icmpge]         = {"if_icmpge", 2},
    [if_icmpgt]         = {"if_icmpgt", 2},
    [if_icmple]         = {"if_icmple", 2},
    [if_acmpeq]         = {"if_acmpeq", 2},
    [if_acmpne]         = {"if_acmpne", 2},

    [goto_]             = {"goto_", 2},
    [jsr]               = {"jsr", 2},
    [ret]               = {"ret", 1},

    [tableswitch]       = {"tableswitch", 0},
    [lookupswitch]      = {"lookupswitch", 0},

    [ireturn]           = {"ireturn", 0},
    [lreturn]           = {"lreturn", 0},
    [freturn]           = {"freturn", 0},
    [dreturn]           = {"dreturn", 0},
    [areturn]           = {"areturn", 0},
    [return_]           = {"return_", 0},


    [getstatic]         = {"getstatic", 2},
    [putstatic]         = {"putstatic", 2},
    [getfield]          = {"getfield", 2},
    [putfield]          = {"putfield", 2},

    [invokevirtual]     = {"invokevirtual", 2},
    [invokespecial]     = {"invokespecial", 2},
    [invokestatic]      = {"invokestatic", 2},
    [invokeinterface]   = {"invokeinterface", 4},
    [invokedynamic]     = {"invokedynamic", 4},

    [new_]              = {"new_", 2},
    [newarray]          = {"newarray", 1},
    [anewarray]         = {"anewarray", 2},
    [arraylength]       = {"arraylength", 0},

    [athrow]            = {"athrow", 0},
    [checkcast]         = {"checkcast", 2},
    [instanceof_]       = {"instanceof_", 2},

    [monitorenter]      = {"monitorenter", 0},
    [monitorexit]       = {"monitorexit", 0},

    [wide]              = {"wide", 5}, 
    [multianewarray]    = {"multianewarray", 3},

    [ifnull]            = {"ifnull", 2},
    [ifnonnull]         = {"ifnonnull", 2},

    [goto_w]            = {"goto_w", 4},
    [jsr_w]             = {"jsr_w", 4} 
};

instruction* buildInstructions(void) {
    instruction *instructions = calloc(totalEnumOpCodes, sizeof(instruction));
    if (instructions == NULL) {
        return NULL;
    }

    for (u1 idx = 0; idx < totalEnumOpCodes; idx++) {
        const char* name = instruction_data[idx].name;

        if (name != NULL) {
            instructions[idx].name = name;
            instructions[idx].opcode = idx;
            instructions[idx].num_arguments = instruction_data[idx].num_arguments;
        
            if (instructions[idx].num_arguments > 0) {
                instructions[idx].argument_type = calloc(instructions[idx].num_arguments, sizeof(u4));
                
                if (instructions[idx].argument_type == NULL) {
                    return NULL;
                }
            } else {
                instructions[idx].argument_type = NULL;
            }
        } else {
            instructions[idx].name = "unknow";
            instructions[idx].opcode = UINT8_MAX;
            instructions[idx].num_arguments = UINT8_MAX;
            instructions[idx].argument_type = NULL;
        }
    } 

    return instructions;
}
