# MapReduceWithQueue

This is an implementation of the MapReduce techinique in C++, using the producer and consumer problem.
The Map function produces pairs and adds them to the shuffle buffer. The reduce worker consumes the buffer generating a report with the number of each word on a set of files.
