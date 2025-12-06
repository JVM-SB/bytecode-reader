#ifndef HELPERS_H
#define HELPERS_H

#include "structures.h"
#include "classfile.h"

int constantPoolStringIsEqual(const ClassFile *cf, u2 index, const char *str);

method_info* findMethod(const ClassFile *cf, const char *name, const char *descriptor);

code_attribute* findCodeAttribute(const ClassFile *cf, method_info *method);

#endif // HELPERS_H