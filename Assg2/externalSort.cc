
#import "externalSort.h"
#import "library.h"


/**
 * Input: D must be n-sorted
 * Output: E will be kn-sorted
 */
void merge(D, E, n) {
    const int k = ...; // a constant
    Stream incomings[k];
    V      buf[k];
 
    for(int offset=0; offset < D.size(); offset += n⋅k) {
        // initialize streams and merge buffer
        for(int i=0; i < k; i++) {
            incomings[i] = Stream(D, offset+i⋅n, offset+(i+1)⋅n)
            buf[i] = incoming[i].next()
        }
        // perform k-way merge
        int min_val, min_idx
        loop {
            min_val, min_idx = get_minimum(buf);
            E.append(min_val);
            buf[min_idx] = incomings[min_idx].next();
 
        } until(∀i≤k, buf[i] == ∞)
    }
}



void merge_sort(D) {
    const int k = ...; // a constant
    E = allocate(D.size()) // byte array D.size()×∥V∥
    int n = 1; // the length of sorted segments in D
    while(n < D.size()) {
        merge(D, E, n); // E is kn-sorted
        D, E = E, D; // swap D, E as pointers in O(1)
        n = k ⋅ n; // now, D is kn sorted
    }
}



void extern_merge_sort(D) {
    /**
     * Create large sorted segments in D
     * MAX_MEMORY_SIZE is the maximum number of
     * elements that the main memory can hold.
     t
     */
    void *buf = allocate(MAX_MEMORY_SIZE);
    for(int i=0; i <= MAX_MEMORY_SIZE / D.size(); i++) { 
        Disk::read(D, into=buf, offset=i*MAX_MEMORY_SIZE, size=MAX_MEMORY_SIZE)
        main_memory_sort(buf);
        Disk::write(D, from=buf, offset=i*MAX_MEMORY_SIZE, size=MAX_MEMORY_SIZE);
    } 
    free(buf);
 
 
    // ... invoke merge(D, E, MAX_MEMORY_SIZE) ...
 
 
}