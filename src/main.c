#include <stdio.h>
#include "ast.h"
#include "symbol_table.h"
#include "codegen.h"

extern int yyparse();
extern FILE *yyin, *yyout;
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

    printf("Compiling %s...", argv[1]);
    
    yyin = file;
    yyout = fopen("NUL", "w");  // Redirect lexer output to null device
    int parse_result = yyparse();
    if (yyout) fclose(yyout);
    fclose(file);

    if (parse_result != 0) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    if (root) {
        generate_code(root, argv[1]);
        free_ast(root);
    }

    free_symbol_table();
    return 0;
}
