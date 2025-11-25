#ifndef EXECUTION_ENGINE_H
#define EXECUTION_ENGINE_H

#include "jvm_structures.h"

// Definição do tipo ponteiro de função
typedef void (*instruction_func)(Frame *frame);

void initExecutionEngine();

void execute(JVM *jvm);

#endif // EXECUTION_ENGINE_H
