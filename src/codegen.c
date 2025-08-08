#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

// Helper function to escape string for C output
static void print_escaped_string(FILE *out, const char *str) {
    while (*str) {
        switch (*str) {
            case '\n': fprintf(out, "\\n"); break;
            case '\t': fprintf(out, "\\t"); break;
            case '\r': fprintf(out, "\\r"); break;
            case '\\': fprintf(out, "\\\\"); break;
            case '"': fprintf(out, "\\\""); break;
            default: fputc(*str, out); break;
        }
        str++;
    }
}

static void generate_node(ASTNode *node, FILE *out, int indent);
static void generate_string_length_calc(ASTNode *node, FILE *out);
static void generate_string_concat(ASTNode *node, FILE *out, const char *temp_var, int indent);
static void generate_concat_expr(ASTNode *node, FILE *out, const char *temp_var, int indent);
static void indent_spaces(FILE *out, int indent)
{
    for (int i = 0; i < indent; i++)
        fputc(' ', out);
}

void generate_code(ASTNode *root, const char *input_filename)
{
    // Get base name without extension
    char base_name[256];
    const char *last_slash = strrchr(input_filename, '/');
    const char *last_backslash = strrchr(input_filename, '\\');
    const char *filename = last_slash ? last_slash + 1 : (last_backslash ? last_backslash + 1 : input_filename);
    strncpy(base_name, filename, sizeof(base_name));
    base_name[sizeof(base_name)-1] = '\0';
    char *dot = strrchr(base_name, '.');
    if (dot) *dot = '\0';

    char out_c[300];
    char out_exe[300];
    snprintf(out_c, sizeof(out_c), "%s.c", base_name);
    snprintf(out_exe, sizeof(out_exe), "%s.exe", base_name);

    FILE *out = fopen(out_c, "w");
    if (!out)
    {
        perror("Failed to open output .c file");
        exit(1);
    }

    fprintf(out, "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n\n");
    fprintf(out, "// Auto-cleanup system\n");
    fprintf(out, "static const void **__cleanup_ptrs = NULL;\n");
    fprintf(out, "static int __cleanup_count = 0;\n");
    fprintf(out, "static int __cleanup_capacity = 0;\n");
    fprintf(out, "\n");
    fprintf(out, "void __register_ptr(const void *ptr) {\n");
    fprintf(out, "    if (__cleanup_count >= __cleanup_capacity) {\n");
    fprintf(out, "        __cleanup_capacity = __cleanup_capacity ? __cleanup_capacity * 2 : 16;\n");
    fprintf(out, "        __cleanup_ptrs = realloc(__cleanup_ptrs, __cleanup_capacity * sizeof(const void*));\n");
    fprintf(out, "    }\n");
    fprintf(out, "    __cleanup_ptrs[__cleanup_count++] = ptr;\n");
    fprintf(out, "}\n");
    fprintf(out, "\n");
    fprintf(out, "void __cleanup_all() {\n");
    fprintf(out, "    for (int i = 0; i < __cleanup_count; i++) {\n");
    fprintf(out, "        free((void*)__cleanup_ptrs[i]);\n");
    fprintf(out, "    }\n");
    fprintf(out, "    free(__cleanup_ptrs);\n");
    fprintf(out, "}\n");
    fprintf(out, "\n");
    fprintf(out, "int main() {\n");
    generate_node(root, out, 4);
    fprintf(out, "    __cleanup_all();\n");
    fprintf(out, "    return 0;\n}\n");

    fclose(out);

    char cmd[600];
    snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\"", out_c, out_exe);
    system(cmd);
    
    // Extract just the filename from the input path for display
    char display_name[256];
    strncpy(display_name, filename, sizeof(display_name));
    display_name[sizeof(display_name)-1] = '\0';
    printf("Compiled to %s.exe\n", base_name);
}

static void generate_concat_expr(ASTNode *node, FILE *out, const char *temp_var, int indent)
{
    if (!node) return;
    
    fprintf(out, "{\n");
    indent_spaces(out, indent + 4);
    fprintf(out, "size_t old_len = strlen(%s);\n", temp_var);
    indent_spaces(out, indent + 4);
    fprintf(out, "char* expr_val = ");
    
    if (node->type == AST_STRING) {
        fprintf(out, "strdup(\"");
        print_escaped_string(out, node->sval);
        fprintf(out, "\")");
    } else if (node->type == AST_VAR) {
        fprintf(out, "strdup(%s)", node->name);
    } else if (node->type == AST_TOSTRING) {
        fprintf(out, "({\n");
        indent_spaces(out, indent + 8);
        fprintf(out, "char* __tostr = malloc(64);\n");
        indent_spaces(out, indent + 8);
        fprintf(out, "sprintf(__tostr, \"%%.15g\", (double)(");
        generate_node(node->left, out, 0);
        fprintf(out, "));\n");
        indent_spaces(out, indent + 8);
        fprintf(out, "__tostr;\n");
        indent_spaces(out, indent + 4);
        fprintf(out, "})");
    } else {
        // For other expressions, try to convert to string
        fprintf(out, "({\n");
        indent_spaces(out, indent + 8);
        fprintf(out, "char* __exprstr = malloc(64);\n");
        indent_spaces(out, indent + 8);
        fprintf(out, "sprintf(__exprstr, \"%%.15g\", (double)(");
        generate_node(node, out, 0);
        fprintf(out, "));\n");
        indent_spaces(out, indent + 8);
        fprintf(out, "__exprstr;\n");
        indent_spaces(out, indent + 4);
        fprintf(out, "})");
    }
    
    fprintf(out, ";\n");
    indent_spaces(out, indent + 4);
    fprintf(out, "size_t expr_len = strlen(expr_val);\n");
    indent_spaces(out, indent + 4);
    fprintf(out, "%s = realloc(%s, old_len + expr_len + 1);\n", temp_var, temp_var);
    indent_spaces(out, indent + 4);
    fprintf(out, "strcat(%s, expr_val);\n", temp_var);
    indent_spaces(out, indent + 4);
    fprintf(out, "free(expr_val);\n");
    indent_spaces(out, indent);
    fprintf(out, "}\n");
    indent_spaces(out, indent);
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
            fprintf(out, "%.17g", node->fval);
            break;

        case AST_STRING:
            fprintf(out, "\"");
            print_escaped_string(out, node->sval);
            fprintf(out, "\"");
            break;

        case AST_VAR:
            fprintf(out, "%s", node->name);
            break;

        case AST_DECL:
        {
            const char *ctype = node->fval == TYPE_NUMBER ? "double" : "const char*";
            fprintf(out, "%s %s;\n", ctype, node->name);
            break;
        }

        case AST_ASSIGN:
        {
            Symbol *sym = lookup_symbol(node->name);
            if (!sym)
            {
                fprintf(stderr, "Internal error: unknown var %s\n", node->name);
                exit(1);
            }
            if (sym->type == TYPE_STRING) {
                // Handle different string assignment types
                if (node->left && (node->left->type == AST_STRING_CONCAT || node->left->type == AST_ADD)) {
                    // Use the expression generator for concatenation
                    fprintf(out, "%s = ", node->name);
                    generate_node(node->left, out, 0);
                    fprintf(out, "; __register_ptr(%s);\n", node->name);
                } else if (node->left && node->left->type == AST_STRING) {
                    fprintf(out, "%s = strdup(\"", node->name);
                    print_escaped_string(out, node->left->sval);
                    fprintf(out, "\"); __register_ptr(%s);\n", node->name);
                } else if (node->left && node->left->type == AST_VAR) {
                    fprintf(out, "%s = %s;\n", node->name, node->left->name);
                } else if (node->left && node->left->type == AST_TOSTRING) {
                    fprintf(out, "%s = ", node->name);
                    generate_node(node->left, out, 0);
                    fprintf(out, "; __register_ptr(%s);\n", node->name);
                } else {
                    // Default: try to generate the expression
                    fprintf(out, "%s = ", node->name);
                    generate_node(node->left, out, 0);
                    fprintf(out, "; __register_ptr(%s);\n", node->name);
                }
            } else {
                // Handle number assignments
                fprintf(out, "%s = ", node->name);
                if (node->left) {
                    generate_node(node->left, out, 0);
                } else {
                    fprintf(out, "0.0");
                }
                fprintf(out, ";\n");
            }
            break;
        }

        case AST_DECL_ASSIGN:
        {
            const char *ctype = node->fval == TYPE_NUMBER ? "double" : "const char*";

            // Handle string concatenation properly for any ADD expression
            if (node->fval == TYPE_STRING && node->left->type == AST_ADD) {
                // Create unique temp var name
                char temp[32];
                static int temp_id_decl = 3000;
                snprintf(temp, sizeof(temp), "__strtmp%d", temp_id_decl++);

                // Generate concatenation code
                fprintf(out, "char* %s = malloc(", temp);
                generate_string_length_calc(node->left, out);
                fprintf(out, " + 1); __register_ptr(%s);\n", temp);
                
                indent_spaces(out, indent);
                fprintf(out, "%s[0] = '\\0';\n", temp);
                
                generate_string_concat(node->left, out, temp, indent);
                
                indent_spaces(out, indent);
                fprintf(out, "%s %s = %s;\n", ctype, node->name, temp);
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
                fprintf(out, "{ double __temp; scanf(\"%%lf\", &__temp); %s = __temp; }\n", node->name);
                fprintf(out, "int __c; while ((__c = getchar()) != '\\n' && __c != EOF) { }\n"); // <--- FIX
            }
            else
            {
                fprintf(out, "char __buffer[256];\n");
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
                fprintf(out, "{ char __buf[64]; sprintf(__buf, \"%%.15g\", %s); ", node->name);
                fprintf(out, "char* __p = __buf + strlen(__buf) - 1; ");
                fprintf(out, "while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\\0'; ");
                fprintf(out, "if(__p > __buf && *__p == '.') *__p = '\\0'; ");
                fprintf(out, "printf(\"%%s\", __buf); }\n");
            }
            else
            {
                fprintf(out, "printf(\"%%s\", %s);\n", node->name);
            }
            break;
        }

        case AST_DISPLAY_EXPR:
        {
            fprintf(out, "{ ");
            if (node->left->type == AST_STRING) {
                fprintf(out, "printf(\"%%s\", \"");
                print_escaped_string(out, node->left->sval);
                fprintf(out, "\"); ");
            } else if (node->left->type == AST_VAR) {
                Symbol *sym = lookup_symbol(node->left->name);
                if (sym && sym->type == TYPE_NUMBER) {
                    fprintf(out, "char __buf[64]; sprintf(__buf, \"%%.15g\", %s); ", node->left->name);
                    fprintf(out, "char* __p = __buf + strlen(__buf) - 1; ");
                    fprintf(out, "while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\\0'; ");
                    fprintf(out, "if(__p > __buf && *__p == '.') *__p = '\\0'; ");
                    fprintf(out, "printf(\"%%s\", __buf); ");
                } else {
                    fprintf(out, "printf(\"%%s\", %s); ", node->left->name);
                }
            } else if (node->left->type == AST_ADD) {
                // String concatenation result
                fprintf(out, "char* __temp = ");
                generate_node(node->left, out, 0);
                fprintf(out, "; printf(\"%%s\", __temp); ");
            } else if (node->left->type == AST_STRING_CONCAT) {
                // String concatenation result
                fprintf(out, "char* __temp = ");
                generate_node(node->left, out, 0);
                fprintf(out, "; printf(\"%%s\", __temp); ");
            } else if (node->left->type == AST_TOSTRING) {
                // toString function result
                fprintf(out, "char* __temp = ");
                generate_node(node->left, out, 0);
                fprintf(out, "; printf(\"%%s\", __temp); ");
            } else {
                // Check if it might be a string expression
                if (node->left->type == AST_ADD || 
                    node->left->type == AST_STRING_CONCAT ||
                    (node->left->type == AST_VAR && lookup_symbol(node->left->name) && lookup_symbol(node->left->name)->type == TYPE_STRING)) {
                    // String expression
                    fprintf(out, "char* __temp = ");
                    generate_node(node->left, out, 0);
                    fprintf(out, "; printf(\"%%s\", __temp); ");
                } else {
                    // Numeric expression
                    fprintf(out, "printf(\"%%g\", (double)(");
                    generate_node(node->left, out, 0);
                    fprintf(out, ")); ");
                }
            }
            fprintf(out, "}\n");
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
            // Check if this is string concatenation by examining operands
            int is_string_concat = 0;
            
            // Check left operand
            if (node->left->type == AST_VAR) {
                Symbol *left = lookup_symbol(node->left->name);
                if (left && left->type == TYPE_STRING) is_string_concat = 1;
            } else if (node->left->type == AST_STRING) {
                is_string_concat = 1;
            } else if (node->left->type == AST_ADD) {
                // Recursive string concatenation - assume string if left is ADD
                is_string_concat = 1;
            }
            
            // Check right operand  
            if (node->right->type == AST_VAR) {
                Symbol *right = lookup_symbol(node->right->name);
                if (right && right->type == TYPE_STRING) is_string_concat = 1;
            } else if (node->right->type == AST_STRING) {
                is_string_concat = 1;
            }
            
            if (is_string_concat) {
                char temp[32];
                snprintf(temp, sizeof(temp), "__strtmp%d", temp_id++);
                
                // Handle string concatenation
                fprintf(out, "({\n");
                indent_spaces(out, indent + 4);
                fprintf(out, "char* %s = malloc(", temp);
                
                // Calculate total length needed
                generate_string_length_calc(node, out);
                fprintf(out, " + 1);\n");
                
                indent_spaces(out, indent + 4);
                fprintf(out, "%s[0] = '\\0';\n", temp);
                
                // Generate concatenation calls
                generate_string_concat(node, out, temp, indent + 4);
                
                indent_spaces(out, indent + 4);
                fprintf(out, "%s;\n", temp);
                indent_spaces(out, indent);
                fprintf(out, "})");
            } else {
                // Numeric addition
                fprintf(out, "(");
                generate_node(node->left, out, 0);
                fprintf(out, " + ");
                generate_node(node->right, out, 0);
                fprintf(out, ")");
            }
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
        {
            // Check if we're comparing strings
            int left_is_string = (node->left->type == AST_VAR && 
                                 lookup_symbol(node->left->name) && 
                                 lookup_symbol(node->left->name)->type == TYPE_STRING) ||
                                (node->left->type == AST_STRING);
            int right_is_string = (node->right->type == AST_VAR && 
                                  lookup_symbol(node->right->name) && 
                                  lookup_symbol(node->right->name)->type == TYPE_STRING) ||
                                 (node->right->type == AST_STRING);
            
            if (left_is_string || right_is_string) {
                fprintf(out, "(strcmp(");
                generate_node(node->left, out, 0);
                fprintf(out, ", ");
                generate_node(node->right, out, 0);
                fprintf(out, ") == 0)");
            } else {
                fprintf(out, "(");
                generate_node(node->left, out, 0);
                fprintf(out, " == ");
                generate_node(node->right, out, 0);
                fprintf(out, ")");
            }
            break;
        }

        case AST_NEQ:
        {
            // Check if we're comparing strings
            int left_is_string = (node->left->type == AST_VAR && 
                                 lookup_symbol(node->left->name) && 
                                 lookup_symbol(node->left->name)->type == TYPE_STRING) ||
                                (node->left->type == AST_STRING);
            int right_is_string = (node->right->type == AST_VAR && 
                                  lookup_symbol(node->right->name) && 
                                  lookup_symbol(node->right->name)->type == TYPE_STRING) ||
                                 (node->right->type == AST_STRING);
            
            if (left_is_string || right_is_string) {
                fprintf(out, "(strcmp(");
                generate_node(node->left, out, 0);
                fprintf(out, ", ");
                generate_node(node->right, out, 0);
                fprintf(out, ") != 0)");
            } else {
                fprintf(out, "(");
                generate_node(node->left, out, 0);
                fprintf(out, " != ");
                generate_node(node->right, out, 0);
                fprintf(out, ")");
            }
            break;
        }

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

        case AST_STRING_CONCAT:
        {
            // Generate a simple compound expression that returns a string
            char temp[32];
            snprintf(temp, sizeof(temp), "__strtmp%d", temp_id++);
            
            fprintf(out, "({\n");
            indent_spaces(out, indent + 4);
            fprintf(out, "char* %s = malloc(1);\n", temp);
            indent_spaces(out, indent + 4);
            fprintf(out, "%s[0] = '\\0';\n", temp);
            
            // Concatenate left side
            indent_spaces(out, indent + 4);
            if (node->left->type == AST_STRING) {
                fprintf(out, "%s = realloc(%s, %d + 1);\n", temp, temp, (int)strlen(node->left->sval));
                indent_spaces(out, indent + 4);
                fprintf(out, "strcat(%s, \"", temp);
                print_escaped_string(out, node->left->sval);
                fprintf(out, "\");\n");
            } else if (node->left->type == AST_VAR) {
                fprintf(out, "%s = realloc(%s, strlen(%s) + 1);\n", temp, temp, node->left->name);
                indent_spaces(out, indent + 4);
                fprintf(out, "strcat(%s, %s);\n", temp, node->left->name);
            } else if (node->left->type == AST_TOSTRING) {
                fprintf(out, "char* __left_str = ");
                generate_node(node->left, out, 0);
                fprintf(out, ";\n");
                indent_spaces(out, indent + 4);
                fprintf(out, "%s = realloc(%s, strlen(__left_str) + 1);\n", temp, temp);
                indent_spaces(out, indent + 4);
                fprintf(out, "strcat(%s, __left_str);\n", temp);
                indent_spaces(out, indent + 4);
                fprintf(out, "free(__left_str);\n");
            } else {
                // Recursively handle other concatenations
                fprintf(out, "char* __left_str = ");
                generate_node(node->left, out, 0);
                fprintf(out, ";\n");
                indent_spaces(out, indent + 4);
                fprintf(out, "%s = realloc(%s, strlen(__left_str) + 1);\n", temp, temp);
                indent_spaces(out, indent + 4);
                fprintf(out, "strcat(%s, __left_str);\n", temp);
            }
            
            // Concatenate right side
            indent_spaces(out, indent + 4);
            if (node->right->type == AST_STRING) {
                fprintf(out, "%s = realloc(%s, strlen(%s) + %d + 1);\n", temp, temp, temp, (int)strlen(node->right->sval));
                indent_spaces(out, indent + 4);
                fprintf(out, "strcat(%s, \"", temp);
                print_escaped_string(out, node->right->sval);
                fprintf(out, "\");\n");
            } else if (node->right->type == AST_VAR) {
                fprintf(out, "%s = realloc(%s, strlen(%s) + strlen(%s) + 1);\n", temp, temp, temp, node->right->name);
                indent_spaces(out, indent + 4);
                fprintf(out, "strcat(%s, %s);\n", temp, node->right->name);
            } else if (node->right->type == AST_TOSTRING) {
                fprintf(out, "char* __right_str = ");
                generate_node(node->right, out, 0);
                fprintf(out, ";\n");
                indent_spaces(out, indent + 4);
                fprintf(out, "%s = realloc(%s, strlen(%s) + strlen(__right_str) + 1);\n", temp, temp, temp);
                indent_spaces(out, indent + 4);
                fprintf(out, "strcat(%s, __right_str);\n", temp);
                indent_spaces(out, indent + 4);
                fprintf(out, "free(__right_str);\n");
            } else {
                // Recursively handle other concatenations
                fprintf(out, "char* __right_str = ");
                generate_node(node->right, out, 0);
                fprintf(out, ";\n");
                indent_spaces(out, indent + 4);
                fprintf(out, "%s = realloc(%s, strlen(%s) + strlen(__right_str) + 1);\n", temp, temp, temp);
                indent_spaces(out, indent + 4);
                fprintf(out, "strcat(%s, __right_str);\n", temp);
            }
            
            indent_spaces(out, indent + 4);
            fprintf(out, "__register_ptr(%s);\n", temp);
            indent_spaces(out, indent + 4);
            fprintf(out, "%s;\n", temp);
            indent_spaces(out, indent);
            fprintf(out, "})");
            break;
        }

        case AST_TOSTRING:
        {
            char temp[32];
            snprintf(temp, sizeof(temp), "__strtmp%d", temp_id++);
            fprintf(out, "({\n");
            indent_spaces(out, indent + 4);
            fprintf(out, "char* %s = malloc(64);\n", temp);
            indent_spaces(out, indent + 4);
            fprintf(out, "sprintf(%s, \"%%.15g\", (double)(", temp);
            generate_node(node->left, out, 0);
            fprintf(out, "));\n");
            indent_spaces(out, indent + 4);
            fprintf(out, "%s;\n", temp);
            indent_spaces(out, indent);
            fprintf(out, "})");
            break;
        }

        case AST_TONUMBER:
        {
            fprintf(out, "atof(");
            generate_node(node->left, out, 0);
            fprintf(out, ")");
            break;
        }

        default:
            fprintf(stderr, "Codegen: unhandled AST type %d\n", node->type);
            exit(1);
        }

        node = node->next;
    }
}

// Helper function to calculate total string length for concatenation
static void generate_string_length_calc(ASTNode *node, FILE *out) {
    if (node->type == AST_STRING) {
        fprintf(out, "%d", (int)strlen(node->sval));
    } else if (node->type == AST_VAR) {
        fprintf(out, "strlen(%s)", node->name);
    } else if (node->type == AST_TOSTRING) {
        fprintf(out, "64"); // Conservative estimate for number-to-string conversion
    } else if (node->type == AST_ADD) {
        generate_string_length_calc(node->left, out);
        fprintf(out, " + ");
        generate_string_length_calc(node->right, out);
    }
}

// Helper function to generate string concatenation
static void generate_string_concat(ASTNode *node, FILE *out, const char *temp_var, int indent) {
    static int temp_id = 0;
    
    if (node->type == AST_STRING) {
        indent_spaces(out, indent);
        fprintf(out, "strcat(%s, \"", temp_var);
        print_escaped_string(out, node->sval);
        fprintf(out, "\");\n");
    } else if (node->type == AST_VAR) {
        indent_spaces(out, indent);
        fprintf(out, "strcat(%s, %s);\n", temp_var, node->name);
    } else if (node->type == AST_TOSTRING) {
        indent_spaces(out, indent);
        char inner_temp[32];
        snprintf(inner_temp, sizeof(inner_temp), "__numstr%d", temp_id++);
        fprintf(out, "{ char %s[64]; sprintf(%s, \"%%.15g\", (double)(", inner_temp, inner_temp);
        generate_node(node->left, out, 0);
        fprintf(out, ")); strcat(%s, %s); }\n", temp_var, inner_temp);
    } else if (node->type == AST_ADD) {
        generate_string_concat(node->left, out, temp_var, indent);
        generate_string_concat(node->right, out, temp_var, indent);
    }
}
