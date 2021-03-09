#ifndef parser_symbols
#define parser_symbols

#define bufferSize 100
#define MAX_FILENAME 100
#define MAXIDLEN 32

typedef struct {
    char id[32]; // IDs associate with at most 32 chars
    int val;
} SYMBOL;

void initSymbolTable();
void assignEntry(char* sym);
void updateSymbolTable(char* sym, int val);
int findIdx(char* sym);
int symbolVal(char* sym);
char* get_temp(void);

#endif