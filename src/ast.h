// ...existing code...
#ifndef AST_H
#define AST_H

typedef enum {
    AST_NUMBER,
    AST_STRING,
    AST_VAR,
    AST_DECL_ASSIGN,
    AST_DECL,
    AST_ASSIGN,  // new
    AST_FETCH,
    AST_DISPLAY,
    AST_DISPLAY_EXPR,
    AST_PRINT,
    AST_REPEAT,
    AST_WHEN,
    AST_BREAK,
    AST_TOSTRING,
    AST_TONUMBER,
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,
    AST_EQ,
    AST_NEQ,
    AST_LT,
    AST_GT,
    AST_LE,
    AST_GE,
    AST_AND,
    AST_OR,
    AST_STRING_CONCAT // new for string concatenation
} ASTType;

// Convert number to string
char *toString(double val);
// Convert string to number
double toNumber(const char *str);

typedef struct ASTNode {
    ASTType type;
    char *name;
    double fval;
    char *sval;
    struct ASTNode *left, *right;
    struct ASTNode *cond, *then_branch, *else_branch;
    struct ASTNode *next;
} ASTNode;

ASTNode *make_string_concat_node(ASTNode *left, ASTNode *right);
ASTNode *make_number_node(double val);
ASTNode *make_string_node(char *val);
ASTNode *make_var_node(char *name);
ASTNode *make_declaration_node(char *name, int vartype);
ASTNode *make_declaration_assign_node(char *name, ASTNode *expr, int vartype);
ASTNode *make_assign_node(char *name, ASTNode *expr); // new
ASTNode *make_fetch_node(char *name);
ASTNode *make_display_node(char *name);
ASTNode *make_display_expression_node(ASTNode *expr);
ASTNode *make_print_node(char *name);
ASTNode *make_repeat_node(ASTNode *count_expr, ASTNode *body);
ASTNode *make_when_node(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch);
ASTNode *make_break_node();
ASTNode *make_tostring_node(ASTNode *expr);
ASTNode *make_tonumber_node(ASTNode *expr);
ASTNode *make_binary_node(ASTType type, ASTNode *left, ASTNode *right);
ASTNode *append_statements(ASTNode *list, ASTNode *stmt);

void free_ast(ASTNode *node);

#endif
