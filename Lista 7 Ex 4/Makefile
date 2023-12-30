all: lex.yy.c
	gcc -o l7e4 lex.yy.c -ll

lex.yy.c: l7e4.l
	flex l7e4.l

clean:
	rm -rf l7e4    