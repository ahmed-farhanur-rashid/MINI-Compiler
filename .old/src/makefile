all: mini

mini: parser.tab.c lex.yy.c ast.o symbol_table.o codegen.o main.o
	gcc -o mini parser.tab.c lex.yy.c ast.o symbol_table.o codegen.o main.o -lfl

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

ast.o: ast.c ast.h
	gcc -c ast.c

symbol_table.o: symbol_table.c symbol_table.h
	gcc -c symbol_table.c

codegen.o: codegen.c ast.h symbol_table.h
	gcc -c codegen.c

main.o: main.c ast.h symbol_table.h
	gcc -c main.c

clean:
	rm -f *.o lex.yy.c parser.tab.c parser.tab.h mini out.c out.exe
