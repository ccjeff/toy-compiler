#ifndef TRANSLATE
#define TRANSLATE

#define MAX_STATEMENT_LEN 10000
typedef char string[33];


/* for <primary> and <expression> */
enum expr { IDEXPR, LITERALEXPR, TEMPEXPR };

typedef struct expression{
	enum expr kind;
	string name;  /* for IDEXPR, TEMPEXPR */
	int val;	     /* for LITERALEXPR */
} expr_rec;

char* generate(char* op, expr_rec source, expr_rec dest, char* temp);
void assignment(expr_rec target, expr_rec source);
void readID(expr_rec inVar);
void writeID(expr_rec outVar);
expr_rec gen_infix(expr_rec e1, char* op, expr_rec e2);
void finalize();

#endif