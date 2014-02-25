#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <stdlib.h>
#include <string.h>

//for fixed size records
const int nbAttributes = 9;
const int AttributeSize = 1;
int num_iterators;
long run_length;
long page_size;
long nbPages;
long buf_size;


typedef char Record[9];
typedef struct {
    void *data;
    int page_size;
 	int slot_size;
} Page;

typedef int PageID;
typedef char Record[9];
     
typedef struct {
	int page_offset;
	int slot;
} RecordID;




int cmpstr(void const *a, void const *b);


/**
* creates runs of length `run_length` in
* the `out_fp`.
*/
void mk_runs(FILE *in_fp, FILE *out_fp, long run_length); 


class RunIterator {

	public:
	int curr_pos;
	int size;
	char *buf;
	int start_pos;
	char *page;
	int page_offset;
	/**
	* creates an iterator using the `buf_size` to
	* scan through a run that starts at `start_pos`
	* with length `run_length`
	*/

	RunIterator(FILE *fp, long start_pos, long run_length, long buf_size);
	/**
	* reads the next record. Returns null if
	* iterator reads the end of the run.
	*/
		char * next();
};

void merge_runs(FILE *out_fp, 
				RunIterator* iterators, 
				int num_iterators,
			    long buf_size);



#endif
