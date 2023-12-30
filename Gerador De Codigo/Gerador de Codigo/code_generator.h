#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <stdio.h>

#include "macros.h"
#include "context_manager.h"

#include "data_structures/list/list.h"
#include "data_structures/stack/stack.h"
#include "data_structures/symbol_table/symbol_table.h"

#include "ast_nodes/expression_node/expression_node.h"
#include "ast_nodes/command_list_node/command_list_node.h"
#include "ast_nodes/function_call_node/function_call_node.h"
#include "ast_nodes/unary_operator_node/unary_operator_node.h"
#include "ast_nodes/binary_operator_node/binary_operator_node.h"
#include "ast_nodes/ternary_operator_node/ternary_operator_node.h"
#include "ast_nodes/type_declaration_node/type_declaration_node.h"
#include "ast_nodes/primary_expression_node/primary_expression_node.h"
#include "ast_nodes/program_declarations_node/program_declarations_node.h"
#include "ast_nodes/function_declarations_node/function_declarations_node.h"
#include "ast_nodes/constant_declarations_node/constant_declarations_node.h"

void cg_code_generate(void *ast);

void cg_print_ast(void *ast, FILE *ast_file);

#endif