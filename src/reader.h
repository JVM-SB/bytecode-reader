#ifndef READER_H
#define READER_H

#include <stdio.h>

#include "instructions.h"
#include "classfile.h"


// Leitores de Inteiros
u1 u1Read(FILE *file);
u2 u2Read(FILE *file);
u4 u4Read(FILE *file);


// Leitores de Estruturas
ClassFile* readFile(char *);
cp_info* readConstantPool(FILE *file, u2 cp_count);

u2* readInterfaces(FILE *file, u2 interfaces_count);

attribute_info * readAttributes (FILE *fp, cp_info *cp, u2 attributes_count);
field_info* readFields(FILE *file, cp_info *cp, u2 fields_count);
method_info* readMethods(FILE *file, cp_info *cp, u2 methods_count);

code_attribute* readCode(FILE *fp, cp_info *cp);
exceptions_attribute* readExceptionsAttribute(FILE *fp);
verification_type_info* readVerificationTypeInfo(FILE *fp);

constantValue_attribute* readConstantValue(FILE *fp);
stackMapTable_attribute* readStackMapTable(FILE *fp);
stack_map_frame* readStackMapFrame(FILE *fp);

exception_table* readExceptionTable (FILE *fp, u2 size);

classes* readClasses(FILE *fp);
innerClasses_attribute* readInnerClasses(FILE *fp, cp_info *cp);

signature_attribute* readSignature(FILE *fp);

line_number_table* readLineNumberTable(FILE *fp, cp_info *cp);

sourceFile_attribute* readSourceFile (FILE *fp);


// Decodificadores
char* decodeInstructionOpcode(cp_info *cp, u2 index, u2 cp_count);
char* decodeCode(cp_info *cp, u2 cp_count, u1 *code, u4 code_length, instruction* instructions);

int decodeIntegerInfo(cp_info *cp);
long long decodeLongInfo(cp_info * cp);

double decodeDoubleInfo(cp_info *cp);
float decodeFloatInfo(cp_info *cp);

char* decodeStringUTF8(cp_info *cp);

char* decodeAccessFlags(u2 flag);

// Função de Liberação de Memória
void freeClassFile(ClassFile *cf);

#endif // READER_H
