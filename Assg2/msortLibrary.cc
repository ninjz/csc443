
#include "msort.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/** 
 * Compute the number of bytes to serialize record
 */
int fixed_len_sizeof(Record *record){
	return nbAttributes*AttributeSize;
}

/**
 * Serialize the record to a byte array to be stored in buf.
 */
void fixed_len_write(Record *record, void *buf){
	//for(Record::iterator it = record->begin();	it!= record->end();++it){

		memcpy(buf,record,AttributeSize*nbAttributes);
		buf = (char*)buf + AttributeSize*nbAttributes;
	//}
}

/**
 * Deserializes from `size` bytes from the buffer, `buf`, and
 * stores the record in `record`.
 */
void fixed_len_read(void *buf, int size, Record * record){
	//for(Record::iterator it = record->begin(); it != record->end(); ++it){
		memcpy(record,buf,AttributeSize*nbAttributes);
		buf = (char*)buf + AttributeSize*nbAttributes;	
	//}
}



/**
* Initializes a page using the given slot size
*/
void init_fixed_len_page(Page *page, int page_size, int slot_size){
	page->data = malloc(page_size*sizeof(char)); 
	page->page_size = page_size;
	page->slot_size = slot_size;
	memset(page->data,0,page_size);
	char * temp = (char *) page->data + page->page_size - sizeof(int);
	*(int*)temp = (int) fixed_len_page_capacity(page);
}

/**
* Calculates the maximal number of records that fit in a page
*/ 
int fixed_len_page_capacity(Page *page){
	int records = (int)page->page_size/(nbAttributes*AttributeSize);
	int overhead = sizeof(int) + records/8 + 1 ; 
	return (page->page_size-overhead)/(nbAttributes*AttributeSize);
}

/**
* Calculate the free space (number of free slots) in the page
*/ 
int fixed_len_page_freeslots(Page *page){
	int res=0;
	int nbSlots = fixed_len_page_capacity(page);
	int Slot_offset = (nbSlots % 8) ? (nbSlots/8 + 1) : nbSlots/8;
	char * endfile = (char*)page->data+page->page_size-Slot_offset-sizeof(int);
	for(int i=0;i<nbSlots;i++){
		if(!(endfile[(int)(i/8)] & 1 << (i % 8))) res++;
	}
	return res;
}
 
/**
* Add a record to the page
* Returns:
* record slot offset if successful,
* -1 if unsuccessful (page full)
*/
int add_fixed_len_page(Page *page, Record *r){
	int nbSlots = fixed_len_page_capacity(page);
	int res = 0;
	int Slot_offset = (nbSlots % 8) ? (nbSlots/8 + 1) : nbSlots/8;
	char * endfile = (char*) page->data+page->page_size-Slot_offset-sizeof(int);
	while(res<nbSlots && (endfile[(int)(res/8)] & 1 << (res % 8))) res++;
	if(res>=nbSlots) return -1;
	fixed_len_write(r,(char*)page->data + res*fixed_len_sizeof(r));
	endfile[(int)(res/8)] |= 1 << (res % 8);
	return res;
}
 
/**
* Write a record into a given slot.
*/
void write_fixed_len_page(Page *page, int slot, Record *r){
	fixed_len_write(r,(char*)page->data+slot*fixed_len_sizeof(r));
}
 
/**
* Read a record from the page from a given slot.
*/
void read_fixed_len_page(Page *page, int slot, Record *r){
	int size = fixed_len_sizeof(r);
	fixed_len_read((char*)page->data+slot*size,size,r);	
}







void mk_filePage (FILE *out_fp, char* filename, int page_size){


	FILE *file = fopen(filename, "r");
	if (file ==NULL){
		printf("Error openning file:%s\n",filename);
		exit(0);
	}
	out_fp = fopen("in_fp", "w");
	if(out_fp == NULL){
		exit(0);
	}

	//defined for initalizing records and pages.
	Record r;
	Page p;
	init_fixed_len_page(&p,page_size, AttributeSize*nbAttributes); // initialize the page 
	
	char c;
	while (1){

		fread(r,AttributeSize, nbAttributes, file);
		printf("record is %s\n", r);

		if(add_fixed_len_page(&p,&r) == -1){  //page is full

			fwrite(p.data,p.page_size,sizeof(char),out_fp);
			memset(p.data, 0, p.page_size);
			if(add_fixed_len_page(&p,&r) !=0){
				printf("ERROR while writing to %s\n","out_fp");

			}
		}

		//take a peak at the next char if its EOF break the loop
		c = getc(file);
		if (c == EOF){
			break;
		}
		ungetc(c,file);
	}

	if(fixed_len_page_freeslots(&p) !=  fixed_len_page_capacity(&p)){
		fwrite(p.data,p.page_size,sizeof(char),out_fp);
	}
	fclose(file);
	fclose(out_fp);


	//free(r);
	free(p.data);



}


/**

used for qsort


**/
int cmpstr(void const *a, void const *b) { 
    char const *aa = (char const *)a;
    char const *bb = (char const *)b;

    return strcmp(aa, bb);
}

/**
* creates runs of length `run_length` in
* the `out_fp`. this uses an in-memory sort algorithm
*/
void mk_runs(FILE *in_fp, FILE *out_fp, long run_length){



	// //*****************Done by Calvin**************************
	// char * buf;
	// int amount_read;
	// buf = (char *) malloc(run_length);
	
 //    while ( ! feof (in_fp) ){
 //    	// Grab run_length amount of data and put in buf to be sorted.
	// 	amount_read = fread(buf , run_length, 1, in_fp);	
 //    	// sort data in buf.
	// 	qsort((void *)buf, amount_read, sizeof(Record), cmpstr);
	// 	fputs(buf, out_fp);
 //    }

 //    fclose(in_fp);
 //    fclose(out_fp);
	// free(buf);

	
	//run_length is the number of records that can fit in the memory
	char c;
	char * buf;
	printf("size of record is %ld\n",sizeof(Record));
	buf = (char *) malloc(run_length*sizeof(Record));
	int nbrecords;
	while(1){

		//if not the end of the file
		nbrecords = fread(buf, sizeof(Record), run_length, in_fp);

 
		printf("nbrewacorsd i %d\n", nbrecords);
		qsort((void *) buf, nbrecords, sizeof(Record) ,cmpstr);
		fwrite(buf,sizeof(Record), nbrecords,out_fp);
		num_iterators ++;

		c = fgetc(in_fp);
		if (c ==EOF){

			break;
		}
		ungetc(c, in_fp);
	}
	free(buf);

}

/**
* fp is an open file to read from ,
* start_pos is the position in the file fp to start reading from
* run_length is the number of records per run
* buf_size is the page_size of records used for the runs.
**/
RunIterator::RunIterator(FILE *fp, long start_pos, long run_length, long buf_size){
	this->curr_pos = 0; // record offset in the run
	this->start_pos = start_pos; 
	this->size = run_length; //size of the run --> how many records is there for each run
	this->buf = (char *)malloc(buf_size * sizeof(Record)); //size of the page used for runs to fill it in. 
	fseek(fp, start_pos, SEEK_SET); // seek to start pos
	fread(this->buf, sizeof(Record), buf_size,fp); //read one page from the run in the fp file


}

Record* RunIterator::next(){
	Record *record;
	if(this->curr_pos >= this->size){
		return NULL;
	}
	strncpy((char *)record, (char *)this->buf[this->curr_pos], sizeof(Record));
	this->curr_pos += sizeof(Record);
	return record;
}





void merge_runs(FILE *out_fp, 
				RunIterator iterators[], 
				int num_iterators,
			    long buf_size)
{

	int i = 0; //used to make sure we merge all iterators.
	long nbRecords = 0; //used to make sure that we don't exceed the page_size 
	Record * buffer = (Record *) malloc(buf_size); // memory buffer
	long nbPages = run_length / page_size; //number of pages per iterator
	long nbindex = 0; //used to iterate throgh pages in runs


	while (nbindex < nbPages){ // while there is still more pages in the runs
		int i=0;
		while (i < num_iterators){//we haven't gone through all the iterators 
			while((i*page_size < buf_size)){	//there is space to put it in memory and we haven't reached the end of the iterator
				for (int j=0; j<page_size; j++){ // fill the buf with records from the page
					strncpy((char *) buffer[i+j],(char *)iterators[i].next(),sizeof(Record));
				}
				iterators[i].start_pos += page_size;
				i++;
			}
			//sort it 
			qsort((void *) buffer, nbRecords, sizeof(Record), cmpstr);
			//write it out 
			fwrite(buffer, buf_size, 1,out_fp);
		}
		nbindex++;
	}

	free(buffer);
}


int main(int argc, char * argv[]){

	FILE * in_fp, *out_fp;
	int mem_capacity, k;

//*** checking if we got correct format and no erros happen while oppening files **************************
	if (argc != 5){ 																					//*
		printf("proper usage: msort <input_file> <out_file> <mem_capacity> <k>\n"); 					//*
		return 0; 																						//*
 																										//*
	} 																									//*
 																										//*
 																										//*
 																										//*
	if (! (k = atoi(argv[4]))){ 																		//*
		printf("Error converting k to integer\n"); 														//* 														//*
 																										//*
	} 																									//*
	if (! (mem_capacity = atoi(argv[3]))){ 																//*
		printf("Error converting mem_capcity to int\n");												//*
 																										//*
	} 																									//*
 																										//*
	out_fp = fopen(argv[2],"w"); 																		//*
	if(out_fp == NULL){ 																				//*
		exit(0); 																						//*
	} 																									//*
 																										//*			
	in_fp = fopen(argv[1], "r"); 																		//*
	if(in_fp == NULL){ 																					//*
		exit(0); 																						//*
 																										//*
	} 																									//*
//*********************************************************************************************************






//************************** create runs of memory_capacity sorted **************************************************************
	run_length = mem_capacity/sizeof(Record); // initalize run length is the number of records we can fit into memroy    //*
	buf_size = run_length; // buf_size is going to be the number of records we can fit into memeory ALWAYS				  //*
	page_size = buf_size/k+1; // page_size is the number of records we can fit per page 								  //*
																															  //*
																															  //*
	//create runs of size mem_capacity and that is mem_capacity-sorted then we can use these in merge sort to sort them out.  //*
	num_iterators = 0;  																									  //*
	mk_runs(in_fp, out_fp, run_length); 																					  //*
	fclose(in_fp);																											  //*
	fclose(out_fp);	
																									  //*
//*******************************************************************************************************************************




//********************* use the runs that are mem_capacity and merge them together to get a sorted file ***************************
																																//*
//*check for errors whle reading the file and initalize necessaly variables****** 												//*	 																		  //*												//*
	RunIterator *iteratorsArray;                                              //*												//*
						                                                      //*												//*
	char * buf;    															  //*												//*
	int start_pos=0;                       									  //*												//*
																																//*
	//merge runs from out_fp
	out_fp = fopen(argv[2],"r");                                              //*												//*	
	if(out_fp == NULL){														  //*												//*
		exit(0);                                                              //*												//*
	}	
	in_fp = fopen(argv[1], "w");																	  //*												//*
	if (in_fp == NULL){
		exit(0);
	}
//*******************************************************************************												//*


// ***************** mergin happens here ***************************************************************
	while (num_iterators >=1){
		//read iterator from out_fp;
		for (int j = 0; j< num_iterators; j++){ //initialize iterators 
			iteratorsArray[j] = RunIterator::RunIterator(out_fp, start_pos, run_length, page_size);
			start_pos+=run_length;
		}
		merge_runs(in_fp, iteratorsArray, num_iterators, buf_size);
		num_iterators = num_iterators / k; //decrease the number of iterators
		run_length = run_length * k; //increase the run_length
		std::swap(in_fp, out_fp); //swap out_fp and in_fp

	}
	std::swap(in_fp, out_fp); //swap out_fp and in_fp
	fclose(out_fp);
	fclose(in_fp);
	return 0;

}
