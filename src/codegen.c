#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ast.h"
#include "symbol_table.h"

/* ===================== Utilities for emitting code ===================== */

static void indent_spaces(FILE *out, int indent) {
    for (int i = 0; i < indent; i++) fputc(' ', out);
}

static void print_escaped_string(FILE *out, const char *s) {
    for (; *s; s++) {
        switch (*s) {
            case '\n': fputs("\\n", out); break;
            case '\t': fputs("\\t", out); break;
            case '\r': fputs("\\r", out); break;
            case '\\': fputs("\\\\", out); break;
            case '\"': fputs("\\\"", out); break;
            default:   fputc(*s, out); break;
        }
    }
}

/* Forward decls for code emission */
static void generate_node(ASTNode *node, FILE *out, int indent);
static void generate_string_length_calc(ASTNode *node, FILE *out);
static void generate_string_concat(ASTNode *node, FILE *out, const char *dst_var, int indent);

/* ===================== Top-level: generate_code ===================== */

void generate_code(ASTNode *root, const char *input_filename)
{
    /* derive base name (no path, no extension) */
    char base[256];
    const char *slash1 = strrchr(input_filename, '/');
    const char *slash2 = strrchr(input_filename, '\\');
    const char *fname = slash1 ? slash1 + 1 : (slash2 ? slash2 + 1 : input_filename);

    strncpy(base, fname, sizeof(base));
    base[sizeof(base)-1] = '\0';
    char *dot = strrchr(base, '.');
    if (dot) *dot = '\0';

    char out_c[300], out_exe[300];
    snprintf(out_c, sizeof(out_c), "%s.c", base);
    snprintf(out_exe, sizeof(out_exe), "%s.exe", base);

    FILE *out = fopen(out_c, "w");
    if (!out) { perror("open .c"); exit(1); }

    /* ---- Emit C prelude/runtime that the generated program will use ---- */
    fputs(
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "#include <ctype.h>\n"
        "\n"
        "/* ------- Auto-cleanup registry ------- */\n"
        "static void **__cleanup_ptrs = NULL;\n"
        "static int __cleanup_count = 0;\n"
        "static int __cleanup_cap = 0;\n"
        "static void __register_ptr(void *p){\n"
        "    if(!p) return;\n"
        "    if(__cleanup_count >= __cleanup_cap){\n"
        "        __cleanup_cap = __cleanup_cap ? __cleanup_cap*2 : 16;\n"
        "        __cleanup_ptrs = (void**)realloc(__cleanup_ptrs, (size_t)__cleanup_cap * sizeof(void*));\n"
        "    }\n"
        "    __cleanup_ptrs[__cleanup_count++] = p;\n"
        "}\n"
        "static void __cleanup_all(void){\n"
        "    for(int i=0;i<__cleanup_count;i++) free(__cleanup_ptrs[i]);\n"
        "    free(__cleanup_ptrs);\n"
        "}\n"
        "\n"
        "/* ------- Number formatting (double) ------- */\n"
        "static void __trim_trailing_zeros(char *s){\n"
        "    if(!s) return;\n"
        "    char *dot = strchr(s, '.');\n"
        "    if(!dot) return;\n"
        "    char *p = s + strlen(s) - 1;\n"
        "    while(p > dot && *p == '0'){ *p-- = '\\0'; }\n"
        "    if(p == dot) *p = '\\0';\n"
        "}\n"
        "static char* __num_to_string(double v){\n"
        "    char *buf = (char*)malloc(72);\n"
        "    if(!buf) return NULL;\n"
        "    /* 15 significant digits → like your original toString */\n"
        "    snprintf(buf, 72, \"%.15g\", v);\n"
        "    __trim_trailing_zeros(buf);\n"
        "    return buf;\n"
        "}\n"
        "\n"
        "/* ------- String helpers that always return newly-allocated memory ------- */\n"
        "static char* __dup_cstr(const char *s){\n"
        "    if(!s) return NULL;\n"
        "    size_t n = strlen(s) + 1;\n"
        "    char *p = (char*)malloc(n);\n"
        "    if(!p) return NULL;\n"
        "    memcpy(p, s, n);\n"
        "    return p;\n"
        "}\n"
        "static char* __concat2(const char *a, const char *b){\n"
        "    size_t na = a ? strlen(a) : 0;\n"
        "    size_t nb = b ? strlen(b) : 0;\n"
        "    char *p = (char*)malloc(na + nb + 1);\n"
        "    if(!p) return NULL;\n"
        "    if(a) memcpy(p, a, na); if(b) memcpy(p+na, b, nb);\n"
        "    p[na+nb] = '\\0';\n"
        "    return p;\n"
        "}\n"
        "\n", out
    );

    /* ---- main() open ---- */
    fputs("int main(void){\n", out);

    /* emit the program body */
    generate_node(root, out, 4);

    /* cleanup and exit */
    fputs("    __cleanup_all();\n", out);
    fputs("    return 0;\n}\n", out);

    fclose(out);

    /* build */
    char cmd[600];
    snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\"", out_c, out_exe);
    (void)system(cmd);

    printf("Compiled to %s.exe\n", base);
}

/* ===================== Core code emission ===================== */

static void generate_node(ASTNode *node, FILE *out, int indent)
{
    static int temp_id = 0;

    while (node) {
        indent_spaces(out, indent);

        switch (node->type) {
        case AST_NUMBER:
            /* print as a literal double */
            fprintf(out, "(%.15g)", (double)node->fval);
            break;

        case AST_STRING:
            fputc('\"', out);
            print_escaped_string(out, node->sval);
            fputc('\"', out);
            break;

        case AST_VAR:
            fprintf(out, "%s", node->name);
            break;

        case AST_DECL: {
            const char *ctype = (node->fval == TYPE_NUMBER) ? "double" : "char*";
            fprintf(out, "%s %s;\n", ctype, node->name);
            break;
        }

        case AST_ASSIGN: {
            Symbol *sym = lookup_symbol(node->name);
            if (!sym) { fprintf(stderr, "unknown var %s\n", node->name); exit(1); }

            if (sym->type == TYPE_STRING) {
                /* produce: name = <string expr>; __register_ptr(name); */
                fprintf(out, "%s = ", node->name);

                /* string-producing expression paths */
                if (node->left && node->left->type == AST_STRING) {
                    /* strdup literal */
                    fputs("(__dup_cstr(\"", out);
                    print_escaped_string(out, node->left->sval);
                    fputs("\"))", out);
                } else if (node->left && node->left->type == AST_VAR) {
                    /* duplicate the variable's string to own it */
                    fprintf(out, "__dup_cstr(%s)", node->left->name);
                } else if (node->left && node->left->type == AST_TOSTRING) {
                    /* number to string -> already allocated via helper below */
                    generate_node(node->left, out, 0);
                } else if (node->left && node->left->type == AST_ADD) {
                    /* string concat: handled by ADD case to return allocated string */
                    generate_node(node->left, out, 0);
                } else {
                    /* generic: try emit left and assume it yields char* */
                    generate_node(node->left, out, 0);
                }
                fputs(";\n", out);
                indent_spaces(out, indent);
                fprintf(out, "__register_ptr(%s);\n", node->name);
            } else {
                /* numeric assign */
                fprintf(out, "%s = ", node->name);
                if (node->left) generate_node(node->left, out, 0);
                else fputs("0.0", out);
                fputs(";\n", out);
            }
            break;
        }

        case AST_DECL_ASSIGN: {
            const char *ctype = (node->fval == TYPE_NUMBER) ? "double" : "char*";
            if (node->fval == TYPE_STRING && node->left && node->left->type == AST_ADD) {
                /* declare string and init via concat path */
                char tmp[32]; static int tid=3000; snprintf(tmp,sizeof(tmp),"__strtmp%d", tid++);
                /* compute final string using helper concat generation */
                fprintf(out, "%s %s = NULL;\n", ctype, node->name);
                indent_spaces(out, indent);
                fprintf(out, "{\n");
                indent_spaces(out, indent+4);
                fprintf(out, "/* build concatenation */\n");
                indent_spaces(out, indent+4);
                fprintf(out, "size_t __need = ");
                generate_string_length_calc(node->left, out);
                fprintf(out, " + 1;\n");
                indent_spaces(out, indent+4);
                fprintf(out, "char *%s = (char*)malloc(__need);\n", tmp);
                indent_spaces(out, indent+4);
                fprintf(out, "if(%s){ %s[0]='\\0'; }\n", tmp, tmp);
                generate_string_concat(node->left, out, tmp, indent+4);
                indent_spaces(out, indent+4);
                fprintf(out, "%s = %s;\n", node->name, tmp);
                indent_spaces(out, indent+4);
                fprintf(out, "__register_ptr(%s);\n", node->name);
                indent_spaces(out, indent);
                fprintf(out, "}\n");
            } else {
                fprintf(out, "%s %s = ", ctype, node->name);
                generate_node(node->left, out, 0);
                fputs(";\n", out);
                if (node->fval != TYPE_NUMBER) {
                    indent_spaces(out, indent);
                    fprintf(out, "__register_ptr(%s);\n", node->name);
                }
            }
            break;
        }

        case AST_FETCH: {
            Symbol *sym = lookup_symbol(node->name);
            if (!sym) { fprintf(stderr, "unknown var %s\n", node->name); exit(1); }
            if (sym->type == TYPE_NUMBER) {
                /* read double; then flush rest of line */
                fprintf(out, "{ double __t=0.0; if(scanf(\"%%lf\", &__t)==1) %s = __t; int __c; while((__c=getchar())!='\\n' && __c!=EOF){} }\n", node->name);
            } else {
                /* read a line safely and duplicate */
                fprintf(out, "{ char __buf[1024]; if(fgets(__buf, sizeof(__buf), stdin)){ __buf[strcspn(__buf, \"\\n\")] = 0; char* __tmp = __dup_cstr(__buf); %s = __tmp; __register_ptr(%s);} }\n", node->name, node->name);
            }
            break;
        }

        case AST_DISPLAY: {
            Symbol *sym = lookup_symbol(node->name);
            if (!sym) { fprintf(stderr, "unknown var %s\n", node->name); exit(1); }
            if (sym->type == TYPE_NUMBER) {
                /* format once via helper, print, free */
                fprintf(out, "{ char* __s = __num_to_string(%s); if(__s){ fputs(__s, stdout); free(__s);} }\n", node->name);
            } else {
                fprintf(out, "fputs(%s ? %s : \"\", stdout);\n", node->name, node->name);
            }
            break;
        }

        case AST_DISPLAY_EXPR: {
            fputs("{ ", out);
            if (node->left->type == AST_STRING) {
                fputs("fputs(\"", out); print_escaped_string(out, node->left->sval); fputs("\", stdout); ", out);
            } else if (node->left->type == AST_VAR) {
                Symbol *sym = lookup_symbol(node->left->name);
                if (sym && sym->type == TYPE_NUMBER) {
                    fprintf(out, "char* __s = __num_to_string(%s); if(__s){ fputs(__s, stdout); free(__s);} ", node->left->name);
                } else {
                    fprintf(out, "fputs(%s ? %s : \"\", stdout); ", node->left->name, node->left->name);
                }
            } else if (node->left->type == AST_ADD || node->left->type == AST_STRING_CONCAT || node->left->type == AST_TOSTRING) {
                /* these yield a string */
                fprintf(out, "char* __t = "); generate_node(node->left, out, 0); fputs("; if(__t){ fputs(__t, stdout); } ", out);
            } else {
                /* treat as numeric */
                fputs("{ char* __s = __num_to_string((double)(", out);
                generate_node(node->left, out, 0);
                fputs(")); if(__s){ fputs(__s, stdout); free(__s);} } ", out);
            }
            fputs("}\n", out);
            break;
        }

        case AST_REPEAT:
            fputs("for (int __i = 0; __i < ", out);
            generate_node(node->left, out, 0);
            fputs("; __i++) {\n", out);
            generate_node(node->then_branch, out, indent + 4);
            indent_spaces(out, indent); fputs("}\n", out);
            break;

        case AST_WHEN:
            fputs("if (", out); generate_node(node->cond, out, 0); fputs(") {\n", out);
            generate_node(node->then_branch, out, indent + 4);
            indent_spaces(out, indent); fputs("}", out);
            if (node->else_branch) {
                fputs(" else {\n", out);
                generate_node(node->else_branch, out, indent + 4);
                indent_spaces(out, indent); fputs("}", out);
            }
            fputs("\n", out);
            break;

        case AST_BREAK:
            fputs("break;\n", out);
            break;

        case AST_ADD: {
            /* Decide: numeric add vs string concat */
            int is_string = 0;
            if (node->left->type == AST_STRING || node->right->type == AST_STRING) is_string = 1;
            if (!is_string && node->left->type == AST_VAR) {
                Symbol *L = lookup_symbol(node->left->name);
                if (L && L->type == TYPE_STRING) is_string = 1;
            }
            if (!is_string && node->right->type == AST_VAR) {
                Symbol *R = lookup_symbol(node->right->name);
                if (R && R->type == TYPE_STRING) is_string = 1;
            }
            if (!is_string && node->left->type == AST_ADD) is_string = 1; /* cascading concat */
            if (is_string) {
                /* Build with a temporary buffer (pre-sized) */
                char tmp[32]; snprintf(tmp, sizeof(tmp), "__strtmp%d", temp_id++);
                /* produce a block that builds and returns the allocated string */
                fprintf(out, "({ char* %s = NULL; size_t __need = ", tmp);
                generate_string_length_calc(node, out);
                fputs(" + 1; ", out);
                fprintf(out, "%s = (char*)malloc(__need); if(%s) %s[0]='\\0'; ", tmp, tmp, tmp);
                generate_string_concat(node, out, tmp, 0);
                fprintf(out, "__register_ptr(%s); %s; })", tmp, tmp);
            } else {
                fputc('(', out); generate_node(node->left, out, 0); fputs(" + ", out); generate_node(node->right, out, 0); fputc(')', out);
            }
            break;
        }

        case AST_SUB:
            fputc('(', out); generate_node(node->left, out, 0); fputs(" - ", out); generate_node(node->right, out, 0); fputc(')', out);
            break;
        case AST_MUL:
            fputc('(', out); generate_node(node->left, out, 0); fputs(" * ", out); generate_node(node->right, out, 0); fputc(')', out);
            break;
        case AST_DIV:
            fputc('(', out); generate_node(node->left, out, 0); fputs(" / ", out); generate_node(node->right, out, 0); fputc(')', out);
            break;

        case AST_EQ: {
            int Ls = (node->left->type == AST_STRING) ||
                     (node->left->type == AST_VAR && lookup_symbol(node->left->name) && lookup_symbol(node->left->name)->type == TYPE_STRING);
            int Rs = (node->right->type == AST_STRING) ||
                     (node->right->type == AST_VAR && lookup_symbol(node->right->name) && lookup_symbol(node->right->name)->type == TYPE_STRING);
            if (Ls || Rs) {
                fputs("(strcmp(", out); generate_node(node->left, out, 0); fputs(", ", out); generate_node(node->right, out, 0); fputs(") == 0)", out);
            } else {
                fputc('(', out); generate_node(node->left, out, 0); fputs(" == ", out); generate_node(node->right, out, 0); fputc(')', out);
            }
            break;
        }

        case AST_NEQ: {
            int Ls = (node->left->type == AST_STRING) ||
                     (node->left->type == AST_VAR && lookup_symbol(node->left->name) && lookup_symbol(node->left->name)->type == TYPE_STRING);
            int Rs = (node->right->type == AST_STRING) ||
                     (node->right->type == AST_VAR && lookup_symbol(node->right->name) && lookup_symbol(node->right->name)->type == TYPE_STRING);
            if (Ls || Rs) {
                fputs("(strcmp(", out); generate_node(node->left, out, 0); fputs(", ", out); generate_node(node->right, out, 0); fputs(") != 0)", out);
            } else {
                fputc('(', out); generate_node(node->left, out, 0); fputs(" != ", out); generate_node(node->right, out, 0); fputc(')', out);
            }
            break;
        }

        case AST_LT:
            fputc('(', out); generate_node(node->left, out, 0); fputs(" < ", out); generate_node(node->right, out, 0); fputc(')', out);
            break;
        case AST_GT:
            fputc('(', out); generate_node(node->left, out, 0); fputs(" > ", out); generate_node(node->right, out, 0); fputc(')', out);
            break;
        case AST_LE:
            fputc('(', out); generate_node(node->left, out, 0); fputs(" <= ", out); generate_node(node->right, out, 0); fputc(')', out);
            break;
        case AST_GE:
            fputc('(', out); generate_node(node->left, out, 0); fputs(" >= ", out); generate_node(node->right, out, 0); fputc(')', out);
            break;

        case AST_AND:
            fputc('(', out); generate_node(node->left, out, 0); fputs(" && ", out); generate_node(node->right, out, 0); fputc(')', out);
            break;
        case AST_OR:
            fputc('(', out); generate_node(node->left, out, 0); fputs(" || ", out); generate_node(node->right, out, 0); fputc(')', out);
            break;

        case AST_STRING_CONCAT: {
            /* legacy node: emit as explicit concat using helpers */
            char tmp[32]; snprintf(tmp, sizeof(tmp), "__strtmp%d", temp_id++);
            fprintf(out, "({ char* %s = NULL; size_t __need = 1 + ", tmp);
            /* a minimal length calc for left */
            if (node->left->type == AST_STRING) fprintf(out, "%d", (int)strlen(node->left->sval));
            else if (node->left->type == AST_VAR) fprintf(out, "strlen(%s)", node->left->name);
            else fputs("0", out);
            fputs(" + ", out);
            if (node->right->type == AST_STRING) fprintf(out, "%d", (int)strlen(node->right->sval));
            else if (node->right->type == AST_VAR) fprintf(out, "strlen(%s)", node->right->name);
            else fputs("0", out);
            fputs("; ", out);
            fprintf(out, "%s = (char*)malloc(__need); if(%s){ %s[0]='\\0'; } ", tmp, tmp, tmp);
            if (node->left->type == AST_STRING) { fprintf(out, "strcat(%s, \"", tmp); print_escaped_string(out, node->left->sval); fputs("\"); ", out); }
            else if (node->left->type == AST_VAR) fprintf(out, "if(%s) strcat(%s, %s); ", node->left->name, tmp, node->left->name);
            if (node->right->type == AST_STRING) { fprintf(out, "strcat(%s, \"", tmp); print_escaped_string(out, node->right->sval); fputs("\"); ", out); }
            else if (node->right->type == AST_VAR) fprintf(out, "if(%s) strcat(%s, %s); ", node->right->name, tmp, node->right->name);
            fprintf(out, "__register_ptr(%s); %s; })", tmp, tmp);
            break;
        }

        case AST_TOSTRING: {
            /* emit: char* tmp = __num_to_string(<expr>); __register_ptr(tmp); tmp */
            static int tid=0; char t[32]; snprintf(t,sizeof(t),"__toStr%d", tid++);
            fprintf(out, "({ char* %s = __num_to_string((double)(", t);
            generate_node(node->left, out, 0);
            fprintf(out, ")); __register_ptr(%s); %s; })", t, t);
            break;
        }

        case AST_TONUMBER:
            fputs("((double)atof(", out); generate_node(node->left, out, 0); fputs("))", out);
            break;

        case AST_LIST_DECL: {
            const char *elem = (node->list_type == LIST_TYPE_NUMBER) ? "double" : "char*";
            fprintf(out, "/* list %s */\n", node->name);
            indent_spaces(out, indent);
            fprintf(out, "%s *%s;\n", elem, node->name);
            indent_spaces(out, indent);
            fprintf(out, "int %s_size = ", node->name); generate_node(node->size_expr, out, 0); fputs(";\n", out);
            indent_spaces(out, indent);
            fprintf(out, "int %s_capacity = %s_size;\n", node->name, node->name);
            indent_spaces(out, indent);
            fprintf(out, "int %s_count = 0;\n", node->name);
            indent_spaces(out, indent);
            fprintf(out, "%s = (%s*)malloc(sizeof(%s) * %s_capacity);\n", node->name, elem, elem, node->name);
            indent_spaces(out, indent);
            fprintf(out, "__register_ptr(%s);\n", node->name);
            break;
        }

        case AST_LIST_ACCESS:
            fprintf(out, "%s[(int)(", node->name); generate_node(node->index_expr, out, 0); fputs(")]", out);
            break;

        case AST_LIST_FUNC: {
            if (!strcmp(node->func_name, "push_back")) {
                fprintf(out, "/* %s.push_back */\n", node->name);
                indent_spaces(out, indent);
                fprintf(out, "if (%s_count >= %s_capacity) {\n", node->name, node->name);
                indent_spaces(out, indent+4);
                fprintf(out, "%s_capacity *= 2;\n", node->name);
                indent_spaces(out, indent+4);
                Symbol *sym = lookup_symbol(node->name);
                const char *elem = (sym && sym->type == TYPE_LIST_NUMBER) ? "double" : "char*";
                fprintf(out, "%s = (%s*)realloc(%s, sizeof(%s) * %s_capacity);\n",
                        node->name, elem, node->name, elem, node->name);
                indent_spaces(out, indent);
                fputs("}\n", out);
                indent_spaces(out, indent);
                fprintf(out, "%s[%s_count++] = ", node->name, node->name);
                if (node->value_expr) generate_node(node->value_expr, out, 0);
                fputs(";\n", out);
            } else if (!strcmp(node->func_name, "pop_back")) {
                fprintf(out, "/* %s.pop_back */ if (%s_count > 0) %s_count--;\n", node->name, node->name, node->name);
            } else if (!strcmp(node->func_name, "size")) {
                fprintf(out, "%s_count", node->name);
            } else if (!strcmp(node->func_name, "at")) {
                fprintf(out, "%s[", node->name);
                if (node->value_expr) generate_node(node->value_expr, out, 0);
                fputs("]", out);
            } else {
                fprintf(stderr, "Unknown list function: %s\n", node->func_name);
                exit(1);
            }
            break;
        }

        default:
            fprintf(stderr, "Codegen: unhandled AST type %d\n", node->type);
            exit(1);
        }

        node = node->next;
    }
}

/* Helper function to calculate total string length for concatenation */
static void generate_string_length_calc(ASTNode *node, FILE *out) {
    if (node->type == AST_STRING) {
        fprintf(out, "%d", (int)strlen(node->sval));
    } else if (node->type == AST_VAR) {
        fprintf(out, "strlen(%s)", node->name);
    } else if (node->type == AST_TOSTRING) {
        fprintf(out, "32"); /* conservative estimate for number->string using %.15g */
    } else if (node->type == AST_ADD) {
        generate_string_length_calc(node->left, out);
        fprintf(out, " + ");
        generate_string_length_calc(node->right, out);
    } else {
        fprintf(out, "0");
    }
}

/* Helper function to generate string concatenation */
static void generate_string_concat(ASTNode *node, FILE *out, const char *temp_var, int indent) {
    if (node->type == AST_STRING) {
        indent_spaces(out, indent);
        fprintf(out, "strcat(%s, \"", temp_var);
        print_escaped_string(out, node->sval);
        fprintf(out, "\");\n");
    } else if (node->type == AST_VAR) {
        indent_spaces(out, indent);
        fprintf(out, "if(%s) strcat(%s, %s);\n", node->name, temp_var, node->name);
    } else if (node->type == AST_TOSTRING) {
        indent_spaces(out, indent);
        char inner_temp[32];
        static int inner_id = 0;
        snprintf(inner_temp, sizeof(inner_temp), "__numstr%d", inner_id++);
        fprintf(out, "char %s[64]; sprintf(%s, \"%.15g\", (double)(", inner_temp, inner_temp);
        generate_node(node->left, out, 0);
        fprintf(out, ")); strcat(%s, %s);\n", temp_var, inner_temp);
    } else if (node->type == AST_ADD) {
        generate_string_concat(node->left, out, temp_var, indent);
        generate_string_concat(node->right, out, temp_var, indent);
    } else {
        /* generic expression -> convert to string with helper at runtime */
        indent_spaces(out, indent);
        fprintf(out, "char *__tmp_conv = __num_to_string((double)(");
        generate_node(node, out, 0);
        fprintf(out, ")); if(__tmp_conv){ strcat(%s, __tmp_conv); free(__tmp_conv); }\n", temp_var);
    }
}
