#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "attributes_reader.h"
#include "reader.h"


static verification_type_info* readVerificationTypeInfo(FILE *fp) {
    verification_type_info *info = calloc(1, sizeof(verification_type_info));
    if (info == NULL) {
        fprintf(stderr, "Erro ao alocar verification_type_info.\n");
        return NULL;
    }
    
    info->tag = u1Read(fp);
    switch (info->tag) {
        case 0: // ITEM_Top
        case 1: // ITEM_Integer
        case 2: // ITEM_Float
        case 3: // ITEM_Double
        case 4: // ITEM_Long
        case 5: // ITEM_Null
        case 6: // ITEM_UninitializedThis
            break;

        case 7: // ITEM_Object
            info->type_info.object_variable_info.cpool_index = u2Read(fp);
            break;
        case 8: // ITEM_Uninitialized
            info->type_info.uninitialized_variable_info.offset = u2Read(fp);
            break;

        default:
            fprintf(stderr, "Erro: Tag de verification_type_info desconhecida: %d\n", info->tag);
            free(info);
            return NULL;
    }

    return info;
}


static stack_map_frame* readStackMapFrame(FILE *fp) {
    stack_map_frame *frame = calloc(1, sizeof(stack_map_frame));
    if (frame == NULL) {
        fprintf(stderr, "Erro ao alocar stack_map_frame.\n");
        return NULL;
    }

    frame->frame_type = u1Read(fp);
    u1 type = frame->frame_type;

    if (type <= 63) { // SAME (0-63)
        
    } else if (type <= 127) { // SAME_LOCALS_1_STACK_ITEM (64-127)
        frame->map_frame_type.same_locals_1_stack_item_frame.stack = calloc(1, sizeof(verification_type_info*));
        frame->map_frame_type.same_locals_1_stack_item_frame.stack[0] = readVerificationTypeInfo(fp);
    
    } else if (type == 247) { // SAME_LOCALS_1_STACK_ITEM_EXTENDED (247)
        frame->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta = u2Read(fp);
        frame->map_frame_type.same_locals_1_stack_item_frame_extended.stack = calloc(1, sizeof(verification_type_info*));
        frame->map_frame_type.same_locals_1_stack_item_frame_extended.stack[0] = readVerificationTypeInfo(fp);
    
    } else if (type >= 248 && type <= 250) { // CHOP (248-250)
        frame->map_frame_type.chop_frame.offset_delta = u2Read(fp);
    
    } else if (type == 251) { // SAME_FRAME_EXTENDED (251)
        frame->map_frame_type.same_frame_extended.offset_delta = u2Read(fp);
    
    } else if (type >= 252 && type <= 254) { // APPEND (252-254)
        frame->map_frame_type.append_frame.offset_delta = u2Read(fp);

        u1 num_locals_to_append = type - 251; // k = frame_type - 251

        frame->map_frame_type.append_frame.locals = calloc(num_locals_to_append, sizeof(verification_type_info*));

        for (int i = 0; i < num_locals_to_append; i++) {
            frame->map_frame_type.append_frame.locals[i] = readVerificationTypeInfo(fp);
        }

    } else if (type == 255) { // FULL_FRAME (255)
        frame->map_frame_type.full_frame.offset_delta = u2Read(fp);
        
        frame->map_frame_type.full_frame.number_of_locals = u2Read(fp);
        u2 num_locals = frame->map_frame_type.full_frame.number_of_locals;

        if (num_locals > 0) {
            frame->map_frame_type.full_frame.locals = calloc(num_locals, sizeof(verification_type_info*));
            
            for (int i = 0; i < num_locals; i++) {
                frame->map_frame_type.full_frame.locals[i] = readVerificationTypeInfo(fp);
            }
        }

        frame->map_frame_type.full_frame.number_of_stack_items = u2Read(fp);
        u2 num_stack = frame->map_frame_type.full_frame.number_of_stack_items;

         if (num_stack > 0) {
            frame->map_frame_type.full_frame.stack = calloc(num_stack, sizeof(verification_type_info*));
           
            for (int i = 0; i < num_stack; i++) {
                frame->map_frame_type.full_frame.stack[i] = readVerificationTypeInfo(fp);
            }
         }

    } else {
        fprintf(stderr, "Erro: frame_type de stack_map_frame desconhecido: %d\n", type);
       
        free(frame);
        return NULL;
    }

    return frame;
}


stackMapTable_attribute* readStackMapTable(FILE *fp) {
    stackMapTable_attribute *table = calloc(1, sizeof(stackMapTable_attribute));
    if (table == NULL) {
        fprintf(stderr, "Erro ao alocar stackMapTable_attribute.\n");
        return NULL;
    }

    table->number_of_entries = u2Read(fp);
    if (table->number_of_entries > 0) {
        table->entries = calloc(table->number_of_entries, sizeof(stack_map_frame*));
        
        if (table->entries == NULL) {
            fprintf(stderr, "Erro ao alocar entries para StackMapTable.\n");
            free(table);
            return NULL;
        }

        for (u2 i = 0; i < table->number_of_entries; i++) {
            table->entries[i] = readStackMapFrame(fp);
            
            if (table->entries[i] == NULL) {
                // TODO: Liberar memória já alocada no loop
                free(table->entries);
                free(table);
                return NULL;
            }
        }

    } else {
        table->entries = NULL;
    
    }
    return table;
}


bootstrap_methods_attribute* readBootstrapMethods(FILE *fp) {
    bootstrap_methods_attribute *bsm_attr = calloc(1, sizeof(bootstrap_methods_attribute));
    if (bsm_attr == NULL) {
        fprintf(stderr, "Erro ao alocar bootstrap_methods_attribute.\n");
        return NULL;
    }
    
    bsm_attr->num_bootstrap_methods = u2Read(fp);
    u2 num_bsm = bsm_attr->num_bootstrap_methods;

    if (num_bsm > 0) {
        bsm_attr->bootstrap_methods = calloc(num_bsm, sizeof(*(bsm_attr->bootstrap_methods)));
        
        if (bsm_attr->bootstrap_methods == NULL) {
            fprintf(stderr, "Erro ao alocar bootstrap_methods array.\n");
            
            free(bsm_attr);
            return NULL;
        }

        for (u2 i = 0; i < num_bsm; i++) {
            bsm_attr->bootstrap_methods[i].bootstrap_method_ref = u2Read(fp);
            bsm_attr->bootstrap_methods[i].num_bootstrap_arguments = u2Read(fp);
            u2 num_args = bsm_attr->bootstrap_methods[i].num_bootstrap_arguments;
            
            if (num_args > 0) {
                bsm_attr->bootstrap_methods[i].bootstrap_arguments = calloc(num_args, sizeof(u2));
                
                if (bsm_attr->bootstrap_methods[i].bootstrap_arguments == NULL) {
                     fprintf(stderr, "Erro ao alocar bootstrap_arguments array.\n");
                     
                     for (u2 j = 0; j < i; j++) { 
                         free(bsm_attr->bootstrap_methods[j].bootstrap_arguments);
                     }

                     free(bsm_attr->bootstrap_methods);
                     free(bsm_attr);

                     return NULL;
                }

                for (u2 j = 0; j < num_args; j++) {
                    bsm_attr->bootstrap_methods[i].bootstrap_arguments[j] = u2Read(fp);
                }

            } else {
                bsm_attr->bootstrap_methods[i].bootstrap_arguments = NULL;
            }
        }

    } else {
        bsm_attr->bootstrap_methods = NULL;
    }

    return bsm_attr;
}
