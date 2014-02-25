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
 * Compute the number of bytes to serialize record
 */
int fixed_len_sizeof(Record *record);

/**
 * Serialize the record to a byte array to be stored in buf.
 */
void fixed_len_write(Record *record, void *buf);


/**
 * Deserializes from `size` bytes from the buffer, `buf`, and
 * stores the record in `record`.
 */
void fixed_len_read(void *buf, int size, Record * record);

/**
* Initializes a page using the given slot size
*/
void init_fixed_len_page(Page *page, int page_size, int slot_size);

/**
* Calculates the maximal number of records that fit in a page
*/ 
int fixed_len_page_capacity(Page *page);

/**
* Calculate the free space (number of free slots) in the page
*/ 
int fixed_len_page_freeslots(Page *page);

/**
* Add a record to the page
* Returns:
* record slot offset if successful,
* -1 if unsuccessful (page full)
*/
int add_fixed_len_page(Page *page, Record *r);

/**
* Write a record into a given slot.
*/
void write_fixed_len_page(Page *page, int slot, Record *r);

/**
* Read a record from the page from a given slot.
*/
void read_fixed_len_page(Page *page, int slot, Record *r);


void mk_filePage (FILE *out_fp, char* filename, int page_size);

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
	char * page;
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
		Record* next();
};

void merge_runs(FILE *out_fp, 
				RunIterator* iterators, 
				int num_iterators,
			    long buf_size);



#endif
