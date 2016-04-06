#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
using namespace std;

#define NUM_POINTS 1000

int main (int argc, char *argv[])
{
  if (argc < 4) {
    cout << "Enter the following arguments ..\n"
            "1) Number of dimensions\n"
            "2) Number of points\n"
            "3) Output filename (csv)\n"
            "\n";
    return -1;
  }

  string n_dimensions_str = argv[1];
  int ndimensions = std::stoi (n_dimensions_str);
  cout << "Number of dimensions = " << ndimensions << endl;

  int npoints = std::stoi (argv[2]);
  cout << "Number of points = " << npoints << endl;

  string outfilename = argv[3];

  ofstream ofile (outfilename.c_str ());
  if (!ofile) {
    cout << "Unable to open file " << outfilename << endl;
    return -1;
  }

  cout << "Wrting to file " << outfilename << endl;

  ofile << std::scientific;
  ofile << std::setprecision (18);

  srand (time (NULL));
  for (int i = 0; i < npoints; i++) {
    double val = 0.;
    for (int z = 0; z < ndimensions-1; z++) {
      val = double (rand ()) / RAND_MAX;
      ofile << val << ",";
    }
    val = double (rand ()) / RAND_MAX;
    ofile << val << "\n";
  }
  ofile.close ();


  return 0;
}
