#ifndef HELPERS_H
#define HELPERS_H
#define READ_U4(f) (f->pc += 4, \
    ((u4)f->bytecode[f->pc-4] << 24) | \
    ((u4)f->bytecode[f->pc-3] << 16) | \
    ((u4)f->bytecode[f->pc-2] << 8)  | \
    ((u4)f->bytecode[f->pc-1]))

#include "structures.h"
#include "classfile.h"

int countArguments(const char *descriptor);

int constantPoolStringIsEqual(const ClassFile *cf, u2 index, const char *str);

method_info* findMethod(const ClassFile *cf, const char *name, const char *descriptor);

code_attribute* findCodeAttribute(const ClassFile *cf, method_info *method);

#endif // HELPERS_H