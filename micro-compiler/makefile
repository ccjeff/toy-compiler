scanner: scanner.c scanner.h lex.yy.c
	cc scanner.c lex.yy.c -ll -o scanner 

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm scanner
	rm lex.yy.c