#ifndef TRANSLATE
#define TRANSLATE

#define MAX_STATEMENT_LEN 1000

char* generate(char* op, int srcVal, char* dest, char* temp);
void assignment(char* target, int param);
void readID(char* inVar);
void writeID(int outVar);


#endif