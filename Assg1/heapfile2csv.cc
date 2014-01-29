#include "library.h"
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <sys/time.h>

int main(int argc,char * argv[]){

	if(argc!= 3 && argc!=4 ){
		printf("Usage: $ heapfile2csv <heapfile> <csv_file> (<page_size>)");
		return 0;
	}

	int pageSize;
	if(argc==3){
		pageSize= 10000;
	}else{
		pageSize=atoi(argv[3]);	
	}

	struct timespec t,t1;
	clock_gettime(CLOCK_REALTIME,&t);
	int nbRecords=0;
	//Page p;
	Record r;
	Heapfile hf;

	FILE * csv, *hpfile;
	hpfile = fopen(argv[1],"r");
	if(hpfile==NULL){
		printf("EOF %s\n", argv[2]);
	  	exit (EXIT_FAILURE);
	}
	
	csv = fopen(argv[2],"w");
	if(csv==NULL){
		printf("EOF %s\n", argv[1]);
	  	exit (EXIT_FAILURE);
	}

	init_heapfile(&hf,pageSize,hpfile);

	RecordIterator RI (&hf);
	
	while(RI.hasNext()){
		r = RI.next();
		nbRecords++;

		char c=',';
		for(Record::iterator it = r.begin(); it != r.end() ;++it){
			*it;
			fwrite(*it,AttributeSize,sizeof(char),csv);
			fwrite(&c,sizeof(char),sizeof(char),csv);
		}
		c='\n';
		fwrite(&c,sizeof(char),sizeof(char),csv);
	}

	printf("\nNUMBER OF RECORDS: %d\n",nbRecords);	
	//printf("NUMBER OF PAGES: %d\n",nbPages);

	fclose(csv);
	fclose(hpfile);
	/*for(Record::iterator it = r.begin();it != r.end();++it)
		free(*it); 

	free(p.data);*/

	clock_gettime(CLOCK_REALTIME,&t1);

	double ti = (t1.tv_sec*1e9+t1.tv_nsec-t.tv_sec*1e9-t.tv_nsec)/1e6;
	printf("TIME: %f milliseconds\n",ti);	

	return 0;
}
	


