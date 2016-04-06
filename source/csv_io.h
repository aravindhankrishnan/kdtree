#pragma once

#include "pointset.h"

#include <iostream>

template <class T>
bool readCSVFile (const std::string filename, PointSet <T> & point_set);

template <class T>
bool writeCSVFile (const std::string filename, const PointSet <T>& point_set);

