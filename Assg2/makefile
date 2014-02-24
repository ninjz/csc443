#makefile
#Group names: c1chengc, g3amr
CC = g++
CFLAGS =  -g -ggdb -ansi -pedantic -Wall 
LIBRARY = -lrt
EXECUTABLES = msort gRecords bsort
LEVELDB = ~/leveldb-1.4.0
all: $(EXECUTABLES)
	     
library.o: msortLibrary.cc msortLibrary.h
	$(CC) -o $@ $(CFLAGS) -c $<

msort: msortLibrary.cc library.o
	$(CC) -o $@ $(CFLAGS) $< library.o $(LIBRARY)

gRecords: generateRecords.cc library.o
	$(CC) -o $@ $(CFLAGS) $< library.o $(LIBRARY) 
				      
bsort: bsort.cc library.o
	$(CC) -o $@(CFLAGS)  $(LEVELDB)/libleveldb.a -lpthread -l $(LEVELDB)/include $< library.o 

clean:
	rm $(EXECUTABLES) *.o
