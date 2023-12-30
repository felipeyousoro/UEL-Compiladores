all: lex.yy.c
	gcc -o l7e3 lex.yy.c -ll

lex.yy.c: l7e3.l
	flex l7e3.l

clean:
	rm -rf l7e3
    