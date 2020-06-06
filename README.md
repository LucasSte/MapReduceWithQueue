# MapReduceWithQueue

This is an implementation of the MapReduce techinique in C++, using the producer and consumer problem.
The Map function produces pairs and adds them to the shuffle buffer. The shuffle worker consumes the buffer and sort the keys to insert on the reduce buffers.
