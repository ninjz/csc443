#include "library.h"
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>

int main(int argc,char * argv[]){	

	if(argc != 3){
		printf("Usage:$ read_fixed_len_page <page_file> <page_size>\n");
		return 0;
	}

	struct timespec t,t1;
	clock_gettime(CLOCK_REALTIME,&t);

	FILE * page_file;
	int page_size;
	Page p;
	Record r;
	int nbPages=0;
	int nbRecords=0;

	page_file = fopen(argv[1],"r");
	if(page_file==NULL){
		printf("EOF %s\n", argv[2]);
	  	exit (EXIT_FAILURE);
	}

	page_size = atoi(argv[2]);

	for(int i=0;i<nbAttributes;++i){
		char * temp = (char*)malloc(AttributeSize*sizeof(char));
		r.push_back(temp);
	}	
	init_fixed_len_page(&p,page_size,AttributeSize*nbAttributes);

	int c;
	while(1){
		
		fread(p.data,p.page_size,sizeof(char),page_file);
	
		nbPages++;
		
		// i am assuming that the records of a page are in order
		int nbSlots = fixed_len_page_capacity(&p);
		int freeSlots = fixed_len_page_freeslots(&p);

		for(int i=0;i<nbSlots-freeSlots;i++){
			nbRecords++;
			read_fixed_len_page(&p,i,&r);
			for(Record::iterator it = r.begin(); it != r.end() ;++it){
				char * temp = *it;
				for(int j=0;j<AttributeSize;j++){
					// just for plotting
					printf("%c",*temp);
					temp++;
				}
				printf(",");
			}
			printf("\n");
		}

		//peeking for eof
		c = fgetc(page_file);
		if(c == EOF) break;
		ungetc(c,page_file);
	}

	fclose(page_file);

	for(Record::iterator it = r.begin();it != r.end();++it)
		free(*it); 

	free(p.data);	

	printf("\nNUMBER OF RECORDS: %d\n",nbRecords);	
	printf("NUMBER OF PAGES: %d\n",nbPages);

	clock_gettime(CLOCK_REALTIME,&t1);

	double ti = (t1.tv_sec*1e9+t1.tv_nsec-t.tv_sec*1e9-t.tv_nsec)/1e6;
	/*std::string ti2;
	std::ostringstream convert;
	convert << ti;
	ti2 = convert.str();
	
	perror(ti2.c_str());*/
	printf("TIME: %f milliseconds\n",ti);	

	return 0;
}
	
