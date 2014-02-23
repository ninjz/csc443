#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

int main(int argc, char* argv[]){

	if (argc != 3){
		printf("Proper usage is <file> <nbRecords>\n");
		return 0;
	}
	FILE *file;
	if (! (file = fopen(argv[1],"w"))){
		printf("couldn't ope n %s\n", argv[1]);
		return 0;

	}
	int nbRecords = atoi(argv[2]);
	char * buf = "01234567\0\n";

	for (int i=0; i< nbRecords; i++){

		fwrite(buf, sizeof(char), 9, file);

	}

	return 0;
}