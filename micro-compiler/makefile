scanner: myscanner.c scanner.h lex.yy.c
	lex scanner.l
	gcc myscanner.c lex.yy.c -o scanner.out

lex.yy.c: scanner.l
	lex scanner.l

clean:
	# rm scanner.out
	rm micro.out
	rm lex.yy.c
	rm y.tab.c
	rm y.tab.h

micro:
	bison -yd parser.y
	lex scanner.l
	gcc lex.yy.c y.tab.c parser.c translate.c -o micro.out