
#include "msort.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




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

// void merge_runs(FILE *out_fp, 
// RunIterator iterators[], 
// int num_iterators,
//    long buf_size)
// {

// char buf[buf_size]; // idk if this is how u do this
// RunIterator *iter;
// int done_iterators = 0;


// while(done_iterators < num_iterators){
// int num = 0;

// done_iterators = 0;
// int pos = 0;

// for(int i = 0; i < num_iterators; i++){
// char * temp = iterators[i].next();
// if(temp != NULL){
// for(int j = 0; j < sizeof(Record); j++){
// buf[pos] = temp[j];
// pos++;
// }
// num++;

// } else {
// done_iterators++;
// }

// }

// qsort((void *)buf, num, sizeof(Record), cmpstr);
// fputs((const char *)buf,out_fp);
// }
// 
// }
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
this->buf = (char *) malloc(buf_size); //size of the page used for runs to fill it in. 
this->fp = fp;
fseek(fp, start_pos, SEEK_SET); // seek to start pos
fread(this->buf, sizeof(char), run_length, fp);
}

char * RunIterator::next(){
char record[9];
if(this->curr_pos >= this->size){
return NULL;
}
for (int i= 0; i< sizeof(Record);i++){
record[i] = this->buf[this->curr_pos];
this->curr_pos ++;
}	

printf("record is %s\n", record);
return record;
}

// /**
// * fp is an open file to read from ,
// * start_pos is the position in the file fp to start reading from
// * run_length is the number of records per run
// * buf_size is the page_size of records used for the runs.
// **/
// RunIterator::RunIterator(FILE *fp, long start_pos, long run_length, long buf_size){
// 	this->curr_pos = 0; // record offset in the run
// 	this->start_pos = start_pos; 
// 	this->size = run_length; //size of the run --> how many records is there for each run
// 	this->buf = (char **) malloc(buf_size * sizeof(Record)); //size of the page used for runs to fill it in. 
// 	this->page = (char *) malloc(page_size * sizeof(Record));// initialize the page 
// 	this->page_offset = 0;
// 	fseek(fp, start_pos, SEEK_SET); // seek to start pos
// 	fread(this->buf ,sizeof(Record) ,this->size,fp);
// 	printf("iterator buffer is %s\n", this->buf);
	


// }

// char * RunIterator::next(){
// 	char record[9];
// 	if(this->curr_pos >= this->size){
// 		return NULL;
// 	}
// 	for (int i= 0; i< sizeof(Record);i++){
// 		record[i] = this->buf[this->curr_pos][i];
// 		//this->curr_pos++;
// 	}	
// 	this->curr_pos ++;
// 	printf("record is %s\n", record);
// 	return record;
// }



// void merge_runs(FILE *out_fp, 
// 				RunIterator iterators[], 
// 				int num_iterators,
// 			    long buf_size)
// {


// 	int i = 0 ;//keep index of the iterators
// 	Record r;
// 	int write_page_offset =buf_size - page_size;
// 	int page_offset= 0;
// 	char * memmory_buffer = (char *) malloc(buf_size * sizeof(Record));
// 	while (i < num_iterators){ 												//we haven't gone throgh all the iterators 
// 		int offset=0;
// 		while (i*page_size < buf_size -page_size ){							// we haven't filled the memory yet, and because one page is for writing out
// 			for (int j=0; j< page_size; j++){	
// 											//load the page from the iterator at i
// 				if (strncpy(r,(char *)iterators[i].next(),sizeof(Record))){
// 					printf("r is %s\n",r);
// 					strncpy((char *)(iterators[i].page+page_offset), r, sizeof(Record)); //load the record to the page 
// 					page_offset+=sizeof(Record);
// 					printf("page is %s\n", iterators[i].page);
// 				}
// 			} 
// 			iterators[i].start_pos +=page_size;								// since we wrote one page already
// 			strncpy((char *)(memmory_buffer + offset), iterators[i].page, page_size);
// 							printf("today was a FUCKED UP DAY\n");
// 			offset+=page_size;
// 			i++; 															//increment the index for the iterators 
// 		}
// 		int write_record_offset = 0;
// 		Record min_record; 
// 		int page_offset =0; 												// this is used to determine which page to replace in memory
// 		strncpy(min_record, memmory_buffer, sizeof(Record)); 				//initialize the buffer to the first record in the first page 
// 		while(write_record_offset < page_size){
// 			for (int k = 0;  k < i;k++){ // get the minimum record
// 				if (strcmp((char *) min_record, (char *)(iterators[i-k].page + iterators[i-k].page_offset)) <0){		 //we found a minimum record
// 					strncpy((char*)memmory_buffer[write_record_offset],(char *)(iterators[i-k].page[ (int)iterators[i-k].page_offset]), sizeof(Record));
// 					write_record_offset+=sizeof(Record);
// 					iterators[i-k].page_offset+=sizeof(Record);
// 					if (iterators[i-k].page_offset == page_size/sizeof(Record)){ //load a page into memory 
// 						for (int j=0; j< page_size; j++){								//load the page from the iterator at i
// 							if (strncpy(r,(char *)iterators[i-k].next(),sizeof(Record))){
// 								strncpy(iterators[i-k].page, (char *) r, sizeof(Record)); //load the record to the page 
// 							}
// 						} 
// 						iterators[i-k].start_pos +=page_size;	
// 						strncpy((char *)(memmory_buffer + (i-k)*sizeof(Record)), iterators[i-k].page, page_size);
// 					}

// 				}

// 			}
// 		}
// 		fwrite(memmory_buffer,page_size, 1, out_fp);
// 		//get the minimum record from a page and put it into the buffer page.

// 	}
// 	for (int z=0; z < num_iterators; z++){
// 		free (iterators[z].buf);
// 		free (iterators[z].page);
// 	}
// 	free(memmory_buffer);
// }
	// int i = 0; //used to make sure we merge all iterators.
	// long nbRecords = 0; //used to make sure that we don't exceed the page_size 
	// Record * buffer = (Record *) malloc(buf_size * sizeof(Record)); // memory buffer
	// long nbPages = run_length / page_size; //number of pages per iterator
	// long nbindex = 0; //used to iterate throgh pages in runs


	// while (nbindex < nbPages){ // while there is still more pages in the runs
	// 	int i=0;
	// 	while (i <= num_iterators){//we haven't gone through all the iterators 
	// 		while((i*page_size < buf_size)){	//there is space to put it in memory and we haven't reached the end of the iterator
	// 			int offset =i;
	// 			for (int j=0; j<page_size; j++){ // fill the buf with records from the page
	// 							strncpy((char *)buffer[offset],(char *)iterators[i].next(),sizeof(Record));
	// 							printf("before merge_runs4\n");
	// 			}
	// 			iterators[i].start_pos += page_size;
	// 			i++;
	// 		}
	// 		qsort((void *) buffer, nbRecords, sizeof(Record), cmpstr);
	// 		//write it out 
	// 		fwrite(buffer, buf_size, 1,out_fp);
	// 	}
	// 	nbindex++;
	// }
	// free(buffer);
void merge_runs(FILE *out_fp, RunIterator iterators[], int num_iterators, long buf_size)
{

	char buf[buf_size]; // idk if this is how u do this
	RunIterator *iter;
	int done_iterators = 0;


	while(done_iterators < num_iterators){
		int num = 0;

	done_iterators = 0;
	int pos = 0;

	for(int i = 0; i < num_iterators; i++){
		char * temp = iterators[i].next();
		if(temp != NULL){
		for(int j = 0; j < sizeof(Record); j++){
		buf[pos] = temp[j];
		pos++;
	}
	num++;

	} else {
		done_iterators++;
	}

	}

	qsort((void *)buf, num, sizeof(Record), cmpstr);
	fputs((const char *)buf,out_fp);
	}

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





//clock_gettime(CLOCK_REALTIME,&t);
//************************** create runs of memory_capacity sorted **************************************************************
	run_length = mem_capacity/sizeof(Record); // initalize run length is the number of records we can fit into memroy    //*
	printf("this is run_length%ld\n",run_length );
	buf_size = run_length; // buf_size is going to be the number of records we can fit into memeory ALWAYS
	printf("this is buf_size%ld\n",buf_size );				  //*
	page_size = buf_size/k+1; // page_size is the number of records we can fit per page 	
	printf("this is page_size%ld\n",page_size );							  //*
																															  //*
																															  //*
	//create runs of size mem_capacity and that is mem_capacity-sorted then we can use these in merge sort to sort them out.  //*
	num_iterators = 0;  																									  //*
	mk_runs(in_fp, out_fp, run_length); 																					  //*
	fclose(in_fp);																											  //*
	fclose(out_fp);	
																									  //*
// //*******************************************************************************************************************************
// printf("this is the number of itereators %d\n", num_iterators);



// //********************* use the runs that are mem_capacity and merge them together to get a sorted file ***************************
// 																																//*
// //*check for errors whle reading the file and initalize necessaly variables****** 												//*	 																		  //*												//*
// 	                                             //*												//*
// 						                                                      //*												//*
//   																			  //*												//*
// 	long start_pos=0;                       									  //*												//*
// 	char * buf;																															//*
// 	//merge runs from out_fp
// 	out_fp = fopen(argv[2],"r");                                              //*												//*	
// 	if(out_fp == NULL){														  //*												//*
// 		exit(0);                                                              //*												//*
// 	}	
// 	in_fp = fopen(argv[1], "w");																	  //*												//*
// 	if (in_fp == NULL){
// 		exit(0);
// 	}
// //*******************************************************************************												//*


// FILE *from, *to;
// from = out_fp;
// to = in_fp;
// // ***************** mergin happens here ***************************************************************
// 	while (num_iterators >=1){
// 		//read iterator from out_fp;
// 		RunIterator *iteratorsArray = (RunIterator *) malloc(sizeof(RunIterator) * num_iterators); 	
// 		for (int j = 0; j< num_iterators; j++){ //initialize iterators 
// 			//iteratorsArray[j] = (RunIterator *) malloc(sizeof(RunIterator));
// 			iteratorsArray[j] = RunIterator::RunIterator(out_fp, start_pos, run_length, page_size);

// 			start_pos+=run_length;
// 		}
		
// 		merge_runs(from, iteratorsArray, num_iterators, buf_size);
// 		num_iterators = num_iterators / k; //decrease the number of iterators
// 		run_length = run_length * k; //increase the run_length
// 		std::swap(from, to); 
// 		free(iteratorsArray);

// 	}
// 	std::swap(from, to);
// 	out_fp = from ;
	// fclose(out_fp);
	// fclose(in_fp);

	//clock_gettime(CLOCK_REALTIME,&t1);
	//double ti = (t1.tv_sec*1e9+t1.tv_nsec-t.tv_sec*1e9-t.tv_nsec)/1e6;
	//printf("TIME: %f milliseconds\n",ti);	
	return 0;

}
