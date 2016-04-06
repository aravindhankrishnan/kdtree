#pragma once
#include <vector>
#include <memory>

#include "pointset.h"

#define SPLIT_CRITERIA_ROUND_ROBIN    "roundrobin"
#define SPLIT_CRITERIA_MAX_VARIANCE   "maxvariance"

template <class T>
class SplitCriteria
{
  protected:

  public:

    static SplitCriteria <T>* getSplitCriteriaObject (const std::string& criteria);

    virtual int getSplitDimension (const PointSetSharedPtr <T>& point_set,
                                   const std::vector <int>& indices,
                                   const int start,
                                   const int end,
                                   const int level) = 0;
};

template <class T>
class RoundRobinSplitCriteria : public SplitCriteria <T>
{
  protected:

  public:

    RoundRobinSplitCriteria  ()
    {
    }

    int getSplitDimension (const PointSetSharedPtr <T>& point_set,
                           const std::vector <int>& indices,
                           const int start,
                           const int end,
                           const int level) override;

    ~RoundRobinSplitCriteria  ()
    {
    }

  private:
};
 
template <class T>
class MaximumVarianceSplitCriteria : public SplitCriteria <T>
{
  protected:

    T computeVarianceOnDimension (const PointSetSharedPtr <T>& point_set,
                                  const int dimension,
                                  const std::vector <int>& indices,
                                  const int start,
                                  const int end);

  public:

    MaximumVarianceSplitCriteria ()
    {
    }

    int getSplitDimension (const PointSetSharedPtr <T>& point_set,
                           const std::vector <int>& indices,
                           const int start,
                           const int end,
                           const int level) override;

    ~MaximumVarianceSplitCriteria ()
    {
    }

  private:
};

template <class T>
using SplitCriteriaSharedPtr = std::shared_ptr <SplitCriteria <T> >;
