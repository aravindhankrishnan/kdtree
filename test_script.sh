cd build
./build_kdtree sample_data_5d.csv sample_data_5d.kdtree 
./query_kdtree sample_data_5d.kdtree query_data_5d.csv results1.txt
./brute_force_search sample_data_5d.csv query_data_5d.csv results2.txt
echo "Diff"
diff results1.txt results2.txt 
