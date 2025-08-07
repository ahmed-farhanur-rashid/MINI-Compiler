bison -d parser.y
flex lexer.l
gcc parser.tab.c lex.yy.c symbol_table.c ast.c codegen.c main.c -o mini.exe
pause