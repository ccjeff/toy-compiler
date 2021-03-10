lex.yy.c: scanner.l
	lex scanner.l

scanner:
	lex scanner.l
	gcc ./test_utils/scanner.c lex.yy.cc -o scanner.out
	
clean:
	rm micro.out
	rm lex.yy.c
	rm y.tab.c
	rm y.tab.h
	rm a.txt

micro:
	bison -yd parser.y
	lex scanner.l
	gcc translate.c lex.yy.c y.tab.c parser.c -o micro.out