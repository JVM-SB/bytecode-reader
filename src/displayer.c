#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "displayer.h"
#include "structures.h"
#include "instructions.h"

#define JAVA_1_0_2 453
#define JAVA_1_1 4565535
#define JAVA_1_2 46
#define JAVA_1_3 47
#define JAVA_1_4 48
#define JAVA_5_0 49
#define JAVA_6 50
#define JAVA_7 51
#define JAVA_8 52
#define JAVA_9 53
#define JAVA_10 54

typedef uint64_t u8;

static u2 read_u2_from_code(const u1 *code, u4 pc) {
    return ((u2)code[pc] << 8) | code[pc+1];
}

static u4 read_u4_from_code(const u1 *code, u4 pc) {
    return ((u4)code[pc]   << 24) |
           ((u4)code[pc+1] << 16) |
           ((u4)code[pc+2] << 8)  |
           code[pc+3];
}

static void print_cp_utf8(const ClassFile *cf, u2 index) {
    if (index == 0 || index >= cf->constant_pool_count) {
        printf("[Indice CP Invalido: %u]", index);
        return;
    }
    const cp_info *cp = &cf->constant_pool[index - 1];
    if (cp->tag != CONSTANT_Utf8) {
        printf("[Indice CP #%u nao e UTF-8 (tag: %d)]", index, cp->tag);
        return;
    }
    printf("%.*s", cp->info.utf8_info.length, cp->info.utf8_info.bytes);
}

static void displayCode(const ClassFile *cf, const code_attribute *code_attr) {
    printf("      Code:\n");
    printf("        max_stack = %u, max_locals = %u, code_length = %u\n",
           code_attr->max_stack, code_attr->max_locals, code_attr->code_length);

    u4 pc = 0; // Program Counter
    while (pc < code_attr->code_length) {
        u1 opcode = code_attr->code[pc];
        const char *mnemonic = instruction_data[opcode].name;
        int num_args = instruction_data[opcode].num_arguments;
        
        int pc_increment = 1 + num_args; 

        printf("        %4u: %s", pc, mnemonic);

       
        if (num_args == 1) {
            u1 arg1 = code_attr->code[pc + 1];

            if (opcode == ldc) {
                printf(" #%d",(int8_t) arg1); 

                const cp_info *ref_info = &cf->constant_pool[arg1 - 1];

                if (ref_info->tag == CONSTANT_Integer) {
                    printf(" <%u>", ref_info->info.integer_info.bytes); 
                } 
                else if (ref_info->tag == CONSTANT_Float) {
                    float f;
                    memcpy(&f, &ref_info->info.float_info.bytes, sizeof(float));
                    printf(" <%.1f>", f); 
                }
                else if (ref_info->tag == CONSTANT_String) {
                    printf(" // \"");
                    print_cp_utf8(cf, ref_info->info.string_info.string_index);
                    printf("\"");
                } else {
                    printf(" [CP item ?]");
                }
            
            } else if (opcode == newarray) {
                printf(" %d", (int8_t)arg1); 
                printf(" // "); 
                switch (arg1) {
                    case T_BOOLEAN: printf("(boolean)"); break;
                    case T_CHAR:    printf("(char)"); break;
                    case T_FLOAT:   printf("(float)"); break;
                    case T_DOUBLE:  printf("(double)"); break;
                    case T_BYTE:    printf("(byte)"); break;
                    case T_SHORT:   printf("(short)"); break;
                    case T_INT:     printf("(int)"); break;
                    case T_LONG:    printf("(long)"); break;
                    default:        printf("[Tipo de array desconhecido]"); break;
                }
            
            } else {
                printf(" %d", (int8_t)arg1);
            }
            
            printf("\n");

        } 
        else if (num_args == 2) {
            
            if (opcode == iinc) {
                u1 index = code_attr->code[pc + 1];
                int8_t constant = (int8_t)code_attr->code[pc + 2];
                printf(" %u by %d", index, constant);
                printf("\n");
            
            } else { 
                u2 arg = read_u2_from_code(code_attr->code, pc + 1); 

                int is_branch = (opcode >= ifeq && opcode <= jsr) || 
                                 opcode == ifnull || opcode == ifnonnull;
                
                int is_cp_ref = (opcode == getstatic || opcode == putstatic || opcode == getfield || opcode == putfield ||
                                  opcode == invokevirtual || opcode == invokespecial || opcode == invokestatic ||
                                  opcode == new_ || opcode == anewarray || opcode == checkcast || opcode == instanceof_ ||
                                  opcode == ldc_w || opcode == ldc2_w);

                if (is_branch) {
                    int16_t offset = (int16_t)arg;
                    u4 target_pc = pc + offset;

                    printf(" %u (%+d)", target_pc, offset);
                    printf("\n");

                } else if (is_cp_ref) {
                    printf(" #%u", arg);
                    
                    printf(" ");
                    
                    const cp_info *ref_info = &cf->constant_pool[arg - 1];
                    
                    if (opcode == ldc_w) {
                        if (ref_info->tag == CONSTANT_Integer) {
                            printf("<%u>", ref_info->info.integer_info.bytes);
                        } else if (ref_info->tag == CONSTANT_Float) {
                            float f;
                            memcpy(&f, &ref_info->info.float_info.bytes, sizeof(float));
                            printf("<%f>", f);
                        } else if (ref_info->tag == CONSTANT_String) {
                            printf("// \"");
                            print_cp_utf8(cf, ref_info->info.string_info.string_index);
                            printf("\"");
                        } else if (ref_info->tag == CONSTANT_Class) {
                            printf("// Class ");
                            print_cp_utf8(cf, ref_info->info.class_info.name_index);
                        } else {
                            printf(" // [CP Item ?]");
                        }
                    } else if (opcode == ldc2_w) {
                            if (ref_info->tag == CONSTANT_Long) {
                            long l;
                            u8 long_word = ((u8)ref_info->info.long_info.high_bytes << 32) | ref_info->info.long_info.low_bytes;
                            memcpy(&l, &long_word, sizeof(long));
                            printf("<%ldL>", l);
                        } else if (ref_info->tag == CONSTANT_Double) {
                            double d;
                            u8 double_word = ((u8)ref_info->info.double_info.high_bytes << 32) | ref_info->info.double_info.low_bytes;
                            memcpy(&d, &double_word, sizeof(double));
                            printf("<%lfd>", d); 
                        } else {
                            printf(" // [CP Item Long/Double ?]");
                        }
                    
                    } else if (ref_info->tag == CONSTANT_Fieldref || ref_info->tag == CONSTANT_Methodref || ref_info->tag == CONSTANT_InterfaceMethodref) {
                        printf("// ");
                        u2 class_idx = ref_info->info.methodref_info.class_index;
                        u2 nat_idx = ref_info->info.methodref_info.name_and_type_index;
                        u2 class_name_idx = cf->constant_pool[class_idx - 1].info.class_info.name_index;
                        u2 name_idx = cf->constant_pool[nat_idx - 1].info.nameandtype_info.name_index;
                        u2 desc_idx = cf->constant_pool[nat_idx - 1].info.nameandtype_info.descriptor_index;
                        
                        print_cp_utf8(cf, class_name_idx);
                        printf(".");
                        print_cp_utf8(cf, name_idx);
                        printf(":");
                        print_cp_utf8(cf, desc_idx);

                    } else if (ref_info->tag == CONSTANT_Class) {
                        printf("// ");
                        print_cp_utf8(cf, ref_info->info.class_info.name_index);
                    } else if (ref_info->tag == CONSTANT_String) {
                        printf("// \"");
                        print_cp_utf8(cf, ref_info->info.string_info.string_index);
                        printf("\"");
                    }
                    printf("\n");
                
                } else {
                    printf(" %d", (int32_t)arg);
                    printf("\n");
                }
            }
        
        } 
        else if (num_args == 4) {
             u4 arg = read_u4_from_code(code_attr->code, pc + 1);
             
             if (opcode == goto_w || opcode == jsr_w) {
                int32_t offset = (int32_t)arg;
                u4 target_pc = pc + offset;
                printf(" %u (%+d)", target_pc, offset); 
             
             } else if (opcode == invokeinterface) {
                u2 index = (arg >> 16) & 0xFFFF;
                u1 count = (arg >> 8) & 0xFF;
                u1 zero = arg & 0xFF;
                printf(" #%u, %u, %u", index, count, zero);
             
             } else if (opcode == invokedynamic) {
                u2 index = (arg >> 16) & 0xFFFF; 
                u1 zero1 = (arg >> 8) & 0xFF;    
                u1 zero2 = arg & 0xFF;           
                printf(" #%u, %u, %u", index, zero1, zero2);

             } else {
                 printf(" %d", (int32_t)arg);
             }
            printf("\n");
        
        } else if (opcode == tableswitch) {
            printf("\n");

            u4 padding = (4 - ((pc + 1) % 4)) % 4;
            u4 read_head = pc + 1 + padding;

            int32_t default_offset = (int32_t)read_u4_from_code(code_attr->code, read_head);
            read_head += 4;
            int32_t low = (int32_t)read_u4_from_code(code_attr->code, read_head);
            read_head += 4;
            int32_t high = (int32_t)read_u4_from_code(code_attr->code, read_head);
            read_head += 4;
            
            printf("            default = %d (%+d)\n", (int32_t)(pc + default_offset), default_offset);
            printf("            low = %d, high = %d\n", low, high);

            int32_t num_offsets = high - low + 1;
            if (num_offsets < 0) {
                printf("            [Erro: high (%d) < low (%d)]\n", high, low);
                num_offsets = 0;
            }

            for (int32_t i = 0; i < num_offsets; i++) {
                int32_t offset = (int32_t)read_u4_from_code(code_attr->code, read_head);
                printf("            %d: %d (%+d)\n", low + i, (int32_t)(pc + offset), offset);
                read_head += 4;
            }

            pc_increment = read_head - pc;
            
        } else if (opcode == lookupswitch) {
            printf("\n");

            u4 padding = (4 - ((pc + 1) % 4)) % 4;
            u4 read_head = pc + 1 + padding;

            int32_t default_offset = (int32_t)read_u4_from_code(code_attr->code, read_head);
            read_head += 4;
            int32_t npairs = (int32_t)read_u4_from_code(code_attr->code, read_head);
            read_head += 4;

            printf("            default = %d (%+d)\n", (int32_t)(pc + default_offset), default_offset);
            printf("            npairs = %d\n", npairs);

            if (npairs < 0) {
                printf("            [Erro: npairs (%d) < 0]\n", npairs);
                npairs = 0;
            }

            for (int32_t i = 0; i < npairs; i++) {
                int32_t match = (int32_t)read_u4_from_code(code_attr->code, read_head);
                read_head += 4;
                int32_t offset = (int32_t)read_u4_from_code(code_attr->code, read_head);
                read_head += 4;
                printf("            %d: %d (%+d)\n", match, (int32_t)(pc + offset), offset);
            }

            pc_increment = read_head - pc;
        
        } else {
            printf("\n");
        }

        pc += pc_increment; 
    }
}

static void displayAttributes(const ClassFile *cf, attribute_info *attributes, u2 attributes_count, const char* indent) {
     for (u2 i = 0; i < attributes_count; i++) {
        printf("%sAtributo [%u]: ", indent, i);
        
        const cp_info *cp_name = &cf->constant_pool[attributes[i].attribute_name_index - 1];
        if (cp_name->tag != CONSTANT_Utf8) {
             printf("[Nome Invalido]\n");
             continue;
        }
        print_cp_utf8(cf, attributes[i].attribute_name_index);
        printf(" (length: %u)\n", attributes[i].attribute_length);

        u2 len = cp_name->info.utf8_info.length;
        const u1* bytes = cp_name->info.utf8_info.bytes;

        if (strncmp((const char*)bytes, "Code", len) == 0 && len == 4) {
            displayCode(cf, (code_attribute*)attributes[i].info);
        }
     }
}


static void displayFields(const ClassFile *cf) {
    printf("\n---- Fields (%u) ----\n", cf->fields_count);
     for (u2 i = 0; i < cf->fields_count; i++) {
        printf("  Field [%u]: ", i);
        print_cp_utf8(cf, cf->fields[i].name_index);
        printf(" (Descritor: ");
        print_cp_utf8(cf, cf->fields[i].descriptor_index);
        printf(", Flags: 0x%04X)\n", cf->fields[i].access_flags);
        
        displayAttributes(cf, cf->fields[i].attributes, cf->fields[i].attributes_count, "    ");
    }
}


static void displayMethods(const ClassFile *cf) {
    printf("\n---- Methods (%u) ----\n", cf->methods_count);
    for (u2 i = 0; i < cf->methods_count; i++) {
        printf("  Method [%u]: ", i);
        print_cp_utf8(cf, cf->methods[i].name_index);
        printf(" (Descritor: ");
        print_cp_utf8(cf, cf->methods[i].descriptor_index);
        printf(", Flags: 0x%04X)\n", cf->methods[i].access_flags);
        
        displayAttributes(cf, cf->methods[i].attributes, cf->methods[i].attributes_count, "    ");
    }
}


void displayClassFile(const ClassFile *cf) {

    if(cf == NULL) {
        fprintf(stderr, "Arquivo de classe nulo.\n");
        return;
    }

    printf("Magic: %X\n", cf->magic);
    printf("Versao Minor: %u\n", cf->minor_version);
    int major_version =  cf->major_version;
    switch(major_version) {
        case JAVA_1_0_2:
            printf("Versao Major: %u [1.0.2]\n", cf->major_version);
            break;
        case JAVA_1_1:
            printf("Versao Major: %u [1.1]\n", cf->major_version);
            break;
        case JAVA_1_2:
            printf("Versao Major: %u [1.2]\n", cf->major_version);
            break;
        case JAVA_1_3:
            printf("Versao Major: %u [1.3]\n", cf->major_version);
            break;
        case JAVA_1_4:
            printf("Versao Major: %u [1.4]\n", cf->major_version);
            break;
        case JAVA_5_0:
            printf("Versao Major: %u [1.5].0\n", cf->major_version);
            break;
        case JAVA_6:
            printf("Versao Major: %u [1.6]\n", cf->major_version);
            break;
        case JAVA_7:
            printf("Versao Major: %u [1.7]\n", cf->major_version);
            break;
        case JAVA_8:
            printf("Versao Major: %u [1.8]\n", cf->major_version);
            break;
        case JAVA_9:
            printf("Versao Major: %u [1.9]\n", cf->major_version);
            break;
        case JAVA_10:
            printf("Versao Major: %u [10]\n", cf->major_version);
            break;
        default:
            printf("Versão java não compatível.\n");
            break;
    }
    printf("Contador do Pool de Constantes: %.0f\n", (float) cf->constant_pool_count);
    printf("Flag de Acesso: ");
    switch (cf->access_flags) {
    case CLASS_ACC_PUBLIC:
        printf("PUBLIC [0x0001]\n");
        break;
    case CLASS_ACC_FINAL:
        printf("FINAL [0x0010]\n");
        break;
    case CLASS_ACC_SUPER:
        printf("SUPER [0x0020]\n");
        break;
    case CLASS_ACC_INTERFACE:
        printf("INTERFACE [0x0200]\n");
        break;
    case CLASS_ACC_ABSTRACT:
        printf("ABSTRACT [0x0400]\n");
        break;
    case CLASS_ACC_SYNTHETIC:
        printf("SYNTHETIC [0x1000]\n");
        break;
    case CLASS_ACC_ANNOTATION:
        printf("ANNOTATION [0x2000]\n");
        break;
    case CLASS_ACC_ENUM:
        printf("ENUM [0x4000]\n");
        break;
    case 0x0021:
        printf("PUBLIC [0x0021]\n");
        break;
    default:
        printf("ERRO\n");
        break;
    }

    printf("\nThis Class: #%u // ", cf->this_class);
    if (cf->this_class > 0) {
        print_cp_utf8(cf, cf->constant_pool[cf->this_class - 1].info.class_info.name_index);
    }
    printf("\n");
    
    printf("Super Class: #%u // ", cf->super_class);
    if (cf->super_class > 0) {
         print_cp_utf8(cf, cf->constant_pool[cf->super_class - 1].info.class_info.name_index);
    } else {
        printf("[Nenhuma/java/lang/Object]");
    }
    printf("\n");

    printf("\n---- Constant Pool (%u) ----\n", cf->constant_pool_count);
    for (int i=0; i < cf->constant_pool_count-1; i++) {
        int index = 0;
        int index_1 = 0;
        int index_2 = 0;
        switch (cf->constant_pool[i].tag) {
            case CONSTANT_Class:
                printf("[%02d] CONSTANT_Class_info\n", i+1);
                index = cf->constant_pool[i].info.class_info.name_index;
                printf("    ⮡ Nome:\t\tcp_info #%d\t<", index);

                print_cp_utf8(cf, index);
                printf(">\n");
                break;

            case CONSTANT_Fieldref:
                printf("[%02d] CONSTANT_Fieldref_info\n", i+1);
                index = cf->constant_pool[i].info.methodref_info.class_index;
                printf("    ⮡ Nome da Classe:\t\tcp_info #%d\t<", index);
                index_1 = cf->constant_pool[index-1].info.class_info.name_index;
                print_cp_utf8(cf, index_1);
                printf(">\n");
                index = cf->constant_pool[i].info.methodref_info.name_and_type_index;
                printf("    ⮡ Nome e Tipo:\t\tcp_info #%d\t<", index);
                index_1 = cf->constant_pool[index-1].info.nameandtype_info.name_index;
                index_2 = cf->constant_pool[index-1].info.nameandtype_info.descriptor_index;
                print_cp_utf8(cf, index_1);
                printf(": ");
                print_cp_utf8(cf, index_2);
                printf(">\n");
                break;

            case CONSTANT_Methodref:
                printf("[%02d] CONSTANT_Methodref_info\n", i+1);
                index = cf->constant_pool[i].info.methodref_info.class_index;
                printf("    ⮡ Nome da Classe:\t\tcp_info #%d\t<", index);
                index_1 = cf->constant_pool[index-1].info.class_info.name_index;
                print_cp_utf8(cf, index_1);
                printf(">\n");
                index = cf->constant_pool[i].info.methodref_info.name_and_type_index;
                printf("    ⮡ Nome e Tipo:\t\tcp_info #%d\t<", index);
                index_1 = cf->constant_pool[index-1].info.nameandtype_info.name_index;
                index_2 = cf->constant_pool[index-1].info.nameandtype_info.descriptor_index;
                print_cp_utf8(cf, index_1);
                printf(": ");
                print_cp_utf8(cf, index_2);
                printf(">\n");
                break;

            case CONSTANT_InterfaceMethodref:
                printf("[%02d] CONSTANT_InterfaceMethodref_info\n", i+1);
                index = cf->constant_pool[i].info.interface_methodref_info.class_index;
                printf("    ⮡ Nome da Classe:\t\tcp_info #%d\t<", index);
                index_1 = cf->constant_pool[index-1].info.class_info.name_index;
                print_cp_utf8(cf, index_1);
                printf(">\n");
                index = cf->constant_pool[i].info.interface_methodref_info.name_and_type_index;
                printf("    ⮡ Nome e Tipo:\t\tcp_info #%d\t<", index);
                index_1 = cf->constant_pool[index-1].info.nameandtype_info.name_index;
                index_2 = cf->constant_pool[index-1].info.nameandtype_info.descriptor_index;
                print_cp_utf8(cf, index_1);
                printf(": ");
                print_cp_utf8(cf, index_2);
                printf(">\n");
                break;

            case CONSTANT_String:
                printf("[%02d] CONSTANT_String_info\n", i+1);
                index = cf->constant_pool[i].info.string_info.string_index;
                printf("    ⮡ String:\tcp_info #%d\t<", index);
                print_cp_utf8(cf, index);
                printf("> \n");
                break;
            
            // (Integer, Float, Long, Double continuam iguais)
            case CONSTANT_Integer:
                printf("[%02d] CONSTANT_Integer_info\n", i+1);
                printf("    ⮡ Bytes:\t%08x \n", cf->constant_pool[i].info.integer_info.bytes);
                printf("    ⮡ Inteiro:\t%u \n", cf->constant_pool[i].info.integer_info.bytes);
                break;
            
            case CONSTANT_Float:
                printf("[%02d] CONSTANT_Float_info\n", i+1);
                printf("    ⮡ Bytes:\t0x%08x \n", cf->constant_pool[i].info.float_info.bytes);
                float numero_float;
                memcpy(&numero_float, &cf->constant_pool[i].info.float_info.bytes, sizeof(numero_float));
                printf("    ⮡ Float:\t%.2f \n", numero_float);
                break;
            
            case CONSTANT_Long:
                printf("[%02d] CONSTANT_Long_info\n", i+1);
                printf("    ⮡ Bytes Altos:\t0x%08x \n", cf->constant_pool[i].info.long_info.high_bytes);
                printf("    ⮡ Bytes Baixos:\t0x%08x \n", cf->constant_pool[i].info.long_info.low_bytes);
                long meu_long;
                meu_long = ((u8)cf->constant_pool[i].info.long_info.high_bytes) << 32;
                meu_long |=  cf->constant_pool[i].info.long_info.low_bytes;
                printf("    ⮡ Long: %ld\n", meu_long);
                i++; // Long ocupa 2 espaços no CP
                break;
            
            case CONSTANT_Double:
                printf("[%02d] CONSTANT_Double_info\n", i+1);
                printf("    ⮡ Bytes Altos:\t0x%08x \n", cf->constant_pool[i].info.double_info.high_bytes);
                printf("    ⮡ Bytes Baixos:\t0x%08x \n", cf->constant_pool[i].info.double_info.low_bytes);
                u8 double_word;
                double meu_double;
                double_word = ((u8)cf->constant_pool[i].info.double_info.high_bytes) << 32;
                double_word |=  cf->constant_pool[i].info.double_info.low_bytes;
                memcpy(&meu_double, &double_word, sizeof(double));
                printf("    ⮡ Double: %.2lf\n", meu_double);
                i++; // Double ocupa 2 espaços no CP
                break;

            case CONSTANT_NameAndType:
                printf("[%02d] CONSTANT_NameAndType_info\n", i+1);
                index = cf->constant_pool[i].info.nameandtype_info.name_index;
                printf("    ⮡ Nome:\t\tcp_info #%d\t<", index);
                print_cp_utf8(cf, index);
                printf(">\n");
                index = cf->constant_pool[i].info.nameandtype_info.descriptor_index;
                printf("    ⮡ Descricao:\tcp_info #%d\t<", index);
                print_cp_utf8(cf, index);
                printf(">\n");
                break;

            case CONSTANT_Utf8:
                printf("[%02d] CONSTANT_Utf8_info\n", i+1);
                printf("    ⮡ String:\t\t\t\t");
                print_cp_utf8(cf, i + 1);   // (passa o índice do CP)
                printf("\n");
                break;
            
            // (MethodHandle, MethodType, InvokeDynamic continuam iguais)
            case CONSTANT_MethodHandle:
                printf("[%02d] CONSTANT_MethodHandle_info\n", i+1);
                break;

            case CONSTANT_MethodType:
                printf("[%02d] CONSTANT_MethodType_info\n", i+1);
                break;

            case CONSTANT_InvokeDynamic:
                printf("[%02d] CONSTANT_InvokeDynamic_info\n", i+1);
                break;

            default:
                fprintf(stderr, "Erro: Tag desconhecida: %d\n", cf->constant_pool[i].tag);
        }
    }

    printf("\n---- Interfaces (%u) ----\n", cf->interfaces_count);
    for(u2 i = 0; i < cf->interfaces_count; i++) {
        printf("  Interface [%u]: #%u // ", i, cf->interfaces[i]);
        print_cp_utf8(cf, cf->constant_pool[cf->interfaces[i] - 1].info.class_info.name_index);
        printf("\n");
    }

    displayFields(cf);
    displayMethods(cf);

    printf("\n---- Class Attributes (%u) ----\n", cf->attributes_count);
    displayAttributes(cf, cf->attributes, cf->attributes_count, "  ");
    
    printf("\n---- Fim do Display ----\n");
}
