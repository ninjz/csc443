
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

		c = fgetc(in_fp);
		if (c ==EOF){

			break;
		}
		ungetc(c, in_fp);
	}
	free(buf);

	// //*********************Added by Amr*******************************

	//1.read run_length records into buffer the first time
	// int nbRecords =0;
	// Page p;
	// Record r;
	// char c;
	// printf("this is the size of page_size%d\n", page_size);
	// printf("this is the run_length %d \n ", (int)run_length);


	// in_fp = fopen("in_fp", "r");
	// if(in_fp == NULL){
	// 	exit(0);
	// }

	// init_fixed_len_page(&p,page_size,AttributeSize*nbAttributes);
	// char * buf = (char *) malloc(run_length);
	// int offset = 0;
	// //int nbRecordsForlastPage = 0;

	// while(1){ //keep running until there is no more pages in a file


	// 	while (run_length - nbRecords >= page_size/9){ // read pages into memory  until we can't fit any more pages

	// 		fread(p.data,p.page_size,sizeof(char),in_fp); //read one page from in_fp filepage
	// 		int nbSlots = fixed_len_page_capacity(&p); 
	// 		int freeSlots = fixed_len_page_freeslots(&p);

	// 	printf("this is the rnbSlots %d \n ", (int)nbSlots);
	// 	printf("this is the freeSlots %d \n ", (int)freeSlots);
	// 		for(int i=0;i<nbSlots-freeSlots;i++){

	// 			nbRecords++;
	// 			read_fixed_len_page(&p,i,&r);
	// 			memcpy(buf+ offset, r, nbAttributes*AttributeSize); //copy into buf
	// 			printf("buffer is now %s\n", buf);
	// 			offset +=nbAttributes*AttributeSize;
	// 			//memset(p.data,0,p.page_size);
	// 		}

	// 		//check for the 
	// 		c= fgetc(in_fp);
	// 		if (c == EOF ) break;
	// 		ungetc(c, in_fp);


	// 	}


	// 	qsort((void *) buf, run_length, sizeof(Record), cmpstr);
	// 	fwrite(buf,run_length, sizeof(char), out_fp);
	// 	//set nbRecords to 0 in order to use for another run
	// 	nbRecords =0;
	// 	memset(p.data,0,p.page_size);

	// 	//check for the 
	// 	c= fgetc(in_fp);
	// 	if (c == EOF ) break;
	// 	ungetc(c, in_fp);

	// }

	// qsort((void *) buf, nbRecords, sizeof(Record), cmpstr);
	// fwrite(buf,nbRecords, sizeof(char), out_fp);


	// free(p.data);
	// free(buf);
	// fclose(in_fp);
	// fclose(out_fp);
}




// /**
// * currently does not use page reads.
// */

// RunIterator::RunIterator(FILE *fp, long start_pos, long run_length, long buf_size){


// 	//************** added by Calvin**************************
// 	this->curr_pos = 0;
// 	this->size = run_length;
// 	this->buf = (char *)malloc(buf_size);
// 	fseek(fp, start_pos, SEEK_SET); // seek to start pos
// 	fread(this->buf, sizeof(char), run_length, fp);


// 	//************* added by Amr *****************************

// 	//file has to be read into pages.



RunIterator::RunIterator(FILE *fp, long start_pos, long run_length, long buf_size){


// 	//************** added by Calvin**************************
// 	this->curr_pos = 0;
// 	this->size = run_length;
// 	this->buf = (char *)malloc(buf_size);
// 	fseek(fp, start_pos, SEEK_SET); // seek to start pos
// 	fread(this->buf, sizeof(char), run_length, fp);


// 	//************* added by Amr *****************************

// 	//file has to be read into pages.

	//defined for initalizing records and pages.

	
	Record r;
	Page p;
	init_fixed_len_page(&p,buf_size, sizeof(Record)); // initialize the page 
	char c;
	long nbRecords =0;
	while (nbrecords < run_length){
		fseek (fp, start_pos, SEEK_SET); 
		fread(r,AttributeSize, nbAttributes, fp);
		printf("record is %s\n", r);
		nbrecords++;

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

	free(p.data);

}

Record* RunIterator::next(){
	Record *record;
	if(this->curr_pos >= this->size){
		return NULL;
	}
	strncpy((char *)record, (const char *)this->buf[this->curr_pos], sizeof(Record));
	this->curr_pos += sizeof(Record);
	return record;
}





void merge_runs(FILE *out_fp, 
				RunIterator iterators[], 
				int num_iterators,
			    long buf_size)
{
	Record *buf[buf_size];  // idk if this is how u do this
	RunIterator *iter;
	int done_iterators = 0;

	while(done_iterators < num_iterators){
		int num = 0;

		done_iterators = 0;

		for(int i = 0; i < num_iterators; i++){
			Record *temp = iterators[i].next();
			if(temp != NULL){
				buf[i] = temp;
				num++;
				done_iterators++;
			}

		}

		qsort((void *)buf, num, sizeof(Record), strcmp);
		fputs((const char *)buf,out_fp);
	}
	
}





// }

// Record* RunIterator::next(){
// 	Record *record;
// 	if(this->curr_pos >= this->size){
// 		return NULL;
// 	}
// 	strncpy((char *)record, (const char *)this->buf[this->curr_pos], sizeof(Record));
// 	this->curr_pos += sizeof(Record);
// 	return record;
// }


// void merge_runs(FILE *out_fp, 
// 				RunIterator iterators[], 
// 				int num_iterators,
// 			    long buf_size)
// {
// 	Record *buf[buf_size];  // idk if this is how u do this
// 	RunIterator *iter;
// 	int done_iterators = 0;

// 	while(done_iterators < num_iterators){
// 		int num = 0;

// 		done_iterators = 0;

// 		for(int i = 0; i < num_iterators; i++){
// 			Record *temp = iterators[i].next();
// 			if(temp != NULL){
// 				buf[i] = temp;
// 				num++;
// 				done_iterators++;
// 			}

// 		}

// 		qsort((void *)buf, num, sizeof(Record), strcmp);
// 		fputs((const char *)buf,out_fp);
// 	}
	
// }
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

	out_fp = fopen(argv[2],"w");
	if(out_fp == NULL){
		exit(0);
	}

	in_fp = fopen(argv[1], "r");
	if(in_fp == NULL){
		exit(0);

	}

	page_size = mem_capacity / k;
	//make page files of size page_size
	//mk_filePage(in_fp, argv[1],page_size);
	//each record is of size 9 so the length of the first run is 
	long run_length = mem_capacity/9;
	//create runs of size mem_capacity and that is mem_capacity sorted. 
	mk_runs(in_fp, out_fp, run_length);

	fclose(in_fp);
	fclose(out_fp);
	//merge runs from out_fp

	return 0;

}
