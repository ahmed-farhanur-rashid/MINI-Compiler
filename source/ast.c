#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *make_number_node(float val) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->fval = val;
    node->name = NULL;
    node->sval = NULL;
    node->left = node->right = node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *make_string_node(char *val) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_STRING;
    node->sval = strdup(val);
    node->name = NULL;
    node->left = node->right = node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *make_var_node(char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_VAR;
    node->name = strdup(name);
    node->sval = NULL;
    node->left = node->right = node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *make_declaration_node(char *name, int vartype) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_DECL;
    node->name = strdup(name);
    node->fval = vartype;
    node->sval = NULL;
    node->left = node->right = node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *make_declaration_assign_node(char *name, ASTNode *expr, int vartype) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_DECL_ASSIGN;
    node->name = strdup(name);
    node->left = expr;
    node->fval = vartype;
    node->sval = NULL;
    node->right = node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *make_fetch_node(char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FETCH;
    node->name = strdup(name);
    node->sval = NULL;
    node->left = node->right = node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *make_display_node(char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_DISPLAY;
    node->name = strdup(name);
    node->sval = NULL;
    node->left = node->right = node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *make_repeat_node(ASTNode *count_expr, ASTNode *body) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_REPEAT;
    node->left = count_expr;
    node->then_branch = body;
    node->name = NULL;
    node->sval = NULL;
    node->right = node->cond = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *make_when_node(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_WHEN;
    node->cond = cond;
    node->then_branch = then_branch;
    node->else_branch = else_branch;
    node->name = NULL;
    node->sval = NULL;
    node->left = node->right = node->next = NULL;
    return node;
}

ASTNode *make_break_node() {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BREAK;
    node->name = NULL;
    node->sval = NULL;
    node->left = node->right = node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *make_binary_node(ASTType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->left = left;
    node->right = right;
    node->name = NULL;
    node->sval = NULL;
    node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}

ASTNode *append_statements(ASTNode *list, ASTNode *stmt) {
    if (!list) return stmt;
    ASTNode *cur = list;
    while (cur->next) cur = cur->next;
    cur->next = stmt;
    return list;
}

ASTNode *make_assign_node(char *name, ASTNode *expr) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ASSIGN;
    node->name = strdup(name);
    node->left = expr;
    node->sval = NULL;
    node->right = node->cond = node->then_branch = node->else_branch = node->next = NULL;
    return node;
}


void free_ast(ASTNode *node) {
    if (!node) return;
    if (node->name) free(node->name);
    if (node->sval) free(node->sval);
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->cond);
    free_ast(node->then_branch);
    free_ast(node->else_branch);
    free_ast(node->next);
    free(node);
}
