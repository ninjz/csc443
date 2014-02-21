#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <stdlib.h>
#include <string.h>

typedef char Record[9];

/**
* creates runs of length `run_length` in
* the `out_fp`.
*/
void mk_runs(FILE *in_fp, FILE *out_fp, long run_length); 


class RunIterator {
	int curr_pos;
	int size;
	char *buf;
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
	public:
		Record* next();
};

void merge_runs(FILE *out_fp, 
				RunIterator* iterators, 
				int num_iterators,
			    long buf_size);



#endif