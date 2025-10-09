#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "abs.c"

static u1 u1Read(FILE *file) {
    return getc(file);
}

static u2 u2Read(FILE *file) {
    u2 toReturn = getc(file);
    toReturn = (toReturn << 8) | getc(file);
    return toReturn;
}

static u4 u4Read(FILE *file) {
    u4 toReturn = getc(file);
    toReturn = (toReturn << 8) | getc(file);
    toReturn = (toReturn << 8) | getc(file);
    toReturn = (toReturn << 8) | getc(file);
    return toReturn;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <class file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }
    struct ClassFile *cf = (struct ClassFile *) malloc(sizeof(struct ClassFile));

    cf->magic = u4Read(file);
    printf("Magic read: 0x%X\n", cf->magic);
    cf->minor_version = u2Read(file);
    printf("Minor version read: %f\n", cf->minor_version);
    cf->major_version = u2Read(file);
    printf("Major version read: %f\n", cf->major_version);
    cf->constant_pool_count = u2Read(file);
    printf("Constant pool count read: %d\n", cf->constant_pool_count);

    struct cp_info *cp = (struct cp_info *) malloc((cf->constant_pool_count - 1) * sizeof(struct cp_info));
    for (int i = 1; i < cf->constant_pool_count; i++){
        cp->tag = u1Read(file);
        switch (cp->tag) {
            case CONSTANT_Class:
                printf("Reading CONSTANT_Class at index %d\n", i);
                cp->info.class_info.name_index = u2Read(file);
                break;
            case CONSTANT_Fieldref:
                printf("Reading CONSTANT_Fieldref at index %d\n", i);
                cp->info.fieldref_info.class_index = u2Read(file);
                cp->info.fieldref_info.name_and_type_index = u2Read(file);
                break;
            case CONSTANT_Methodref:
                printf("Reading CONSTANT_Methodref at index %d\n", i);
                cp->info.methodref_info.class_index = u2Read(file);
                cp->info.methodref_info.name_and_type_index = u2Read(file);
                break;
            case CONSTANT_InterfaceMethodref:
                printf("Reading CONSTANT_InterfaceMethodref at index %d\n", i);
                cp->info.interface_methodref_info.class_index = u2Read(file);
                cp->info.interface_methodref_info.name_and_type_index = u2Read(file);
                break;
            case CONSTANT_String:
                printf("Reading CONSTANT_String at index %d\n", i);
                cp->info.string_info.string_index = u2Read(file);
                break;
            case CONSTANT_Integer:
                printf("Reading CONSTANT_Integer at index %d\n", i);
                cp->info.integer_info.bytes = u4Read(file);
                break;
            case CONSTANT_Float:
                printf("Reading CONSTANT_Float at index %d\n", i);
                cp->info.float_info.bytes = u4Read(file);
                break;
            case CONSTANT_Long:
                printf("Reading CONSTANT_Long at index %d\n", i);
                cp->info.long_info.high_bytes = u4Read(file);
                cp->info.long_info.low_bytes = u4Read(file);
                i++;
                break;
            case CONSTANT_Double:
                printf("Reading CONSTANT_Double at index %d\n", i);
                cp->info.double_info.high_bytes = u4Read(file);
                cp->info.double_info.low_bytes = u4Read(file);
                i++;
                break;
            case CONSTANT_NameAndType:
                printf("Reading CONSTANT_NameAndType at index %d\n", i);
                cp->info.nameandtype_info.name_index = u2Read(file);
                cp->info.nameandtype_info.descriptor_index = u2Read(file);
                break;
            case CONSTANT_Utf8:
                printf("Reading CONSTANT_Utf8 at index %d\n", i);
                cp->info.utf8_info.length = u2Read(file);
                cp->info.utf8_info.bytes = (u1 *) malloc(cp->info.utf8_info.length * sizeof(u1));
                for (int j = 0; j < cp->info.utf8_info.length; j++) {
                    cp->info.utf8_info.bytes[j] = u1Read(file);
                }
                break;
            default:
                printf("Unknown constant pool tag: %d\n", cp->tag);
                free(cp);
                fclose(file);
                free(cf);
                return 1;
        }
    }
    cf->constant_pool = cp;
    cf->access_flags = u2Read(file);
    printf("Access flags read: 0x%X\n", cf->access_flags);
    cf->this_class = u2Read(file);
    printf("This class index read: %d\n", cf->this_class);
    cf->super_class = u2Read(file);
    printf("Super class index read: %d\n", cf->super_class);
    cf->interfaces_count = u2Read(file);
    printf("Interfaces count read: %d\n", cf->interfaces_count);

    fclose(file);
    free(cf);
    return 0;
}