#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static Symbol *symbol_table = NULL;

void init_symbol_table() {
    symbol_table = NULL;
}

void free_symbol_table() {
    Symbol *cur = symbol_table;
    while (cur) {
        Symbol *next = cur->next;
        free(cur->name);
        free(cur);
        cur = next;
    }
    symbol_table = NULL;
}

void insert_symbol(const char *name, VarType type) {
    if (lookup_symbol(name)) {
        fprintf(stderr, "Error: Variable '%s' already declared\n", name);
        exit(1);
    }
    Symbol *sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->is_assigned = 0;
    sym->next = symbol_table;
    symbol_table = sym;
}

Symbol* lookup_symbol(const char *name) {
    Symbol *cur = symbol_table;
    while (cur) {
        if (strcmp(cur->name, name) == 0)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

void check_declaration(const char *name, VarType type) {
    Symbol *sym = lookup_symbol(name);
    if (sym) {
        fprintf(stderr, "Error: Variable '%s' already declared\n", name);
        exit(1);
    }
    insert_symbol(name, type);
}

void check_variable_declared(const char *name) {
    Symbol *sym = lookup_symbol(name);
    if (!sym) {
        fprintf(stderr, "Error: Variable '%s' used before declaration\n", name);
        exit(1);
    }
}
