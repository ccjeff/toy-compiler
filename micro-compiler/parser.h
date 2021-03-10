#ifndef parser_symbols
#define parser_symbols

#define bufferSize 100
#define MAX_FILENAME 100
#define MAXIDLEN 32

typedef struct {
    char id[32]; // IDs associate with at most 32 chars
    int val;
} SYMBOL;

/*initialize values for symbolTable*/
void initSymbolTable();
/*assign new entry for variable*/
void assignEntry(char* sym);
/*update value stored in symbolTable*/
void updateSymbolTable(char* sym, int val);
/*find the index of variable in symbol table*/
int findIdx(char* sym);
/*return value for symbol in symbol table*/
int symbolVal(char* sym);
/*get name of a new temp register*/
char* get_temp(void);

#endif