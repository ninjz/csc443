#makefile
#Group names: c3volkel, c1chengc, g3amr
CC = g++
CFLAGS =  -g -ggdb -ansi -pedantic -Wall 
LIBRARY = -lrt
EXECUTABLES = write_fixed_len_pages read_fixed_len_pages csv2heapfile heapfile2csv query
all: $(EXECUTABLES)
	     
library.o: library.cc library.h
	$(CC) -o $@ $(CFLAGS) -c $<

write_fixed_len_pages: write_fixed_len_pages.cc library.o
	$(CC) -o $@ $(CFLAGS) $< library.o $(LIBRARY)

read_fixed_len_pages: read_fixed_len_pages.cc library.o
	$(CC) -o $@ $(CFLAGS) $< library.o $(LIBRARY) 
				      
csv2heapfile: csv2heapfile.cc library.o
	$(CC) -o $@ $(CFLAGS) $< library.o $(LIBRARY) 
							       
heapfile2csv: heapfile2csv.cc library.o
	$(CC) -o $@ $(CFLAGS) $< library.o $(LIBRARY) 
										        
query : query.cc library.o
	$(CC) -o $@ $(CFLAGS) $< library.o $(LIBRARY) 

clean:
	rm $(EXECUTABLES) *.o
