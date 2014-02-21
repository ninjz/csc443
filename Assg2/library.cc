#include "library.h"

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
	for(Record::iterator it = record->begin();	it!= record->end();++it){
		memcpy(buf,*it,AttributeSize);
		buf = (char*)buf + AttributeSize;
	}
}

/**
 * Deserializes from `size` bytes from the buffer, `buf`, and
 * stores the record in `record`.
 */
void fixed_len_read(void *buf, int size, Record * record){
	for(Record::iterator it = record->begin(); it != record->end(); ++it){
		memcpy(*it,buf,AttributeSize);
		buf = (char*)buf + AttributeSize;	
	}
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
	//int temp1 = (int) fixed_len_page_capacity(page);
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

//HEAPFILE FUNCTIONS

/**
 * Initalize a heapfile to use the file and page size given.
 */
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file){
	heapfile->page_size = page_size;
	heapfile->file_ptr = file;
}

/**
 * Allocate another page in the heapfile.  This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile){
	void * buf = (void *) malloc(heapfile->page_size*sizeof(char));
	
	//calculating offset for free space in dir_page
	int i=0;
	int temp = heapfile->page_size;
	PageID id=0;
	while(temp >>= 1) i++;
	int bytesSize;
	if(i % 8 == 0) bytesSize= i/8;
	else bytesSize = i/8 + 1;
	int nbpages = (heapfile->page_size - sizeof(int))/(i+sizeof(int));
	
	fseek(heapfile->file_ptr,0,SEEK_SET);
	int test = fread(buf,sizeof(char),heapfile->page_size,heapfile->file_ptr);

	void * dir_start = buf;
	
	if(ferror(heapfile->file_ptr)){
		perror ("The following error occurred");
	}
	//new file 
	//if(feof(heapfile->file_ptr)){
	if(!test){
		memset(buf,0,heapfile->page_size);
		(*(int*)((char*)buf+sizeof(int))) = 1;
		fwrite(buf,sizeof(char),heapfile->page_size,heapfile->file_ptr); 
		(*(int*)((char*)buf+sizeof(int))) = 0;
		fwrite(buf,sizeof(char),heapfile->page_size,heapfile->file_ptr); 
		
		//debugging
		/*fseek(heapfile->file_ptr,0,SEEK_SET);

		fread(buf,sizeof(char),heapfile->page_size,heapfile->file_ptr);

		printf("first entry: %d\n",(*(int*)((char*)buf+sizeof(int))));*/
		//debugging end

		free(dir_start);
		return 0;
	}
	
	//go through directory pages
	while((*(int*)buf)){
		id+=nbpages;
		fseek(heapfile->file_ptr,(nbpages+1)*heapfile->page_size,SEEK_CUR);
		fread(buf,sizeof(char),heapfile->page_size,heapfile->file_ptr);
	}
	buf = (char*)buf + sizeof(int);
	i=0;
	while(i <nbpages && (*(int*)buf)){
		id++;
		i++;
		buf = (char *) buf + sizeof(int) + bytesSize;
	}
	
	//fseek(heapfile->file_ptr,-heapfile->page_size,SEEK_CUR);

	// a new directory page is needed first ?
	bool addNewDirectory = (i == nbpages) ? true : false;

	//add a new directory page
	if(addNewDirectory){
		// zerofill
		memset(dir_start,0,heapfile->page_size);

		fseek(heapfile->file_ptr,heapfile->page_size*(id/nbpages)*(nbpages+1),SEEK_SET);

		//update last diretoy page
		*(int*)dir_start = 1;
		fwrite(dir_start,sizeof(char),sizeof(int),heapfile->file_ptr); 
		*(int*)dir_start = 0;

		fseek(heapfile->file_ptr,0,SEEK_END);

		//add new one
		(*(int*)((char*)dir_start + sizeof(int))) = id+1;
		fwrite(dir_start,sizeof(char),heapfile->page_size,heapfile->file_ptr);

		//add empty record page
		(*(int*)((char*)dir_start + sizeof(int))) = 0;
		fwrite(dir_start,sizeof(char),heapfile->page_size,heapfile->file_ptr);		 
		return id;
	}

	fseek(heapfile->file_ptr,heapfile->page_size*(id/nbpages)*(nbpages+1),SEEK_SET);

	//add id +1 because otherwise page id for first is zero ->conflicts with implementation
	*(int*)buf = id+1;
	buf = (char*) buf + sizeof(int);

	for(int j=0;j<bytesSize;j++)
		*((char*)buf+j)= (heapfile->page_size >> (8*j)) & (0xff);
	//update page_dir normal case
	fwrite(dir_start,heapfile->page_size,sizeof(char),heapfile->file_ptr);

	// zerofill
	memset(dir_start,0,heapfile->page_size);


	//allocate new page
	fseek(heapfile->file_ptr,0,SEEK_END);
	fwrite(dir_start,sizeof(char),heapfile->page_size,heapfile->file_ptr);
	
	free(dir_start);
	return id;
}

/**
 * Read a page into memory
 */
void read_page(Heapfile *heapfile, PageID pid, Page *page){
	int i=0;
	int temp = heapfile->page_size;
	while(temp >>= 1) i++;
	int nbpages = (heapfile->page_size - sizeof(int))/(i+sizeof(int));
	int offset = pid/nbpages + pid + 1;
	fseek(heapfile->file_ptr,offset*heapfile->page_size,SEEK_SET);
	fread(page->data,sizeof(char),heapfile->page_size,heapfile->file_ptr);
	fseek(heapfile->file_ptr,0,SEEK_SET);
}

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid){
	// calculating offset for free space in page
	int i=0;
	int temp = heapfile->page_size;
	while(temp >>= 1) i++;
	int bytesSize;
	if(i % 8 == 0) bytesSize= i/8;
	else bytesSize = i/8 + 1;
	int nbpages = (heapfile->page_size - sizeof(int))/(i+sizeof(int));
	int offset = pid/nbpages + pid + 1;

	//not very optimized
	// updating direcotry page
	void * buf = malloc(sizeof(int)+bytesSize*sizeof(char));
	void * buf_start = buf;	

	//updating dir_page
	*(int*)buf = pid+1;
	//buf = (char*)buf + sizeof(int);
	int free_space = AttributeSize*nbAttributes*fixed_len_page_freeslots(page);
	for(int i=0;i<bytesSize;i++){
		*((char*)buf + i+sizeof(int)) = (free_space >> (8*i))  & (0xff) ; 
	}
	
	int offset2=heapfile->page_size*(nbpages+1)*(pid/nbpages)+ (pid % nbpages)*(sizeof(int)+bytesSize)+sizeof(int);
	fseek(heapfile->file_ptr,offset2,SEEK_SET);
	// update of dir_page
	fwrite(buf,sizeof(char),sizeof(int)+bytesSize,heapfile->file_ptr);

	fseek(heapfile->file_ptr,offset*heapfile->page_size,SEEK_SET);
	fwrite(page->data,sizeof(char),heapfile->page_size,heapfile->file_ptr);

	//debugging
	/*fseek(heapfile->file_ptr,0,SEEK_SET);

	fread(buf_start,sizeof(char),heapfile->page_size,heapfile->file_ptr);

	printf("first entry: %d\n",(*(int*)((char*)buf_start+sizeof(int))));*/
	//debugging end


	free(buf_start);
}

/*
 *Initialize RecordIterator
 */
RecordIterator::RecordIterator(Heapfile * heapfile){
	this->hp = heapfile;
	int i=0;
	// calculating nb pages per direcotory page
	int temp = heapfile->page_size;
	while(temp >>= 1) i++;
	this->nbPages_dir = (heapfile->page_size - sizeof(int))/(i+sizeof(int));

	//byte offset in dir_page
	if(i % 8 == 0) this->dir_ent_size = i/8 + sizeof(int);
	else this->dir_ent_size = i/8 + 1 + sizeof(int);
	this->cur=0;
	this->cur_record=0;

	//initializing dir_page
	fseek(hp->file_ptr,0,SEEK_CUR);
	this->dir_page = (void*)malloc(heapfile->page_size*sizeof(char));
	fread(this->dir_page,sizeof(char),heapfile->page_size,heapfile->file_ptr);

	//initializing record
	for(int i=0;i<nbAttributes;++i){
		char * temp = (char*)malloc(AttributeSize*sizeof(char));
		r.push_back(temp);
	}
	
	//initializing working page
	init_fixed_len_page(&(this->cur_page),this->hp->page_size,fixed_len_sizeof(&r));

	//pointer to go through directory
	pos = (char *) dir_page + sizeof(int)+dir_ent_size;

	//assuming that the heapfile contains a least one record
	fread(cur_page.data,sizeof(char),hp->page_size,hp->file_ptr);
}

/*
 *Destructor RecordIterator
 */
RecordIterator::~RecordIterator(){
	free(this->dir_page);
	free(this->cur_page.data);
	for(Record::iterator it = r.begin();it != r.end();++it)
		free(*it); 
}

/*
 * gets next record, assumes hasNext has been called before()
 */

Record RecordIterator::next(){
	// not very optimized
	int nbSlots = fixed_len_page_capacity(&cur_page);

	if(cur_record >= nbSlots){
		//need to get next page
		//next dir page possibly
		if(cur+1>= nbPages_dir){
			fread(dir_page,hp->page_size,sizeof(char),hp->file_ptr);
			pos = (char*) dir_page + sizeof(int);
			cur=0;
		}else{
			cur++;
		}
		fread(cur_page.data,hp->page_size,sizeof(char),hp->file_ptr);
		//set to next page
		pos = (char*) pos + dir_ent_size;
		cur_record=0;
	}

	read_fixed_len_page(&cur_page, cur_record, &r);
	cur_record++;
	return r;
}

bool RecordIterator::hasNext(){
	// not very optimized
	int nbSlots = fixed_len_page_capacity(&cur_page);
	int freeSlots = fixed_len_page_freeslots(&cur_page);
	// no record left
	if(cur_record >= nbSlots - freeSlots){
		// look at next page
		if(0 == freeSlots){
			// end of directory page
			if(cur+1 >= nbPages_dir){
				// assuming that a new page direcotry is only allocated if needed
				if(*((int*)dir_page)){
					return true;	
				//end of directory page and no record left
				}else{
					return false;
				}
			}else{
				//next page exists
				if(*((int*)(pos ))){
					return true;
				//no next page
				}else{
					return false;
				}
			}
		// else finished
		}else{
			return false;
		}
	}
	return true;
}
