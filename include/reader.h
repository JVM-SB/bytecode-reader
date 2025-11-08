#ifndef READER_H
#define READER_H

#include <stdio.h>

#include "attributes_reader.h"
#include "classfile.h"
#include "instructions.h"


// Leitores de Inteiros
u1 u1Read(FILE *file);
u2 u2Read(FILE *file);


// Leitores de Estruturas
ClassFile* readFile(const char *);

// Liberadores de Memória
void freeClassFile(ClassFile *cf);

#endif // READER_H
