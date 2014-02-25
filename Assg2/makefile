#makefile
#Group names: c1chengc, g3amr
CC = g++
CFLAGS =  -g -ggdb -ansi -pedantic -Wall 
LIBRARY = -lrt
EXECUTABLES = msort gRecords bsort
LEVELDB = leveldb-1.15.0
all: $(EXECUTABLES)
	     
library.o: msortLibrary.cc msort.h
	$(CC) -o $@ $(CFLAGS) -c $<

msort: msortLibrary.cc library.o
	$(CC) -o $@ $(CFLAGS) $< library.o $(LIBRARY)

gRecords: generateRecords.cc library.o
	$(CC) -o $@ $(CFLAGS) $< library.o $(LIBRARY) 
				      
bsort: bsort.cc 
	$(CC) -o $@ $(CFLAGS)  $(LEVELDB)/libleveldb.a -lpthread -I $(LEVELDB)/include $< 

clean:
	rm $(EXECUTABLES) *.o
