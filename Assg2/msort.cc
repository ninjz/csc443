
#include "msort.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>





int main(int argc, char * argv[]){

	if (argc != 5){
		printf("proper usage: msort <input_file> <out_file> <mem_capacity> <k>\n");
		return 0;

	}

	FILE * in_fp, *out_fp;
	int mem_capacity, k;

	if (! (k = atoi(argv[4]))){
		printf("Error converting k to integer\n");

	}
	if (! (mem_capacity = atoi(argv[3]))){
		printf("Error converting mem_capcity to int\n");

	}
	int page_size = mem_capacity / k;

	mk_filePage(in_fp, argv[1],page_size);

	return 0;

}
