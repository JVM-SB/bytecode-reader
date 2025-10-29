// TODO: Implementar a exibição do arquivo de classe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "displayer.h"
#include "structures.h"

#define JAVA_1_0_2 4530000
#define JAVA_1_1 4565535
#define JAVA_1_2 4600000
#define JAVA_1_3 4700000
#define JAVA_1_4 4800000
#define JAVA_5_0 4900000
#define JAVA_6 5000000
#define JAVA_7 5100000
#define JAVA_8 5200000
#define JAVA_9 5300000
#define JAVA_10 5400000

typedef uint64_t u8;

// TODO
void displayClassFile(const ClassFile *cf) {

    if(cf == NULL) {
        fprintf(stderr, "Arquivo de classe nulo.\n");
        return;
    }

    printf("Magic: %X\n", cf->magic);
    printf("Versao Minor: %.0f\n", (float) cf->minor_version);
    int major_version = ((float) cf->major_version)*100000;
    switch(major_version) {
        case JAVA_1_0_2:
            printf("Versao Major: %0.f [1.0.2]\n", (float) cf->major_version);
            break;
        case JAVA_1_1:
            printf("Versao Major: %0.f [1.1]\n", (float) cf->major_version);
            break;
        case JAVA_1_2:
            printf("Versao Major: %0.f [1.2]\n", (float) cf->major_version);
            break;
        case JAVA_1_3:
            printf("Versao Major: %0.f [1.3]\n", (float) cf->major_version);
            break;
        case JAVA_1_4:
            printf("Versao Major: %0.f [1.4]\n", (float) cf->major_version);
            break;
        case JAVA_5_0:
            printf("Versao Major: %0.f [1.5].0\n", (float) cf->major_version);
            break;
        case JAVA_6:
            printf("Versao Major: %0.f [1.6]\n", (float) cf->major_version);
            break;
        case JAVA_7:
            printf("Versao Major: %0.f [1.7]\n", (float) cf->major_version);
            break;
        case JAVA_8:
            printf("Versao Major: %0.f [1.8]\n", (float) cf->major_version);
            break;
        case JAVA_9:
            printf("Versao Major: %0.f [1.9]\n", (float) cf->major_version);
            break;
        case JAVA_10:
            printf("Versao Major: %0.f [10]\n", (float) cf->major_version);
            break;
        default:
            printf("Versão java não compatível.\n");
            exit(1);
    }
    printf("Contador do Pool de Constantes: %.0f\n", (float) cf->constant_pool_count);
    printf("Flag de Acesso: ");
    switch (cf->access_flags) {
    case CLASS_ACC_PUBLIC:
        printf("PUBLIC\n");
        break;
    case CLASS_ACC_FINAL:
        printf("FINAL\n");
        break;
    case CLASS_ACC_SUPER:
        printf("SUPER\n");
        break;
    case CLASS_ACC_INTERFACE:
        printf("INTERFACE\n");
        break;
    case CLASS_ACC_ABSTRACT:
        printf("ABSTRACT\n");
        break;
    case CLASS_ACC_SYNTHETIC:
        printf("SYNTHETIC\n");
        break;
    case CLASS_ACC_ANNOTATION:
        printf("ANNOTATION\n");
        break;
    case CLASS_ACC_ENUM:
        printf("ENUM\n");
        break;
    case 0x0021:
        printf("ACC_PUBLIC, ACC_SUPER\n");
        break;
    default:
        printf("ERRO\n");
        break;
    }
    for (int i=0; i < cf->constant_pool_count-1; i++) {
        // printf("tag: %d\n", cf->constant_pool[i].tag);
        int index = 0;
        int index_1 = 0;
        int index_2 = 0;
        switch (cf->constant_pool[i].tag) {
            case CONSTANT_Class:
                printf("[%02d] CONSTANT_Class_info\n", i+1);
                index = cf->constant_pool[i].info.class_info.name_index;
                printf("    ⮡ Nome:\t\tcp_info #%d\t", index);
                printf("<%s>\n", cf->constant_pool[index-1].info.utf8_info.bytes);
                break;

            case CONSTANT_Fieldref:
                printf("[%02d] CONSTANT_Fieldref_info\n", i+1);
                index = cf->constant_pool[i].info.methodref_info.class_index;
                printf("    ⮡ Nome da Classe:\t\tcp_info #%d\t", index);
                index = cf->constant_pool[index-1].info.class_info.name_index;
                printf("<%s>\n", cf->constant_pool[index-1].info.utf8_info.bytes);
                index = cf->constant_pool[i].info.methodref_info.name_and_type_index;
                printf("    ⮡ Nome e Tipo:\t\tcp_info #%d\t", index);
                index_1 = cf->constant_pool[index-1].info.methodref_info.class_index;
                index_2 = cf->constant_pool[index-1].info.methodref_info.name_and_type_index;
                printf("<%s: ", cf->constant_pool[index_1-1].info.utf8_info.bytes);
                printf("%s>\n", cf->constant_pool[index_2-1].info.utf8_info.bytes);
                break;

            case CONSTANT_Methodref:
                printf("[%02d] CONSTANT_Methodref_info\n", i+1);
                index = cf->constant_pool[i].info.methodref_info.class_index;
                printf("    ⮡ Nome da Classe:\t\tcp_info #%d\t", index);
                index = cf->constant_pool[index-1].info.class_info.name_index;
                printf("<%s>\n", cf->constant_pool[index-1].info.utf8_info.bytes);
                index = cf->constant_pool[i].info.methodref_info.name_and_type_index;
                printf("    ⮡ Nome e Tipo:\t\tcp_info #%d\t", index);
                index_1 = cf->constant_pool[index-1].info.methodref_info.class_index;
                index_2 = cf->constant_pool[index-1].info.methodref_info.name_and_type_index;
                printf("<%s: ", cf->constant_pool[index_1-1].info.utf8_info.bytes);
                printf("%s>\n", cf->constant_pool[index_2-1].info.utf8_info.bytes);
                break;

            case CONSTANT_InterfaceMethodref:
                printf("[%02d] CONSTANT_InterfaceMethodref_info\n", i+1);
                index = cf->constant_pool[i].info.interface_methodref_info.class_index;
                printf("    ⮡ Nome da Classe:\t\tcp_info #%d\t", index);
                index = cf->constant_pool[index-1].info.class_info.name_index;
                printf("<%s>\n", cf->constant_pool[index-1].info.utf8_info.bytes);
                index = cf->constant_pool[i].info.interface_methodref_info.name_and_type_index;
                index_1 = cf->constant_pool[index-1].info.methodref_info.class_index;
                index_2 = cf->constant_pool[index-1].info.methodref_info.name_and_type_index;
                printf("    ⮡ Nome e Tipo:\t\tcp_info #%d\t", index);
                printf("<%s: ", cf->constant_pool[index_1-1].info.utf8_info.bytes);
                printf("%s>\n", cf->constant_pool[index_2-1].info.utf8_info.bytes);
                break;

            case CONSTANT_String:
                printf("[%02d] CONSTANT_String_info\n", i+1);
                index = cf->constant_pool[i].info.string_info.string_index;
                printf("    ⮡ String:\tcp_info #%d\t", index);
                printf("<%s> \n", cf->constant_pool[index-1].info.utf8_info.bytes);
                break;
            
            case CONSTANT_Integer:
                printf("[%02d] CONSTANT_Integer_info\n", i+1);
                printf("    ⮡ Bytes:\t%08x \n", cf->constant_pool[i].info.integer_info.bytes);
                printf("    ⮡ Inteiro:\t%d \n", cf->constant_pool[i].info.integer_info.bytes);
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
                break;

            case CONSTANT_NameAndType:
                printf("[%02d] CONSTANT_NameAndType_info\n", i+1);
                index = cf->constant_pool[i].info.nameandtype_info.name_index;
                printf("    ⮡ Nome:\t\tcp_info #%d\t", index);
                printf("<%s>\n", cf->constant_pool[index-1].info.utf8_info.bytes);
                index = cf->constant_pool[i].info.nameandtype_info.descriptor_index;
                printf("    ⮡ Descricao:\tcp_info #%d\t", index);
                printf("<%s>\n", cf->constant_pool[index-1].info.utf8_info.bytes);
                break;

            case CONSTANT_Utf8:
                printf("[%02d] CONSTANT_Utf8_info\n", i+1);
                printf("    ⮡ Tamanho do array de bytes:\t%d\n", cf->constant_pool[i].info.utf8_info.length);
                printf("    ⮡ Tamanho da string:\t\t%d\n", cf->constant_pool[i].info.utf8_info.length);
                printf("    ⮡ String:\t\t\t\t%s\n", cf->constant_pool[i].info.utf8_info.bytes);
                break;
            
            case CONSTANT_MethodHandle:
                // TODO
                // printf("%d\n", cf->constant_pool[i].info.methodhandle_info.reference_kind);
                // printf("%d\n", cf->constant_pool[i].info.methodhandle_info.reference_index);
                printf("[%02d] CONSTANT_MethodHandle_info\n", i+1);
                break;

            case CONSTANT_MethodType:
                // TODO
                // printf("%d\n", cf->constant_pool[i].info.methodtype_info.descriptor_index);
                printf("[%02d] CONSTANT_MethodType_info\n", i+1);
                break;

            case CONSTANT_InvokeDynamic:
                // TODO
                // printf("%d\n", cf->constant_pool[i].info.invokedynamic_info.bootstrap_method_attr_index);
                // printf("%d\n", cf->constant_pool[i].info.invokedynamic_info.name_and_type_index);
                printf("[%02d] CONSTANT_InvokeDynamic_info\n", i+1);
                break;

            default:
                fprintf(stderr, "Erro: Tag desconhecida: %d\n", cf->constant_pool[i].tag);
                // exit(1);
        }

    }

}