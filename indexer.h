#ifndef INDEXER_H
#define INDEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include "sorted-list.h"
#include "tokenizer.h"

struct IndexReference{
	int numOfFiles;
	int numOfWords;
	char **filePaths;
	int fileIndex;
};

struct WordData{
	char *word;
	int *reoccurence;
	SortedListPtr osl;
};

struct CountData{
	int reoccurence;
	char *filePath;
};

typedef struct IndexReference *IdxRef;
typedef struct WordData *wData;
typedef struct CountData *cData;

int checkFile(char *);
char **getFilePath(IdxRef, char*);
int getFileNum(char *);
TokenizerT *makeTokens(IdxRef, int);
SortedListPtr storeTokens(IdxRef, TokenizerT**, wData *);
int getOccurence(TokenizerT**, char *, int);
int compareStrings(void *, void *, int);
int compareString(void *, void *, int);
int compareInts(void *, void *, int);
void destruct(void *);
void printIndex(SortedListPtr, char *);

#endif
