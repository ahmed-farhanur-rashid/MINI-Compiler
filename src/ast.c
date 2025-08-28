#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"

// Helper function for safe malloc
static ASTNode *safe_malloc_node() {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for AST node\n");
        exit(1);
    }
    // Initialize all pointers to NULL
    memset(node, 0, sizeof(ASTNode));
    return node;
}

// Create a string concatenation node
ASTNode *make_string_concat_node(ASTNode *left, ASTNode *right) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_STRING_CONCAT;
    node->left = left;
    node->right = right;
    return node;
}

// Converts a double to a dynamically allocated string
char *toString(double val) {
    char *buffer = malloc(72);
    if (!buffer) return NULL;
    sprintf(buffer, "%.15g", val);
    
    // Remove trailing zeros
    char *p = buffer + strlen(buffer) - 1; // Gets the pointer to point to the last character in the buffer
    while(p > buffer && *p == '0' && p > strchr(buffer, '.')) *p-- = '\0';
    if(p > buffer && *p == '.') *p = '\0';
    
    return buffer;
}

// Convert string to number 
double toNumber(const char *str) {
    return atof(str);
}

ASTNode *make_number_node(double val) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_NUMBER;
    node->fval = val;
    return node;
}

ASTNode *make_string_node(char *val) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_STRING;
    node->sval = strdup(val);
    return node;
}

ASTNode *make_var_node(char *name) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_VAR;
    node->name = strdup(name);
    return node;
}

ASTNode *make_declaration_node(char *name, int vartype) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_DECL;
    node->name = strdup(name);
    node->fval = vartype;
    return node;
}

ASTNode *make_declaration_assign_node(char *name, ASTNode *expr, int vartype) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_DECL_ASSIGN;
    node->name = strdup(name);
    node->left = expr;
    node->fval = vartype;
    return node;
}

ASTNode *make_assign_node(char *name, ASTNode *expr) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_ASSIGN;
    node->name = strdup(name);
    node->left = expr;
    return node;
}

ASTNode *make_fetch_node(char *name) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_FETCH;
    node->name = strdup(name);
    return node;
}

ASTNode *make_display_node(char *name) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_DISPLAY;
    node->name = strdup(name);
    return node;
}

ASTNode *make_display_expression_node(ASTNode *expr) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_DISPLAY_EXPR;
    node->left = expr;
    return node;
}

ASTNode *make_print_node(char *name) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_PRINT;
    node->name = strdup(name);
    return node;
}

ASTNode *make_repeat_node(ASTNode *count_expr, ASTNode *body) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_REPEAT;
    node->left = count_expr;
    node->then_branch = body;
    return node;
}

ASTNode *make_when_node(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_WHEN;
    node->cond = cond;
    node->then_branch = then_branch;
    node->else_branch = else_branch;
    return node;
}

ASTNode *make_break_node() {
    ASTNode *node = safe_malloc_node();
    node->type = AST_BREAK;
    return node;
}

ASTNode *make_tostring_node(ASTNode *expr) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_TOSTRING;
    node->left = expr;
    return node;
}

ASTNode *make_tonumber_node(ASTNode *expr) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_TONUMBER;
    node->left = expr;
    return node;
}

ASTNode *make_binary_node(ASTType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = safe_malloc_node();
    node->type = type;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *append_statements(ASTNode *list, ASTNode *stmt) {
    if (!list) return stmt;
    ASTNode *current = list;
    while (current->next) {
        current = current->next;
    }
    current->next = stmt;
    return list;
}

// List function implementations
ASTNode *make_list_decl_node(char *name, int list_type, ASTNode *size_expr) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_LIST_DECL;
    node->name = strdup(name);
    node->list_type = list_type;
    node->size_expr = size_expr;
    return node;
}

ASTNode *make_list_access_node(char *name, ASTNode *index_expr) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_LIST_ACCESS;
    node->name = strdup(name);
    node->index_expr = index_expr;
    return node;
}

ASTNode *make_list_func_node(char *name, char *func_name, ASTNode *value_expr) {
    ASTNode *node = safe_malloc_node();
    node->type = AST_LIST_FUNC;
    node->name = strdup(name);
    node->func_name = strdup(func_name);
    node->value_expr = value_expr;
    return node;
}

void free_ast(ASTNode *node) {
    if (!node) return;
    if (node->name) free(node->name);
    if (node->sval) free(node->sval);
    if (node->func_name) free(node->func_name);
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->cond);
    free_ast(node->then_branch);
    free_ast(node->else_branch);
    free_ast(node->next);
    free_ast(node->size_expr);
    free_ast(node->index_expr);
    free_ast(node->value_expr);
    free(node);
}
