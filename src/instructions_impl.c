#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <helpers.h>
#include "instructions_impl.h"

#define READ_U1(f) (f->bytecode[f->pc++])
#define READ_U2(f) (f->pc += 2, (f->bytecode[f->pc-2] << 8) | f->bytecode[f->pc-1])

void nop_impl(Frame *frame) {
    (void) frame;
    return;
}

// Placeholder
void unimpl(Frame *frame) {
    printf("Instrucao nao implementada (PC: %u)\n", frame->pc - 1);
    exit(-1);
}

// CONSTANTES 
void aconst_null_impl(Frame *frame) {
    pushOperand(frame, 0);
}

void iconst_m1_impl(Frame *frame) { 
    pushOperand(frame, -1);
}

void iconst_0_impl(Frame *frame)  { 
    pushOperand(frame, 0);
}

void iconst_1_impl(Frame *frame)  { 
    pushOperand(frame, 1);
}

void iconst_2_impl(Frame *frame)  { 
    pushOperand(frame, 2);
}

void iconst_3_impl(Frame *frame)  { 
    pushOperand(frame, 3);
}

void iconst_4_impl(Frame *frame)  { 
    pushOperand(frame, 4);
}

void iconst_5_impl(Frame *frame)  { 
    pushOperand(frame, 5);
}

void lconst_0_impl(Frame *frame) {
    pushOperand(frame, 0);          // High
    pushOperand(frame, 0);          // Low
}

void lconst_1_impl(Frame *frame) {
    pushOperand(frame, 0);
    pushOperand(frame, 1);
}

void bipush_impl(Frame *frame) {
    int8_t byte = (int8_t) READ_U1(frame);
    pushOperand(frame, (u4)byte);
}

void sipush_impl(Frame *frame) {
    int16_t sho = (int16_t) READ_U2(frame);
    pushOperand(frame, (u4)sho);
}


// LOADS

// Inteiros
void iload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);
}

void iload_0_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[0]);
}

void iload_1_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[1]);
}

void iload_2_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[2]);
}

void iload_3_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[3]);
}


// Float
void fload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);
}
void fload_0_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[0]);
}
void fload_1_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[1]);
}
void fload_2_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[2]);
}
void fload_3_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[3]);
}


// Long
void lload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);      // High
    pushOperand(frame, frame->local_variables[index + 1]);  // Low
}

void lload_0_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[0]);
    pushOperand(frame, frame->local_variables[1]);
}

void lload_1_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[1]);
    pushOperand(frame, frame->local_variables[2]);
}

void lload_2_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[2]);
    pushOperand(frame, frame->local_variables[3]);
}

void lload_3_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[3]);
    pushOperand(frame, frame->local_variables[4]);
}


// Double
void dload_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    pushOperand(frame, frame->local_variables[index]);      // High
    pushOperand(frame, frame->local_variables[index + 1]);  // Low
}

void dload_0_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[0]);
    pushOperand(frame, frame->local_variables[1]);
}

void dload_1_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[1]);
    pushOperand(frame, frame->local_variables[2]);
}

void dload_2_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[2]);
    pushOperand(frame, frame->local_variables[3]);
}

void dload_3_impl(Frame *frame) {
    pushOperand(frame, frame->local_variables[3]);
    pushOperand(frame, frame->local_variables[4]);
}

// STORES
void istore_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    frame->local_variables[index] = popOperand(frame);
}

void istore_0_impl(Frame *frame) {
    frame->local_variables[0] = popOperand(frame);
}

void istore_1_impl(Frame *frame) {
    frame->local_variables[1] = popOperand(frame);
}

void istore_2_impl(Frame *frame) {
    frame->local_variables[2] = popOperand(frame);
}

void istore_3_impl(Frame *frame) {
    frame->local_variables[3] = popOperand(frame);
}

void lstore_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    frame->local_variables[index + 1] = popOperand(frame); // Low
    frame->local_variables[index]     = popOperand(frame); // High
}

void lstore_0_impl(Frame *frame) {
    frame->local_variables[1] = popOperand(frame);
    frame->local_variables[0] = popOperand(frame);
}

void lstore_1_impl(Frame *frame) {
    frame->local_variables[2] = popOperand(frame);
    frame->local_variables[1] = popOperand(frame);
}

void lstore_2_impl(Frame *frame) {
    frame->local_variables[3] = popOperand(frame);
    frame->local_variables[2] = popOperand(frame);
}

void lstore_3_impl(Frame *frame) {
    frame->local_variables[4] = popOperand(frame);
    frame->local_variables[3] = popOperand(frame);
}

void dstore_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    frame->local_variables[index + 1] = popOperand(frame); // Low
    frame->local_variables[index]     = popOperand(frame); // High
}

void dstore_0_impl(Frame *frame) {
    frame->local_variables[1] = popOperand(frame);
    frame->local_variables[0] = popOperand(frame);
}

void dstore_1_impl(Frame *frame) {
    frame->local_variables[2] = popOperand(frame);
    frame->local_variables[1] = popOperand(frame);
}

void dstore_2_impl(Frame *frame) {
    frame->local_variables[3] = popOperand(frame);
    frame->local_variables[2] = popOperand(frame);
}

void dstore_3_impl(Frame *frame) {
    frame->local_variables[4] = popOperand(frame);
    frame->local_variables[3] = popOperand(frame);
}

void fstore_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    frame->local_variables[index] = popOperand(frame);
}
void fstore_0_impl(Frame *frame) {
    frame->local_variables[0] = popOperand(frame);
}
void fstore_1_impl(Frame *frame) {
    frame->local_variables[1] = popOperand(frame);
}
void fstore_2_impl(Frame *frame) {
    frame->local_variables[2] = popOperand(frame);
}
void fstore_3_impl(Frame *frame) {
    frame->local_variables[3] = popOperand(frame);
}

// Aritmética (int)
void iadd_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    u4 result = value1 + value2;

    pushOperand(frame, result);

    printf("IADD FEITO: %u + %u = %u", value1, value2, result);
}

void isub_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    u4 result = value1 - value2;

    pushOperand(frame, result);

    printf("ISUB FEITO: %u - %u = %u", value1, value2, result);
}

void imul_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    u4 result = value1 * value2;

    pushOperand(frame, result);
}

void idiv_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);
    
    if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u4 result = value1 / value2;
    
    pushOperand(frame, result);
}

void irem_impl(Frame *frame) {
    u4 value2 = (u4) popOperand(frame);
    u4 value1 = (u4) popOperand(frame);

    if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u4 result = value1 % value2;

    pushOperand(frame, result);
}

void ineg_impl(Frame *frame) {
    u4 value = (u4) popOperand(frame);

    u4 result = -value;

    pushOperand(frame, result);
}

// iinc
void iinc_impl(Frame *frame) {
    u1 index = READ_U1(frame);                  // Índice da variável local
    int8_t constant = (int8_t) READ_U1(frame);  // Valor a somar

    u4 value = (u4) frame->local_variables[index];
    value += constant;

    frame->local_variables[index] = (u4) value;
}

// LONGS
static u8 popLong(Frame *frame) {
    u4 low = popOperand(frame);
    u4 high = popOperand(frame);

    return ((u8)high << 32) | low;
}

static void pushLong(Frame *frame, u8 value) {
    u4 high = (u4)(value >> 32);
    u4 low = (u4)(value & 0xFFFFFFFF);

    pushOperand(frame, high);
    pushOperand(frame, low);
}

void ladd_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

    u8 result = value1 + value2;
    
    pushLong(frame, result);
}

void lsub_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

    u8 result = value1 - value2;

    pushLong(frame, result);
}

void lmul_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

    u8 result = value1 * value2;

    pushLong(frame, result);
}

void ldiv_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

     if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u8 result = value1 / value2;

    pushLong(frame, result);
}

void lrem_impl(Frame *frame) {
    u8 value2 = popLong(frame);
    u8 value1 = popLong(frame);

     if (value2 == 0) {
        fprintf(stderr, "Erro: Tentativa de dividir um número por zero (0)\n");
        exit(1);
    }

    u8 result = value1 % value2;

    pushLong(frame, result);
}

void lneg_impl(Frame *frame) {
    u8 value = popLong(frame);

    u8 result = -value;

    pushLong(frame, result);
}



// Return void
void return_impl(Frame *frame) {
    JVM *jvm = frame->jvm_ref;

    Frame *prev_frame = frame->previous;
    jvm->current_frame = prev_frame;

    deleteFrame(frame);
}


// Return com valor inteiro
void ireturn_impl(Frame *frame) {
    u4 return_value = popOperand(frame);
    printf(" >> IRETURN: %d\n", return_value); // Debug temporário
    JVM *jvm = frame->jvm_ref;
    Frame * prev_frame = frame->previous;

    if (prev_frame != NULL) {
        pushOperand(prev_frame, return_value);
    }

    jvm->current_frame = prev_frame;
    deleteFrame(frame);
}

// Instruções de Pilha

void pop_impl(Frame *frame) {
    popOperand(frame);
}

void pop2_impl(Frame *frame) {
    popOperand(frame);
    popOperand(frame);
}

void dup_impl(Frame *frame) {
    u4 value = peekOperand(frame);
    pushOperand(frame, value);
}

void dup_x1_impl(Frame *frame) {
    u4 value1 = popOperand(frame);
    u4 value2 = popOperand(frame);

    pushOperand(frame, value1);
    pushOperand(frame, value2);
    pushOperand(frame, value1);
}

void dup_x2_impl(Frame *frame) {
    u4 value1 = popOperand(frame);
    u4 value2 = popOperand(frame);
    u4 value3 = popOperand(frame);

    pushOperand(frame, value1);
    pushOperand(frame, value3);
    pushOperand(frame, value2);
    pushOperand(frame, value1);
}

void dup2_impl(Frame *frame) {
    u4 v1 = popOperand(frame);
    u4 v2 = peekOperand(frame);
    // desempilha tudo e empilha de volta
    u4 v2_val = popOperand(frame);
    
    pushOperand(frame, v2_val);
    pushOperand(frame, v1);
    pushOperand(frame, v2_val);
    pushOperand(frame, v1);
}

void swap_impl(Frame *frame) {
    u4 value1 = popOperand(frame);
    u4 value2 = popOperand(frame);

    pushOperand(frame, value1);
    pushOperand(frame, value2);
}

// Instruções de Comparação

void lcmp_impl(Frame *frame) {
    u8 v2 = popLong(frame);
    u8 v1 = popLong(frame);
    
    if ((int64_t)v1 > (int64_t)v2) pushOperand(frame, 1);
    else if ((int64_t)v1 < (int64_t)v2) pushOperand(frame, -1); // Representação de -1 em u4
    else pushOperand(frame, 0);
}

// Instruções de Desvio

// Auxiliar para calcular salto
static void branch(Frame *frame, int16_t offset) {
    // O PC atual aponta para DEPOIS dos argumentos da instrução de branch.
    // O offset é relativo ao opcode da instrução de branch.
    // Como os branchs (if, goto) têm 2 bytes de offset + 1 byte opcode = 3 bytes.
    // pc_opcode = frame->pc - 3;
    // novo_pc = pc_opcode + offset;
    u4 pc_opcode = frame->pc - 3;
    frame->pc = pc_opcode + offset;
}

void ifeq_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val == 0) branch(frame, offset);
}

void ifne_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val != 0) branch(frame, offset);
}

void iflt_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val < 0) branch(frame, offset);
}

void ifge_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val >= 0) branch(frame, offset);
}

void ifgt_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val > 0) branch(frame, offset);
}

void ifle_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 val = popOperand(frame);
    if ((int32_t)val <= 0) branch(frame, offset);
}

void if_icmpeq_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 == (int32_t)v2) branch(frame, offset);
}

void if_icmpne_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 != (int32_t)v2) branch(frame, offset);
}

void if_icmplt_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 < (int32_t)v2) branch(frame, offset);
}

void if_icmpge_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 >= (int32_t)v2) branch(frame, offset);
}

void if_icmpgt_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 > (int32_t)v2) branch(frame, offset);
}

void if_icmple_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    u4 v2 = popOperand(frame);
    u4 v1 = popOperand(frame);
    if ((int32_t)v1 <= (int32_t)v2) branch(frame, offset);
}

void goto_impl(Frame *frame) {
    int16_t offset = READ_U2(frame);
    branch(frame, offset);
}

// Instruções de Invocação e Campo Estático


// --- HELPER: Conta argumentos baseado no descritor (Ex: "(II)V" -> 2) ---
static int countArguments(const char *descriptor) {
    int count = 0;
    int i = 1; // Pula o '('
    while (descriptor[i] != ')') {
        if (descriptor[i] == 'L') {
            // Objetos (Ljava/lang/String;)
            count++;
            while (descriptor[i] != ';') i++;
        } else if (descriptor[i] == '[') {
            // Arrays
            while (descriptor[i] == '[') i++;
            if (descriptor[i] == 'L') while (descriptor[i] != ';') i++;
            count++;
        } else if (descriptor[i] == 'J' || descriptor[i] == 'D') {
            // Long e Double contam como 2 slots na pilha
            count += 2;
        } else {
            // Tipos primitivos (I, F, B, C, S, Z)
            count++;
        }
        i++;
    }
    return count;
}

// ----------------------------------------------------------------------------
// INSTRUÇÃO: INVOKESTATIC (0xB8)
// ----------------------------------------------------------------------------
void invokestatic_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;

    // 1. Resolver Método
    u2 name_type_index = cp[index-1].info.methodref_info.name_and_type_index;
    u2 name_idx = cp[name_type_index-1].info.nameandtype_info.name_index;
    u2 desc_idx = cp[name_type_index-1].info.nameandtype_info.descriptor_index;

    char *method_name = (char *)cp[name_idx-1].info.utf8_info.bytes;
    char *descriptor = (char *)cp[desc_idx-1].info.utf8_info.bytes;

    // 2. Buscar Método na Classe
    method_info *method = findMethod(frame->cf_ref, method_name);
    if (!method) {
        fprintf(stderr, "Erro: Metodo estatico '%s' nao encontrado.\n", method_name);
        exit(1);
    }

    code_attribute *code = findCodeAttribute(frame->cf_ref, method);

    // 3. Criar Novo Frame
    // O frame atual torna-se o 'previous' do novo frame
    Frame *newFrame = createFrame(frame->jvm_ref, frame->cf_ref, code, frame);

    // 4. Passar Argumentos da Pilha para Locais
    int num_args = countArguments(descriptor);
    
    // A pilha é LIFO, então os argumentos estão invertidos.
    // Buffer para inverter a ordem antes de colocar nas variaveis locais.
    if (num_args > 0) {
        u4 *args_buffer = malloc(sizeof(u4) * num_args);
        
        // Retira da pilha (Ordem inversa: argN, argN-1 ... arg1)
        for(int k = num_args - 1; k >= 0; k--) {
            args_buffer[k] = popOperand(frame);
        }
        
        // Coloca nas locais (Ordem correta: Var0=arg1, Var1=arg2...)
        for(int k = 0; k < num_args; k++) {
            newFrame->local_variables[k] = args_buffer[k];
        }
        free(args_buffer);
    }

    // 5. Troca de Contexto
    frame->jvm_ref->current_frame = newFrame;
}

// ----------------------------------------------------------------------------
// INSTRUÇÃO: GETSTATIC (0xB2) - Focada em System.out
// ----------------------------------------------------------------------------
void getstatic_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;

    // Apenas verificamos se é System.out para simular
    u2 class_index = cp[index-1].info.fieldref_info.class_index;
    u2 name_type_index = cp[index-1].info.fieldref_info.name_and_type_index;

    u2 class_name_idx = cp[class_index-1].info.class_info.name_index;
    u2 field_name_idx = cp[name_type_index-1].info.nameandtype_info.name_index;

    char *class_name = (char *)cp[class_name_idx-1].info.utf8_info.bytes;
    char *field_name = (char *)cp[field_name_idx-1].info.utf8_info.bytes;

    if (strcmp(class_name, "java/lang/System") == 0 && strcmp(field_name, "out") == 0) {
        // Empilha um valor dummy (0) para representar a referência do objeto PrintStream
        pushOperand(frame, 0); 
    } else {
        // Para outros campos estáticos, empilha 0 (suporte futuro necessário para static fields reais)
        pushOperand(frame, 0);
    }
}

void ldc_impl(Frame *frame) {
    u1 index = READ_U1(frame);
    cp_info *cp = frame->cf_ref->constant_pool;
    
    // O índice aponta para uma entrada no pool
    cp_info *item = &cp[index-1]; 

    if (item->tag == CONSTANT_Integer) {
        pushOperand(frame, item->info.integer_info.bytes);
    } 
    else if (item->tag == CONSTANT_Float) {
        pushOperand(frame, item->info.float_info.bytes);
    } 
    else if (item->tag == CONSTANT_String) {
        // Empilha o índice da String no Constant Pool.
        // O invokevirtual vai usar esse índice para buscar o texto.
        pushOperand(frame, index); 
    }
    else {
        fprintf(stderr, "Erro: ldc com tag nao suportada %d\n", item->tag);
        exit(1);
    }
}

void ldc2_w_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;
    cp_info *item = &cp[index-1];

    if (item->tag == CONSTANT_Long) {
        u8 val = ((u8)item->info.long_info.high_bytes << 32) | item->info.long_info.low_bytes;
        pushLong(frame, val);
    } 
    else if (item->tag == CONSTANT_Double) {
        u8 val = ((u8)item->info.double_info.high_bytes << 32) | item->info.double_info.low_bytes;
        pushLong(frame, val);
    }
    else {
        fprintf(stderr, "Erro: ldc2_w com tag invalida %d\n", item->tag);
        exit(1);
    }
}

// Invoke virtual mimetizando System.out.println e System.out.print

void invokevirtual_impl(Frame *frame) {
    u2 index = READ_U2(frame);
    cp_info *cp = frame->cf_ref->constant_pool;
    
    // ... resolve nome e tipo (mesma lógica anterior) ...
    u2 name_type_index = cp[index-1].info.methodref_info.name_and_type_index;
    u2 name_idx = cp[name_type_index-1].info.nameandtype_info.name_index;
    u2 desc_idx = cp[name_type_index-1].info.nameandtype_info.descriptor_index;
    
    char *method_name = (char *)cp[name_idx-1].info.utf8_info.bytes;
    char *descriptor = (char *)cp[desc_idx-1].info.utf8_info.bytes;

    if (strcmp(method_name, "println") == 0 || strcmp(method_name, "print") == 0) {
        
        if (strcmp(descriptor, "(Ljava/lang/String;)V") == 0) {
            // String: O valor na pilha é o índice no Constant Pool (vinda do LDC)
            u4 cp_index = popOperand(frame);
            popOperand(frame); // System.out
            // Busca a string real no CP
            cp_info *strInfo = &cp[cp_index-1];
            if (strInfo->tag == CONSTANT_String) {
                u2 utf8_idx = strInfo->info.string_info.string_index;
                char *strContent = (char *)cp[utf8_idx-1].info.utf8_info.bytes;
                printf("%s\n", strContent);
            } else {
                fprintf(stderr, "Erro: Esperado CONSTANT_String no CP para println, mas encontrou tag %d\n", strInfo->tag);
            }
        } 
        else if (strcmp(descriptor, "(I)V") == 0) { // Integer Print
            u4 val = popOperand(frame);
            popOperand(frame);
            printf("%d\n", (int32_t)val);
        }

        else if (strcmp(descriptor, "(Z)V") == 0) { // Boolean Print
            u4 val = popOperand(frame);
            popOperand(frame);
            printf("%s\n", val ? "true" : "false");
        }

        else if (strcmp(descriptor, "(D)V") == 0) { // Double Print
            u8 val = popLong(frame);
            popOperand(frame);
            double dval;
            memcpy(&dval, &val, sizeof(double));
            printf("%f\n", dval);
        }
        else if (strcmp(descriptor, "(J)V") == 0) { // Long Print
            u8 val = popLong(frame);
            popOperand(frame);
            printf("%lld\n", (long long)val);
        }
        else if (strcmp(descriptor, "(F)V") == 0) { // Float Print
            u4 val = popOperand(frame);
            popOperand(frame);
            float fval;
            memcpy(&fval, &val, sizeof(float));
            printf("%f\n", fval);
        }

        else {
            fprintf(stderr,"Metodo println/print com descritor nao suportado: %s\n", descriptor);
        }
    } else {
        fprintf(stderr, "Metodo virtual nao simulado: %s\n", method_name);
    }
}