#include "library.h"
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <sys/time.h>
#include <map>

int main(int argc,char * argv[]){

	if(argc!= 4 && argc!= 5 ){
		printf("Usage: $ query <heapfile> <start> <end> (<page_size>)");
		return 0;
	}

	int pageSize,start,end;
	if(argc==4){
		pageSize= 10000;
	}else{
		pageSize=atoi(argv[4]);	
	}
	start = atoi(argv[2]);
	end = atoi(argv[3]);

	struct timespec t,t1;
	clock_gettime(CLOCK_REALTIME,&t);
	int nbRecords=0;
	char * temp = (char*)malloc(AttributeSize*sizeof(char));
	//char * attri2 = malloc(AttributeSize*sizeof(char));
	//Page p;
	Record r;
	Heapfile hf;

	std::map<std::string,int> m;

	FILE  *hpfile;
	hpfile = fopen(argv[1],"r");
	if(hpfile==NULL){
		printf("EOF %s\n", argv[2]);
	  	exit (EXIT_FAILURE);
	}
	
	init_heapfile(&hf,pageSize,hpfile);

	RecordIterator RI (&hf);

	while(RI.hasNext()){

		int A1;
		r= RI.next();
		
		//debugging
		/*for(Record::iterator it = r.begin(); it != r.end() ;++it){
				char * temp = *it;
				for(int j=0;j<AttributeSize;j++){
					// just for plotting
					printf("%c",*temp);
					temp++;
				}
				printf(",");
			}
			printf("\n");
		*/
		//debugging end


		A1 = *((int*)r[0]);

		if(A1 >= start && A1 <= end){
		
			std::string str(reinterpret_cast<char*>(r[1]), 10);	
			m[str.substr(0,5)]++;
			nbRecords++;

		}
	}

	for(std::map<std::string,int>::iterator it = m.begin(); it != m.end(); ++it){
		//std::cout<<it->first<<"  "<< it->second<<std::endl;
	}

	free(temp);

	printf("\nNUMBER OF RECORDS IN RANGE: %d\n",nbRecords);	

	fclose(hpfile);
	clock_gettime(CLOCK_REALTIME,&t1);

	double ti = (t1.tv_sec*1e9+t1.tv_nsec-t.tv_sec*1e9-t.tv_nsec)/1e6;
	printf("TIME: %f milliseconds\n",ti);	

	return 0;
}
