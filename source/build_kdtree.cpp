#include <iostream>
using namespace std;

#include "kdtree.h"
#include "kdtreeio.h"
#include "csv_io.h"

using namespace kdtree;

int main (int argc, char *argv[])
{
  if (argc < 3) {
    cout << "Enter the following arguments ..\n"
             "1) Input csv file\n"
             "2) Output kdtree file to save\n";
             "\n";
    return -1;
  }

  string filename = argv[1];
  PointSetSharedPtr <double> point_set (new PointSet <double> ());
  if (!readCSVFile <double> (filename, *point_set)) {
    return -1;
  }

  string outfile = argv[2];
  KdTree <double> kdtree (point_set);
  kdtree.buildTree ();
  kdtree::saveKdTreeToDisk <double> (outfile, kdtree);

  return 0;
}
