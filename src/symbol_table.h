#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum { TYPE_NUMBER = 1, TYPE_STRING = 2 } VarType;

typedef struct Symbol {
    char *name;
    VarType type;
    int is_assigned; // for possible future checks
    struct Symbol *next;
} Symbol;

void init_symbol_table();
void free_symbol_table();

void insert_symbol(const char *name, VarType type);
Symbol* lookup_symbol(const char *name);

void check_declaration(const char *name, VarType type);
void check_variable_declared(const char *name);

#endif
