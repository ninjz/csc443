#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "leveldb/db.h"
 
typedef char Record[9];


using namespace std;



int main(int argc, char ** argv){

	/**
	 * opens a database connection to "./leveldb_dir"
	 */
	leveldb::DB *db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "./leveldb_dir", &db);


	leveldb::WriteOptions();
	leveldb::ReadOptions();

	if (argc != 3){
		printf("proper usage: bsort <input_file> <out_index>\n");
		return 0;

	}
	FILE * input_file = fopen(argv[1], "r");
	if (input_file == NULL){
		exit(0);
	}
	FILE * out_index = fopen(argv[2], "w");
	if (out_index == NULL){
		exit(0);
	}

	//read records
	char c;
	int unique_counter=0;
	char unique_key[9 + sizeof(long)];
	while (1){
		Record record; 
		//read one record at a time 
		fread(record, sizeof(Record), 1, input_file);
		leveldb::Slice value = record;
		unique_counter ++;
   		memcpy(unique_key, record, 9); 
    	memcpy(unique_key + 9, &unique_counter, sizeof(long)); 
 		db->Put(leveldb::WriteOptions(), leveldb::Slice(unique_key, sizeof(unique_key)), value); 

		c = fgetc(input_file);
		if (c==EOF) goto cont;
		ungetc(c, input_file);
	}

	cont: 
	leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next()) {
	    leveldb::Slice key = it->key();
	    leveldb::Slice value = it->value();
	    string key_str = key.ToString();
	    string val_str = value.ToString();
	    fwrite(key_str.c_str(), sizeof(Record), 1, out_index);
	    cout << key_str << ":"  << val_str << endl;
	}
	assert(it->status().ok());  // Check for any errors found during the scan
	delete it;


	fclose(input_file);
	fclose(out_index);

}
