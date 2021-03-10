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

/*generate 3 addr code for assign operation and infix operation.
* for infix operation, there will always be a temp variable
* but for the assign the temp is intenionally set to ""
*/
char* generate(char* op, expr_rec source, expr_rec dest, char* temp);
/*
* generate code for assign operation in micro. calling generate
*/
void assignment(expr_rec target, expr_rec source);
/* generate 3 addr code for read operation*/
void readID(expr_rec inVar);
/* generate 3 addr code for write operation*/
void writeID(expr_rec outVar);
/* generate 3 addr code for infix operation, with unknown number of adds e.g. */
expr_rec gen_infix(expr_rec e1, char* op, expr_rec e2);
/* generate 3 addr code for end*/
void finalize();

#endif