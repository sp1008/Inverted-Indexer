#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct TokenizerT_ {
	char *capturedString;
	int currentIndex;
	char *stringPtr;
	char *state;
	int numWords;
};

typedef struct TokenizerT_ TokenizerT;

TokenizerT *TKCreate(char *);
void TKDestroy(TokenizerT *);
char *TKGetNextToken(TokenizerT *);
void printToken(TokenizerT *, char *);
void stateMachine(TokenizerT *, char *);
int alphaLoop(TokenizerT *, char *, int);
int numLoop(TokenizerT *, char *, int);
int setTokenizer(TokenizerT *, char *, int, int);

#endif
