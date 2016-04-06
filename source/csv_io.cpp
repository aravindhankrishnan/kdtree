#include "csv_io.h"

#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

template <class T>
static void getOneDataPoint (const string& line, DataPoint <T>& data_point)
{
  istringstream istr (line);
  string token = "";
  char delimiter = ',';
  while (getline (istr, token, delimiter)) {
    double val;
    istringstream d_istr (token);
    d_istr >> val;
    data_point.data.push_back (val);
    //cout << setprecision (19) << val << " ";
  }
  //cout << endl;
}

static int findDimensions (const string& line)
{
  istringstream istr (line);
  string token = "";
  char delimiter = ',';
  int dimensions = 0;
  while (getline (istr, token, delimiter)) {
    dimensions++;
  }
  return dimensions;
}

template <class T>
bool readCSVFile (const string filename, PointSet <T>& point_set)
{
  bool status = true;

  ifstream ifile (filename.c_str ());
  if (!ifile) {
    cerr << "Unable to open file " << filename << endl;
    return false;
  }

  int line_num = 1;

  while (ifile) {

    string line = "";
    getline (ifile, line);
    if (line.empty ()) {
      break;
    }

    if (line_num == 1) {
      point_set.dimensions = findDimensions (line);
      cerr << "\n\nDimnensionality of the data is " << point_set.dimensions << endl;
    }

    DataPoint <T> data_point;
    getOneDataPoint (line, data_point);

    if (data_point.data.size () != point_set.dimensions) {
      status = false;
      cerr << "\n\nError at line " << line_num << " : ";
      cerr << "# of dimensions of this data point (" << data_point.data.size () << ") ";
      cerr << "does not match the dimensions of the point set (" << point_set.dimensions << ")\n";
      break;
    }

    point_set.points.push_back (data_point);

    line_num++;
  }

  if (!status) {
    point_set.points.clear ();
    point_set.dimensions = 0;
    return status;
  }

  point_set.npoints = point_set.points.size ();
  cerr << "Loaded " << filename << " with " << point_set.size () << " points ..\n";
  cerr << "Number of dimensions = " << point_set.dimensions << "\n\n";

  return status;
}

template <class T>
bool writeCSVFile (const string filename, const PointSet <T>& point_set)
{
  bool status = true;

  cerr << "Saving file " << filename << endl;
  ofstream ofile (filename.c_str ());
  if (!ofile) {
    cerr << "Unable to open file " << filename << "\n";
    cerr << "Check file path and permissions ..\n";
    return false;
  }

  ofile << setprecision (18);
  ofile << std::scientific;

  for (const DataPoint <T>& pt : point_set.points) {
    int k = 0;
    for (; k < pt.data.size ()-1; k++) {
      ofile << pt.data[k] << ",";
    }
    ofile << pt.data[k] << "\n";
    ofile.flush ();
  }

  ofile.close ();

  return status;
}


// template instantitations for various data types
template
bool readCSVFile (const string filename, PointSet <double>& point_set);

template
bool readCSVFile (const string filename, PointSet <float>& point_set);

template
bool readCSVFile (const string filename, PointSet <int>& point_set);

template
bool readCSVFile (const string filename, PointSet <long>& point_set);


template
bool writeCSVFile (const string filename, const PointSet <double>& point_set);

template
bool writeCSVFile (const string filename, const PointSet <float>& point_set);

template
bool writeCSVFile (const string filename, const PointSet <int>& point_set);

template
bool writeCSVFile (const string filename, const PointSet <long>& point_set);
