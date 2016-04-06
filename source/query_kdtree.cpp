#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#include "kdtree.h"
#include "kdtreeio.h"
#include "csv_io.h"

using namespace kdtree;

int main (int argc, char *argv[])
{
  if (argc < 4) {
    cout << "Enter the following arguments ..\n"
             "1) Input kdtree file\n"
             "2) Input csv file\n"
             "3) Output result file to save\n";
             "\n";
    return -1;
  }

  string kdtree_filename = argv[1];
  KdTree <double> kdtree;
  if (!kdtree::loadKdTreeFromDisk <double> (kdtree_filename, kdtree)) {
    return -1;
  }

  string csv_filename = argv[2];
  PointSetSharedPtr <double> point_set (new PointSet <double> ());
  if (!readCSVFile <double> (csv_filename, *point_set)) {
    return -1;
  }

  string result_file = argv[3];

  ofstream ofile (result_file.c_str ());
  if (!ofile) {
    cout << "Unable to open output file " << result_file << "\n";
    return -1;
  }
  cout << "Writing to output file " << result_file << endl;

  //ofile << std::scientific;
  //ofile << std::setprecision (18);

  for (int i = 0; i < point_set->points.size (); i++) {
    int closest_point_index = -1;
    DataPoint <double> closest_point;
    double closest_point_distance = 0.;
    kdtree.nearestNeighbour (point_set->points[i], closest_point_index, closest_point, closest_point_distance);
    //ofile << closest_point_index << "," << closest_point_distance << endl;
    ofile << closest_point_index << endl;
  }
  ofile.close ();
  return 0;
}
