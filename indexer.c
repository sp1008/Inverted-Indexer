#include "indexer.h"

int main(int argc, char **argv)
{
	IdxRef reference = malloc(sizeof(struct IndexReference));
	int x, y;
	reference->fileIndex = 0;
	reference->numOfFiles = 0;
	reference->numOfWords = 0;
	
	if(argc != 3){
		printf("ERROR: You must include 2 inputs!");
	}
	
	if(!checkFile(argv[1])){
		printf("Please check the name and start again.\n");
		exit(1);
	}
	struct stat s;
	stat(argv[2], &s);
	if(S_ISDIR(s.st_mode)){
		reference->numOfFiles = getFileNum(argv[2]);
		reference->filePaths = (char**) malloc(sizeof(char*) * reference->numOfFiles);
		reference->filePaths = getFilePath(reference, argv[2]);
	}else{
		reference->numOfFiles = 1;
		reference->filePaths = (char**) malloc(sizeof(char*) * reference->numOfFiles);
		*reference->filePaths = argv[2];
	}

	/* create TOKENIZERS */
	TokenizerT **tok = malloc(sizeof(struct TokenizerT*) * reference->numOfFiles);
	
	for(x=0; x<reference->numOfFiles; x++){
		tok[x] = makeTokens(reference, x);
	}
	
	/* create a SORTED LIST */
	wData *dataTree = malloc(sizeof(wData) * reference->numOfWords);
	for(x=0; x<reference->numOfWords; x++){
		dataTree[x] = malloc(sizeof(struct WordData));
		dataTree[x]->word = (char *) malloc(sizeof(char) * 200);
		dataTree[x]->reoccurence = (int *) calloc(sizeof(int) * reference->numOfFiles + 1, sizeof(int));
	}
	
	cData **counterTree = malloc(sizeof(cData *) * reference->numOfWords);
	for(x=0; x<reference->numOfWords; x++){
		counterTree[x] = malloc(sizeof(cData) * reference->numOfFiles);
		for(y=0; y<reference->numOfFiles; y++){
			counterTree[x][y] = malloc(sizeof(struct CountData));
		}
	}

	SortedListPtr sl = storeTokens(reference, tok, dataTree);
	SortedListIteratorPtr iPman = SLCreateIterator(sl);

	wData iPmanReader;
	x=0;
	while(iPman->current != NULL){
		iPmanReader = SLNextItem(iPman);
		iPmanReader->osl= SLCreate(compareInts, destruct, compareString);
		
		for(y=0; y<reference->numOfFiles; y++){
			tok[y]->currentIndex = 0;
			counterTree[x][y]->filePath = reference->filePaths[y];
			counterTree[x][y]->reoccurence = iPmanReader->reoccurence[y];
			SLnewInsert(iPmanReader->osl, counterTree[x][y], y);
		}
		x++;
	}

	printIndex(sl, argv[1]);
	/* free */
	for(x=0; x<reference->numOfWords; x++){
		for(y=0; y<reference->numOfFiles; y++){
			free(counterTree[x][y]);
		}
		free(counterTree[x]);
	}
	free(counterTree);

	iPman->current = iPman->head->start;
	wData iPmankiller;
	while(iPman->current != NULL){
		iPmankiller = SLNextItem(iPman);
		SLDestroy(iPmankiller->osl);
	}

	for(x=0; x<reference->numOfWords; x++){
		free(dataTree[x]->word);
		free(dataTree[x]->reoccurence);
		free(dataTree[x]);
	}
	free(dataTree);

	SLDestroyIterator(iPman);
	SLDestroy(sl);

	for(x=0; x<reference->numOfFiles; x++){
		free(tok[x]);
	}
	free(tok);

	if(reference->numOfFiles == 1){
		free(reference->filePaths);
	}else{
		for(x=0; x<reference->numOfFiles; x++){
			free(reference->filePaths[x]);
		}
	}
	free(reference->filePaths);
	free(reference);
	
	
	return 0;
}

int checkFile(char *makeFile){
	struct stat s;
	int x = stat(makeFile, &s);
	
	FILE *ofp = fopen(makeFile, "w");
	if(ofp == NULL){
		printf("\nERROR: File could not be created.\n");
		fclose(ofp);
		return 0;
	}
	fclose(ofp);
	
	if(x == 0){
		char c[5];
		int choice = 0;
		printf("Filename that you are trying to create already exists. Rewrite?\n");
		printf("1. Yes\n");
		printf("2. No\n");
		do{
			printf("Input choice:   ");
			fgets(c, 5, stdin);
			choice = atoi(c);
			if(choice != 1 && choice != 2){
				printf("\nOnly integer values between 1 and 2 are accepted.\n");
			}
		}while(choice != 1 && choice != 2);
		if(choice == 2){
			return 0;
		}else{
			return 1;
		}
	}else{
		return 1;
	}
	
}

char **getFilePath(IdxRef reference, char *readFile){
	DIR *DT;
	struct dirent *DTdir;
	struct stat s;
	char tempFile[100] = "";
	
	DT = opendir(readFile);
	
	while((DTdir = readdir(DT)) != NULL){
		if(strcmp(DTdir->d_name, ".") != 0 && strcmp(DTdir->d_name, "..") != 0){
			strcat(tempFile, readFile);
			strcat(tempFile, "/");
			strcat(tempFile, DTdir->d_name);
			stat(tempFile, &s);
			if(S_ISDIR(s.st_mode)){
				getFilePath(reference, tempFile);
			}else{
				reference->filePaths[reference->fileIndex] = (char *) malloc(sizeof(char) * strlen(tempFile) + 1);
				strcpy(reference->filePaths[reference->fileIndex], tempFile);
				reference->filePaths[reference->fileIndex][strlen(tempFile)] = '\0';
				reference->fileIndex++;
				tempFile[0] = '\0';
			}
			tempFile[0] = '\0';
		}
	}
	closedir(DT);
	return reference->filePaths;
}

int getFileNum(char *readFile){
	DIR *DT;
	struct dirent *DTdir;
	DT = opendir(readFile);
	if(DT == NULL){
		printf("ERROR: %s", strerror(errno));
		closedir(DT);
		exit(0);
	}
	
	int numOfFiles = 0;
	struct stat s;
	char tempFile[100] = "";
	
	while((DTdir = readdir(DT)) != NULL){
		if(strcmp(DTdir->d_name, ".") != 0 && strcmp(DTdir->d_name, "..") != 0){
			strcat(tempFile, readFile);
			strcat(tempFile, "/");
			strcat(tempFile, DTdir->d_name);
			stat(tempFile, &s);
			if(S_ISDIR(s.st_mode)){
				numOfFiles += getFileNum(tempFile);
			}else{
				numOfFiles++;
			}
			tempFile[0] = '\0';
		}
	}
	return numOfFiles;
}

int isBinary(char *filename){
	FILE *ifp = fopen(filename, "r");
	unsigned long bufferSZ;
	if(fseek(ifp, 0L, SEEK_END) == 0){
		bufferSZ = ftell(ifp);
		if(bufferSZ == -1){
			printf("ERROR: %s\n", strerror(errno));
			exit(0);
		}
		if(fseek(ifp, 0L, SEEK_SET) != 0){
			printf("ERROR: %s\n", strerror(errno));
		}
	}
	int c;
	while((c = fgetc(ifp)) != EOF){
		if(c == '\0'){
			c = fgetc(ifp);
			if(c == '\0'){
				return 1;
			}
		}
	}
	fclose(ifp);
	return 0;
}

TokenizerT *makeTokens(IdxRef reference, int x){
	char *temp;
	TokenizerT *tok;
	FILE *ifp;
	if(isBinary(reference->filePaths[x])){
		ifp = fopen(reference->filePaths[x], "br");
	}else{
		ifp = fopen(reference->filePaths[x], "r");
	}

	if(ifp == NULL){
		printf("ERROR: %s", strerror(errno));
		exit(1);
	}

	/* getting length of file */

	if(fseek(ifp, 0L, SEEK_END) == 0){
		unsigned long bufferSZ = ftell(ifp);
		if(bufferSZ == -1){
			printf("ERROR: %s\n", strerror(errno));
			exit(0);
		}
		
		temp = malloc(sizeof(char) * (bufferSZ + 1));
		temp[0] = '\0';
		if(fseek(ifp, 0L, SEEK_SET) != 0){
			printf("ERROR: %s\n", strerror(errno));
		}
		/* reading file */
		size_t len = fread(temp, sizeof(char), bufferSZ, ifp);

		if(len != 0){
			temp[len] = '\0';
		}
	}
	tok = TKCreate(temp);
	reference->numOfWords += tok->numWords;
	free(temp);
	return tok;
}

SortedListPtr storeTokens(IdxRef reference, TokenizerT **tok, wData *dataTree){
	SortedListPtr sl = SLCreate(compareStrings, destruct, compareInts);
	int x, y;
	y = 0;
	
	char *temp = malloc(sizeof(char) * 200);
	temp[0] = '\0';
	for(x=0; x<reference->numOfFiles; x++){
		do{
			temp = TKGetNextToken(tok[x]);
			if(temp != NULL){
				strcpy(dataTree[y]->word, temp);
				dataTree[y]->reoccurence[x] = 1;
				SLInsert(sl, dataTree[y], x);
				y++;
			}else{
				break;
			}
		}while(temp != NULL);
	}
	free(temp);
	return sl;
}

int compareStrings(void *p1, void *p2, int x){
	wData s1 = p1;
	wData s2 = p2;
	int result = strcmp(s1->word, s2->word);
	if (result == 0){
		s2->reoccurence[x] += 1;
	}
	return result;
}

int compareString(void *p1, void *p2, int x){
	cData s1 = p1;
	cData s2 = p2;
	return strcmp(s1->filePath, s2->filePath);
}

int compareInts(void *p1, void *p2, int x){
	cData s1 = p1;
	cData s2 = p2;
	return s1->reoccurence - s2->reoccurence;
}

void destruct(void *a){
	free(a);
}

void printIndex(SortedListPtr sl, char *makeFile){
	SortedListIteratorPtr iPman = SLCreateIterator(sl);
	FILE *ofp = fopen(makeFile, "w");
	
	wData iPmanReader;
	cData iTmanReader;
	int x;
	char *temp;
	
	fprintf(ofp, "{\"list\" : [\n");
	while(iPman->current != NULL){
		iPmanReader = SLNextItem(iPman);
		fprintf(ofp, "\t{\"%s\" : [", iPmanReader->word);
		SortedListIteratorPtr iTman = SLCreateIterator(iPmanReader->osl);
		while(iTman->current != NULL){
			iTmanReader = SLNextItem(iTman);
			temp = iTmanReader->filePath;
			x = iTmanReader->reoccurence;
			if(x != 0){
				fprintf(ofp, "\n\t\t{\"%s\" : %d},", temp, x);
			}else{
				break;
			}
		}
		fseek(ofp, -sizeof(char), SEEK_CUR);
		ftruncate(fileno(ofp), ftell(ofp));
		fprintf(ofp, "\n");
		if(iPman->current == NULL){
			fprintf(ofp, "\t]}\n");
		}else{
			fprintf(ofp, "\t]},\n");
		}
		SLDestroyIterator(iTman);
	}
	fprintf(ofp, "]}\n");
	SLDestroyIterator(iPman);
	
	fclose(ofp);
}
