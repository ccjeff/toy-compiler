scanner: myscanner.c scanner.h lex.yy.c
	lex scanner.l
	gcc myscanner.c lex.yy.c -o scanner.out

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm scanner.out
	rm lex.yy.c