#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <memory>

template <class T>
struct DataPoint
{
  std::vector <T> data;

  template <class T1>
  friend std::ostream& operator << (std::ostream& cout, const DataPoint<T1>& point);

  static double distance (const DataPoint <T>& p1, const DataPoint <T>& p2)
  {
    double distance = 0;
    for (int z = 0; z < p1.data.size (); z++) {
      distance += ((p1.data[z] - p2.data[z]) * (p1.data[z] - p2.data[z]));
    }
    return std::sqrt (distance);
  }

  ~DataPoint ()
  {
    data.clear ();
  }
};

template <class T>
std::ostream& operator << (std::ostream& cout, const DataPoint<T>& point)
{
  if (point.data.size () == 0) {
    cout << "[ ] ";
    return cout;
  }

  int i = 0;
  cout << "[ ";
  for (i = 0; i < point.data.size ()-1; i++) {
    cout << point.data[i] << ",";
  }
  cout << point.data[i] << "] ";
  return cout;
}

template <class T>
struct PointSet
{
  std::vector <DataPoint <T> > points;

  std::size_t npoints;
  std::size_t dimensions;

  PointSet ()
  {
    npoints = 0;
    dimensions = 0;
  }

  std::size_t size () const
  {
    return npoints;
  }

  ~PointSet ()
  {
    points.clear ();
    npoints = 0;
    dimensions = 0;
  }
};

template <class T>
using PointSetSharedPtr = std::shared_ptr <PointSet <T> >;

