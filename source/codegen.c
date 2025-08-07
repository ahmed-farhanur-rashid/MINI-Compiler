#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

static void generate_node(ASTNode *node, FILE *out, int indent);
static void indent_spaces(FILE *out, int indent)
{
    for (int i = 0; i < indent; i++)
        fputc(' ', out);
}

void generate_code(ASTNode *root)
{
    FILE *out = fopen("out.c", "w");
    if (!out)
    {
        perror("Failed to open out.c");
        exit(1);
    }

    fprintf(out, "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n\nint main() {\n");
    generate_node(root, out, 4);
    fprintf(out, "    return 0;\n}\n");

    fclose(out);
    system("gcc out.c -o out.exe");
    printf("Compiled to out.exe\n");
}

static void generate_node(ASTNode *node, FILE *out, int indent)
{
    static int temp_id = 0;

    while (node)
    {
        indent_spaces(out, indent);

        switch (node->type)
        {
        case AST_NUMBER:
            fprintf(out, "%f", node->fval);
            break;

        case AST_STRING:
            fprintf(out, "\"%s\"", node->sval);
            break;

        case AST_VAR:
            fprintf(out, "%s", node->name);
            break;

        case AST_DECL:
        {
            const char *ctype = node->fval == TYPE_NUMBER ? "float" : "const char*";
            fprintf(out, "%s %s;\n", ctype, node->name);
            break;
        }

        case AST_DECL_ASSIGN:
        {
            const char *ctype = node->fval == TYPE_NUMBER ? "float" : "const char*";

            // Detect string concat expression on RHS: (string var + string var)
            if (node->fval == TYPE_STRING &&
                node->left->type == AST_ADD &&
                node->left->left->type == AST_VAR &&
                node->left->right->type == AST_VAR)
            {

                // Create unique temp var name
                char temp[32];
                snprintf(temp, sizeof(temp), "__strtmp%d", temp_id++);

                // Generate concatenation code with proper indentation
                fprintf(out, "char* %s = malloc(strlen(%s) + strlen(%s) + 1);\n",
                        temp, node->left->left->name, node->left->right->name);
                indent_spaces(out, indent);
                fprintf(out, "strcpy(%s, %s);\n", temp, node->left->left->name);
                indent_spaces(out, indent);
                fprintf(out, "strcat(%s, %s);\n", temp, node->left->right->name);
                indent_spaces(out, indent);
                fprintf(out, "const char* %s = %s;\n", node->name, temp);
            }
            else
            {
                fprintf(out, "%s %s = ", ctype, node->name);
                generate_node(node->left, out, 0);
                fprintf(out, ";\n");
            }
            break;
        }

        case AST_FETCH:
        {
            Symbol *sym = lookup_symbol(node->name);
            if (!sym)
            {
                fprintf(stderr, "Internal error: unknown var %s\n", node->name);
                exit(1);
            }
            if (sym->type == TYPE_NUMBER)
            {
                fprintf(out, "printf(\"Enter number for %s: \");\n", node->name);
                fprintf(out, "scanf(\"%%f\", &%s);\n", node->name);
                fprintf(out, "int __c; while ((__c = getchar()) != '\\n' && __c != EOF) { }\n"); // <--- FIX
            }
            else
            {
                fprintf(out, "char __buffer[256];\n");
                fprintf(out, "printf(\"Enter string for %s: \");\n", node->name);
                fprintf(out, "fgets(__buffer, 256, stdin);\n");
                fprintf(out, "__buffer[strcspn(__buffer, \"\\n\")] = 0;\n");
                fprintf(out, "%s = strdup(__buffer);\n", node->name);
            }
            break;
        }

        case AST_DISPLAY:
        {
            Symbol *sym = lookup_symbol(node->name);
            if (!sym)
            {
                fprintf(stderr, "Internal error: unknown var %s\n", node->name);
                exit(1);
            }
            if (sym->type == TYPE_NUMBER)
            {
                fprintf(out, "printf(\"%%f\\n\", %s);\n", node->name);
            }
            else
            {
                fprintf(out, "printf(\"%%s\\n\", %s);\n", node->name);
            }
            break;
        }

        case AST_REPEAT:
            fprintf(out, "for (int __i = 0; __i < ");
            generate_node(node->left, out, 0);
            fprintf(out, "; __i++) {\n");
            generate_node(node->then_branch, out, indent + 4);
            indent_spaces(out, indent);
            fprintf(out, "}\n");
            break;

        case AST_WHEN:
            fprintf(out, "if (");
            generate_node(node->cond, out, 0);
            fprintf(out, ") {\n");
            generate_node(node->then_branch, out, indent + 4);
            indent_spaces(out, indent);
            fprintf(out, "}");
            if (node->else_branch)
            {
                fprintf(out, " else {\n");
                generate_node(node->else_branch, out, indent + 4);
                indent_spaces(out, indent);
                fprintf(out, "}");
            }
            fprintf(out, "\n");
            break;

        case AST_BREAK:
            fprintf(out, "break;\n");
            break;

        case AST_ADD:
        {
            // Detect if both operands are string variables
            if (node->left->type == AST_VAR && node->right->type == AST_VAR)
            {
                Symbol *left = lookup_symbol(node->left->name);
                Symbol *right = lookup_symbol(node->right->name);
                if (left && right && left->type == TYPE_STRING && right->type == TYPE_STRING)
                {
                    char temp[32];
                    snprintf(temp, sizeof(temp), "__strtmp%d", temp_id++);

                    // Generate concatenation
                    fprintf(out, "char* %s = malloc(strlen(%s) + strlen(%s) + 1);\n",
                            temp, node->left->name, node->right->name);
                    indent_spaces(out, indent);
                    fprintf(out, "strcpy(%s, %s);\n", temp, node->left->name);
                    indent_spaces(out, indent);
                    fprintf(out, "strcat(%s, %s);\n", temp, node->right->name);
                    indent_spaces(out, indent);
                    fprintf(out, "%s", temp);
                    break;
                }
            }

            // Fallback for numeric +
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " + ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;
        }

        case AST_SUB:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " - ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_MUL:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " * ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_DIV:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " / ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_EQ:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " == ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_NEQ:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " != ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_LT:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " < ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_GT:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " > ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_LE:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " <= ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_GE:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " >= ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_AND:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " && ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        case AST_OR:
            fprintf(out, "(");
            generate_node(node->left, out, 0);
            fprintf(out, " || ");
            generate_node(node->right, out, 0);
            fprintf(out, ")");
            break;

        default:
            fprintf(stderr, "Codegen: unhandled AST type %d\n", node->type);
            exit(1);
        }

        node = node->next;
    }
}
