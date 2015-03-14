#include "tokenizer.h"

TokenizerT *TKCreate(char *inputString) {
	TokenizerT *tok = malloc(sizeof(TokenizerT));
	tok->capturedString = (char *)calloc(strlen(inputString)+1,1);
	tok->state = (char *)calloc(strlen(inputString)+1,1);
	tok->numWords = 0;
	
	stateMachine(tok, inputString);
	tok->currentIndex = 0;
	return tok;
}

void TKDestroy(TokenizerT *tok) {
	free(tok->state);
	free(tok->capturedString);
	free(tok);
}

char *TKGetNextToken(TokenizerT *tok) {
	char *word = (char *)calloc(strlen(tok->capturedString)+1, 1);
	if(tok->state[tok->currentIndex] == '\0'){
		return NULL;
	}else if(tok->state[tok->currentIndex] == 'x'){
		tok->currentIndex++;
		free(word);
		return TKGetNextToken(tok);
	}else{
		int start, finish;
		start = tok->currentIndex;
		while(tok->state[tok->currentIndex] == '0'){
			tok->currentIndex++;
		}
		finish = tok->currentIndex;
		int x;
		int y = start;
		for(x=0 ; x<=finish-start; x++, y++){
			word[x] = tolower(tok->capturedString[y]);
		}
		word[x] = '\0';
		tok->currentIndex++;
		
		return word;
	}
}

void stateMachine(TokenizerT *tok, char *string){
	/* sets state to W or x*/
	int index, counter;

	for(index=0; index<strlen(string)+1; index++){
		tok->stringPtr = &string[index];
		counter = 0;
		if(isalpha(string[index])){
			counter += alphaLoop(tok, string, index+counter);
			index += counter-1;
			tok->state[index] = 'W'; /* for word token*/
			tok->numWords += 1;
			continue;
		}else{
			tok->capturedString[index] = string[index];
			tok->state[index] = 'x';
		}
	}
	tok->state[index-1] = '\0';
	tok->currentIndex = 0;	
}

int alphaLoop(TokenizerT *tok, char *string, int index){
	int counter = 0;
	for(; isalnum(*tok->stringPtr); tok->stringPtr++){
		tok->capturedString[index + counter] = string[index + counter];
		tok->state[index + counter] = '0';
		counter++;
	}
	return counter;
}

int setTokenizer(TokenizerT *tok, char *string, int index, int counter){
	tok->capturedString[index + counter] = string[index + counter];
	tok->state[index + counter] = '0';
	tok->stringPtr++;
	return 1;
}

