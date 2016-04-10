

-------------------------------------------------------------------
Implementation of a 'k' dimensional kdtree. 'k' can be arbitrary.
-------------------------------------------------------------------



1. Tools needed
===============

1) g++
2) cmake

2. How to compile
=================

1) Go to 'kdtree' folder
2) mkdir build
3) cd build
4) cmake ../source
5) make

3. Creating datasets
===================================

./generate_nd_dataset 5 10000 sample_data_5d.csv

The above command creates 10000 points of 5 dimensions and stores it to the output file sample_data_5d.csv

'5' is just an arbitrary number, any number can be chosen

Similarly, we can create a query dataset as follows
./generate_nd_dataset 5 1000 query_data_5d.csv

4. To run
=========

1) ./build_kdtree sample_data_5d.csv sample_data_5d.kdtree

The above command creates a kdtree from the input set of points (sample_data_5d.csv) and writes it to the disk
(sample_data_5d.kdtree)

2) ./query_kdtree sample_data_5d.kdtree query_data_5d.csv query_data_result_5d_kdtree.csv

The above command takes the following inputs (i) The kdtree file (2) Query data set (3) Output result file. 
The closest point indices are stored in the  output result file.

3) ./brute_force_search sample_data_5d.csv query_data_5d.csv query_data_5d_result_bruteforce.csv 

The above command runs a brute force search for the nearest point. Inputs are (1) Input dataset (2) Query dataset (3)
  Output result file.

To verify the result of kdtree search we compare the output result files created in Steps 2 and 3.

Running the command "diff query_data_5d_result_kdtree.csv query_data_5d_result_bruteforce.csv" will give an empty result.

