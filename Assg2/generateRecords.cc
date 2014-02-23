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

	const char * buf2 = "6543210\0\n";
	for (int i=0; i< nbRecords; i++){

		fwrite(buf2, sizeof(char), 9, file);

	}


	const char * buf = "0123456\0\n";
	for (int i=0; i< nbRecords; i++){

		fwrite(buf, sizeof(char), 9, file);

	}
	const char * buf1 = "7654321\0\n";
	for (int i=0; i< nbRecords; i++){

		fwrite(buf1, sizeof(char), 9, file);

	}




	fclose(file);

	return 0;
}
