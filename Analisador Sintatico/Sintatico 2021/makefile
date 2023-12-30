all: lex.yy.c sintatico.tab.c
	gcc *.c -I. -o sintatico 
	rm -f *.o lex.yy.c sintatico.tab.c sintatico.tab.h

lex.yy.c: lexico.l
	flex lexico.l 

sintatico.tab.c:
	bison -d -t sintatico.y

clear: 
	rm -f *.o lex.yy.c sintatico.tab.c sintatico.tab.h sintatico
