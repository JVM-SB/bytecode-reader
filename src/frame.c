#include <stdlib.h>
#include <stdio.h>

#include "frame.h"


Frame *createFrame(JVM *jvm, ClassFile *cf, code_attribute *code, Frame *prev) {
    Frame *frame = (Frame*) calloc(1, sizeof(Frame));

    if (frame == NULL) {
        fprintf(stderr, "Erro ao criar Frame!\n");
        exit(-1);
    }

    frame->jvm_ref = jvm;
    frame->pc = 0;
    frame->bytecode = code->code;
    frame->code_length = code->code_length;
    frame->cf_ref = cf;
    frame->previous = prev;
    frame->stack_pointer = 0;

    return frame;
}

void deleteFrame(Frame *frame) {
    if (frame) {
        free(frame);
    }
}

void pushOperand(Frame *frame, u4 value) {
    if (frame->stack_pointer >= MAX_OP_STACK) {
        fprintf(stderr, "Erro: Pilha de operandos cheias.\n");
        exit(-1);
    }

    frame->operand_stack[frame->stack_pointer++] = value;
}

u4 popOperand(Frame *frame) {
    if (frame->stack_pointer == 0) {
        fprintf(stderr, "Erro: Pilha de operandos vazia.\n");
        exit(-1);
    }

    return frame->operand_stack[--frame->stack_pointer];
}

u4 peekOperand(Frame *frame) {
     if (frame->stack_pointer == 0) {
        fprintf(stderr, "Pilha de operandos vazia.\n");
        exit(-1);
    }

    return frame->operand_stack[frame->stack_pointer - 1];
}
