// #include "library.h"
#include "msort.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




/**
* creates runs of length `run_length` in
* the `out_fp`.
*/
void mk_runs(FILE *in_fp, FILE *out_fp, long run_length)
{
	char * buf;
	int amount_read;

	buf = (char *) malloc(run_length);
	
    while ( ! feof (in_fp) ){
    	// Grab run_length amount of data and put in buf to be sorted.
		amount_read = (int)fread(buf , sizeof(char), run_length, in_fp);		
    	// sort data in buf.
		qsort((void *)buf, amount_read/sizeof(Record), sizeof(Record), strcmp);
		fputs(buf, out_fp);
    }

    fclose(in_fp);
    fclose(out_fp);
	
	free(buf);
}
/**
* currently does not use page reads.
*/

RunIterator::RunIterator(FILE *fp, long start_pos, long run_length, long buf_size){
	this->curr_pos = 0;
	this->size = run_length;
	this->buf = (char *)malloc(buf_size);
	fseek(fp, start_pos, SEEK_SET); // seek to start pos
	fread(this->buf, sizeof(char), run_length, fp);

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

