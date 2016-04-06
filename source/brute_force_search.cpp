#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
using namespace std;

#include "csv_io.h"

template <class T>
void findClosestPoint (const PointSet <T>& data_point_set, 
                       const DataPoint <T>& point,
                       int& closest_point_index,
                       DataPoint <T>& closest_point,
                       double& min_distance)
{
  min_distance = std::numeric_limits <T>::max ();

  for (int i = 0; i < data_point_set.points.size (); i++) {

    const DataPoint <T>& current_point = data_point_set.points[i];
    double distance = DataPoint <T>::distance (current_point, point);

    if (distance < min_distance) {
      closest_point_index = i;
      min_distance = distance;
      closest_point = current_point;
    }
  }
}

int main (int argc, char *argv[])
{
  if (argc < 4) {
    cout << "Enter the following arguments ..\n"
             "1) csv file (data)\n"
             "2) csv file (test)\n"
             "3) csv file (output)\n"
             "\n";
    return -1;
  }

  string data_filename = argv[1];
  PointSet <double> data_point_set;
  if (!readCSVFile <double> (data_filename, data_point_set)) {
    return -1;
  }

  string test_filename = argv[2];
  PointSet <double> test_point_set;
  if (!readCSVFile <double> (test_filename, test_point_set)) {
    return -1;
  }

  string outputfile = argv[3];
  ofstream ofile (outputfile.c_str ());
  if (!ofile) {
    cout << "Unable to open output file " << outputfile << "\n";
    return -1;
  }
  cout << "Writing to output file " << outputfile << endl;

  //ofile << std::scientific;
  //ofile << std::setprecision (18);

  for (int i = 0; i < test_point_set.points.size (); i++) {
    int closest_point_index = -1;
    DataPoint <double> closest_point;
    double closest_point_distance = 0.;
    findClosestPoint <double> (data_point_set, test_point_set.points[i], closest_point_index, 
                               closest_point, closest_point_distance);
    //cout << "Point " << test_point_set.points[i] << "\t";
    //cout << "Nearest Point " << closest_point << "\n";
    //ofile << closest_point_index << "," << closest_point_distance << endl;
    ofile << closest_point_index << endl;
  }
  ofile.close ();

  return 0;
}
