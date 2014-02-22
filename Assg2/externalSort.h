class Stream {
    /**
     * construct a stream of elements in D
     * starting with i to j
     */
    Stream(D, int i, int j);
 
    /**
     * gets the next element in the stream.
     * If the stream has reached its end (D[j] or end of D)
     * then we return ∞.
     */
    V next();
}