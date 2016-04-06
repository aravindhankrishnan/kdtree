#include <iostream>
using namespace std;

#include "csv_io.h"

int main (int argc, char *argv[])
{
  if (argc < 2) {
    cout << "Enter the following arguments ..\n"
             "1) Input csv file\n"
             "\n";
    return -1;
  }

  string filename = argv[1];
  PointSet <double> point_set;
  readCSVFile <double> (filename, point_set);
  writeCSVFile <double> ("test.csv", point_set);

  return 0;
}
