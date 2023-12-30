#include "type_declaration_node.h"

type_extension_node *ten_create_node(int type, void *value){
    type_extension_node *node = (type_extension_node *) malloc(sizeof(struct type_extension_node));
    node->type = type;
    node->value = value;
    return node;
}

type_node *tn_create_node(int type, type_extension_node *extension){
    type_node *node = (type_node *) malloc(sizeof(type_node));
    node->type = type;
    node->extension = extension;
    return node;
}

type_declaration_node *td_create_node(char *id, type_node *type) {
    type_declaration_node *node = (type_declaration_node *) malloc(sizeof(type_declaration_node));
    node->id = id;
    node->type = type;
    return node;
}

int td_print_node(type_declaration_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"TYPE DECLARATION\n ID: %s\n\", shape=oval];\n", node_dot_id, node->id);
    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    available_dot_id = tn_print_node(node->type, ast_file, available_dot_id);

    return available_dot_id;
}

int tn_print_node(type_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"TYPE\n", node_dot_id);

    char type[10] = "";
    switch (node->type) {
        case BT_INT:
            strcpy(type, "int");
            break;
        case BT_VOID:
            strcpy(type, "void");
            break;
        case BT_CHAR:
            strcpy(type, "char");
            break;
    };
    fprintf(ast_file, "TYPE: %s", type);

    if(node->extension != NULL){ 
        switch (node->extension->type) {
            case TE_POINTERS:
                for(int* pointer_count = (int *) node->extension->value; *pointer_count > 0; *pointer_count = *pointer_count - 1){
                    fprintf(ast_file, "*");
                }
                fprintf(ast_file, "\n\" shape=oval];\n");
                break;
            case TE_ARRAYS:
                for(int i = list_get_size((list_node *) node->extension->value); i > 0; i--){
                    fprintf(ast_file, "[]");
                }
                fprintf(ast_file, "\n\" shape=oval];\n");
                for(list_node* current = (list_node *) node->extension->value; current != NULL; current = current->next){
                    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
                    available_dot_id = exp_print_node(current->value, ast_file, available_dot_id);
                }
                break;
        }
    }
    else {
        fprintf(ast_file, "\n\" shape=oval];\n");
    }

    return available_dot_id;
}

void td_generate_code_data_section_node(type_declaration_node *node) {
    if(node->type->extension != NULL){
        int size;
        switch (node->type->extension->type) {
            case TE_POINTERS:
                printf("\t%s: .word 0\n", node->id);
                break;
            case TE_ARRAYS:
                size = node->type->type == BT_CHAR ? 1 : 4;
                for(list_node* current = (list_node *) node->type->extension->value; current != NULL; current = current->next){
                    size = size * exp_evaluate_node(current->value);
                }
                printf("\t%s: .space %i\n", node->id, size);
                break;
            default:
                break;
        }
    }
    else {
        switch (node->type->type) {
            case BT_INT:
                printf("\t%s: .word 0\n", node->id);
                break;
            case BT_CHAR:
                printf("\t%s: .byte 0\n", node->id);
                break;
            default:
                break;
        }
    }

    symbol *symbol = st_create_symbol(SYMB_GLOBAL, node->id, node);
    st_push(GET_CURRENT_CONTEXT(), symbol);
}

char *td_get_id(type_declaration_node *node) {
    return node->id;
}

int td_generate_code_global_offset_node(type_declaration_node *node) {
    printf("#\t\t\tACESSO A MEMÓRIA\n");

    int offset_reg = cm_get_available_temp_register();
    MIPS_ADDI(offset_reg, 0, 0);

    if(node->type->extension == NULL) {
        printf("#\t\t\tSEM OFFSET\n");
        return offset_reg;
    }

    int size_reg = cm_get_available_temp_register();
    int size = 1, pos = -1;

    switch (node->type->type) {
        case BT_INT:
            MIPS_ADDI(size_reg, 0, 4);
            break;
        case BT_CHAR:
            MIPS_ADDI(size_reg, 0, 1);
            break;
        default:
            MIPS_ADDI(size_reg, 0, 1);
            break;
    }

    switch (node->type->extension->type) {
        case TE_POINTERS:
            printf("#\t\t\tPONTEIROS WTF\n");
            break;
        case TE_ARRAYS:
            printf("#\t\t\tCOM OFFSET\n");
            for (list_node *current = (list_node *) node->type->extension->value;
                 current != NULL; current = current->next) {
                pos = exp_generate_code_node(current->value);
                MIPS_MUL(pos, pos, size_reg);
                MIPS_ADDI(offset_reg, offset_reg, pos);
                cm_free_register(pos);
            }
            break;
        default:
            printf("#\t\t\tNULL WTF\n");
            break;
    }

    cm_free_register(size_reg);
    return offset_reg;
}