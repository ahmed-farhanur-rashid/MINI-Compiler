#include <stdio.h>
#include "ast.h"
#include "symbol_table.h"
#include "codegen.h"

extern int yyparse();
extern FILE *yyin;
extern ASTNode *root;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source.mini>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open source file");
        return 1;
    }

    init_symbol_table();

    yyin = file;
    int parse_result = yyparse();
    fclose(file);

    if (parse_result != 0) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    if (root) {
        generate_code(root);
        free_ast(root);
    }

    free_symbol_table();
    return 0;
}
