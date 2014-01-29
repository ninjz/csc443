#include "library.h"
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <sys/time.h>

int main(int argc,char * argv[]){

	if(argc!= 3 && argc!=4 ){
		printf("Usage: $ csv2heapfile <csv_file> <heapfile> (<page_size>)");
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
	int nbPages=0;
	int nbRecords=0;
	Page p;
	Record r;

	for(int i=0;i<nbAttributes;++i){
		char * temp = (char*)malloc(AttributeSize*sizeof(char));
		r.push_back(temp);
	}
	init_fixed_len_page(&p,pageSize,AttributeSize*nbAttributes);
	

	FILE * csv, *hpfile;
	csv = fopen(argv[1],"r");

	if(csv==NULL){
		printf("EOF %s\n", argv[1]);
	  	exit (EXIT_FAILURE);
	}

	hpfile = fopen(argv[2],"wb+");
	if(hpfile==NULL){
		printf("EOF %s\n", argv[2]);
	  	exit (EXIT_FAILURE);
	}

	Heapfile hf;
	init_heapfile(&hf,pageSize,hpfile);
	PageID pi;
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
			//pretty much only difference to write_fixed_len_pages
			pi = alloc_page(&hf);
			write_page(&p,&hf,pi);

			memset(p.data,0,p.page_size);
			if(add_fixed_len_page(&p,&r) != 0){
				printf("ERROR while writing %s\n",argv[2]);
			}
		}

		//debugging
		/*int ft = ftell(csv);
		read_page(&hf,nbPages-1,&p);
		read_fixed_len_page(&p,nbRecords-1,&r);
		for(Record::iterator it = r.begin(); it != r.end() ;++it){
			char * temp = *it;
			for(int j=0;j<AttributeSize;j++){
				printf("%c",*temp);
					temp++;
			}
			printf(",");
		}
		printf("\n");
		fseek(csv,ft,SEEK_SET);*/
		//debugging end	

		//peeking for eof
		c = fgetc(csv);
		if(c == EOF) break;
		ungetc(c,csv);
	}

	if(fixed_len_page_freeslots(&p) !=  fixed_len_page_capacity(&p)){
		nbPages++;
		pi = alloc_page(&hf);
		write_page(&p,&hf,pi);
	}

	//debugging start
	/*read_page(&hf,nbPages-1,&p);
	read_fixed_len_page(&p,nbRecords-1,&r);
	for(Record::iterator it = r.begin(); it != r.end() ;++it){
		char * temp = *it;
		for(int j=0;j<AttributeSize;j++){
			printf("%c",*temp);
				temp++;
		}
		printf(",");
	}
	printf("\n");*/
	//debugging end

	printf("\nNUMBER OF RECORDS: %d\n",nbRecords);	
	printf("NUMBER OF PAGES: %d\n",nbPages);



	fclose(csv);
	fclose(hpfile);
	for(Record::iterator it = r.begin();it != r.end();++it)
		free(*it); 

	free(p.data);

	clock_gettime(CLOCK_REALTIME,&t1);

	double ti = (t1.tv_sec*1e9+t1.tv_nsec-t.tv_sec*1e9-t.tv_nsec)/1e6;
	printf("TIME: %f milliseconds\n",ti);	

	return 0;
}	
