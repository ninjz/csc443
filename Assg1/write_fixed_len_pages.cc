#include "library.h"
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <sys/time.h>

int main(int argc,char * argv[]){	
	if(argc!= 4){
		printf("Usage: $ write_fixed_len_pages <csv_file> <page_file> <page_size>");
		return 0;
	}
	struct timespec t,t1;
	clock_gettime(CLOCK_REALTIME,&t);
	//clock_t t = clock();
	int nbPages=0;
	int nbRecords=0;
	int pageSize = atoi(argv[3]);
	Page p;
	Record r;
	for(int i=0;i<nbAttributes;++i){
		char * temp = (char*)malloc(AttributeSize*sizeof(char));
		r.push_back(temp);
	}
	init_fixed_len_page(&p,pageSize,AttributeSize*nbAttributes);

	FILE * csv, *page_file;
	csv = fopen(argv[1],"r");

	if(csv==NULL){
		printf("EOF %s\n", argv[1]);
	  	exit (EXIT_FAILURE);
	}

	page_file = fopen(argv[2],"w");
	if(page_file==NULL){
		printf("EOF %s\n", argv[2]);
	  	exit (EXIT_FAILURE);
	}
	
	char c;
	while(1){

		for(Record::iterator it = r.begin(); it != r.end(); ++it){
			fread(*it,AttributeSize,sizeof(char),csv);
			fscanf(csv,"%c",&c); // ignoring commas
			//printf("%s\n",*it);
		}

		nbRecords++;
		
		if(add_fixed_len_page(&p,&r) == -1){
			nbPages++;
			fwrite(p.data,p.page_size,sizeof(char),page_file);
			memset(p.data,0,p.page_size);
			if(add_fixed_len_page(&p,&r) != 0){
				printf("ERROR while writing %s\n",argv[2]);
			}
		}
		
		//peeking for eof
		c = fgetc(csv);
		if(c == EOF) break;
		ungetc(c,csv);
	}

	if(fixed_len_page_freeslots(&p) !=  fixed_len_page_capacity(&p)){
		nbPages++;
		fwrite(p.data,p.page_size,sizeof(char),page_file);
	}

	printf("\nNUMBER OF RECORDS: %d\n",nbRecords);	
	printf("NUMBER OF PAGES: %d\n",nbPages);



	fclose(csv);
	fclose(page_file);
	for(Record::iterator it = r.begin();it != r.end();++it)
		free(*it); 

	free(p.data);

	clock_gettime(CLOCK_REALTIME,&t1);


	double ti = (t1.tv_sec*1e9+t1.tv_nsec-t.tv_sec*1e9-t.tv_nsec)/1e6;
	printf("TIME: %f milliseconds\n",ti);	


	return 0;
}
