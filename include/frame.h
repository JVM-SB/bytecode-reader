#ifndef FRAME_H
#define FRAME_H

#include "jvm_structures.h"

// Frame
Frame * createFrame(JVM *jvm, ClassFile *cf, code_attribute *code, Frame *prev);
void deleteFrame(Frame *frame);

// Pilha de operandos
void pushOperand(Frame *frame, u4 value);
u4 popOperand(Frame *frame);
u4 peekOperand(Frame *frame);

#endif // FRAME_H
