#include "split_criteria.h"
#include "stringswitch.h"

#include <algorithm>
using namespace std;

template <class T>
SplitCriteria <T>* SplitCriteria <T>::getSplitCriteriaObject (const string& criteria)
{
  string str = criteria;

  BEGIN_STRING_SWITCH_(str)

    CASE_(SPLIT_CRITERIA_ROUND_ROBIN)
      return new RoundRobinSplitCriteria <T> ();

    CASE_(SPLIT_CRITERIA_MAX_VARIANCE)
      return new MaximumVarianceSplitCriteria <T> ();

    DEFAULT_
      cout << "Split critera  ** " << criteria << " ** not recongnizable ..\n";
      cout << "Falling back to " << SPLIT_CRITERIA_MAX_VARIANCE << " criteria\n";
      return new MaximumVarianceSplitCriteria <T> ();

  END_STRING_SWITCH_
}

template <class T>
int RoundRobinSplitCriteria <T>::getSplitDimension (const PointSetSharedPtr <T>& point_set,
                                                const std::vector <int>& indices,
                                                const int start,
                                                const int end,
                                                const int level)
{
  int sort_dimension = level % point_set->dimensions;
  return sort_dimension;
}

template <class T>
T MaximumVarianceSplitCriteria <T>::computeVarianceOnDimension (const PointSetSharedPtr <T>& point_set,
                                          const int dimension,
                                          const vector <int>& indices,
                                          const int start,
                                          const int end)
{
  T minval = std::numeric_limits <T>::max ();
  T maxval = std::numeric_limits <T>::min ();

  for (int i = start; i <= end; i++) {
    T val = point_set->points[i].data[dimension];
    if (val < minval) {
      minval = val;
    }
    if (val > maxval) {
      maxval = val;
    }
  }

  return (maxval - minval);
}

template <class T>
int MaximumVarianceSplitCriteria <T>::getSplitDimension (const PointSetSharedPtr <T>& point_set,
                                                const std::vector <int>& indices,
                                                const int start,
                                                const int end,
                                                const int level)
{
  int sort_dimension = 0;

  vector <T> variance;
  variance.resize (point_set->dimensions);
  fill (variance.begin (), variance.end (), 0.);

  for (int i = 0; i < variance.size (); i++) {
    variance[i] = computeVarianceOnDimension (point_set, i, indices, start, end);
  }

  sort_dimension = std::max_element (variance.begin (), variance.end ()) - variance.begin ();


  return sort_dimension;
}

template class SplitCriteria <int>;
template class SplitCriteria <long>;
template class SplitCriteria <double>;
template class SplitCriteria <float>;

template class RoundRobinSplitCriteria <int>;
template class RoundRobinSplitCriteria <long>;
template class RoundRobinSplitCriteria <double>;
template class RoundRobinSplitCriteria <float>;

template class MaximumVarianceSplitCriteria <int>;
template class MaximumVarianceSplitCriteria <long>;
template class MaximumVarianceSplitCriteria <double>;
template class MaximumVarianceSplitCriteria <float>;
