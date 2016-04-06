#pragma once

#include <memory>
#include <iostream>
#include <stack>
#include <limits>

#include "pointset.h"
#include "split_criteria.h"

namespace kdtree {

struct KdTreeLeafNode
{
  int index = -1;

  KdTreeLeafNode ()
  {
  }

  KdTreeLeafNode (const int _index)
  {
    index = _index;
  }

  ~KdTreeLeafNode ()
  {
  }

};

// Creating typedef for the shared pointer
typedef std::shared_ptr <KdTreeLeafNode> KdTreeLeafNodeSharedPtr;

// forward declaration 
template <class T>
struct KdTreeNode;

// Creating type alias for the pointer
template <class T>
using KdTreeNodePtr = KdTreeNode <T>*;

// Creating type alias for the shared pointer
template <class T>
using KdTreeNodeSharedPtr = std::shared_ptr <KdTreeNode <T> >;

template <class T>
struct KdTreeNode
{
  KdTreeNodeSharedPtr <T> left = nullptr;
  KdTreeNodeSharedPtr <T> right = nullptr;

  T threshold = -1;
  int sort_dimension = -1;

  KdTreeLeafNodeSharedPtr leaf_node = nullptr;

  KdTreeNode ()
  {
  }

  KdTreeNode (const T _threshold, const int _sort_dimension)
  {
    threshold = _threshold;
    sort_dimension = _sort_dimension;
  }

  void printCondition () const
  {
    if (leaf_node) {
      std::cout << "No condition .. leaf node ..\n";
      return;
    }
    std::cout << "condition [" << sort_dimension << " , " << threshold << "]\n";
  }

  ~KdTreeNode ()
  {
  }

  private:
};

//forward declaration
template <class T>
struct KdTreeSearchDataStructure;

template <class T>
class KdTree
{
  protected:

    PointSetSharedPtr <T> point_set;
    std::vector <int> indices;
    std::string split_criteria_str;

    SplitCriteriaSharedPtr <T> split_criteria;
    KdTreeNodeSharedPtr <T> root = nullptr;

    KdTreeNodeSharedPtr <T> recursiveBuild (std::vector <int>& indices, const int level, 
                                             const int start, 
                                             const int end);

    void sortByDimension (std::vector <int>& indices, const int dimension, 
                                             const int start, 
                                             const int end,
                                             const int middle);
         

    void recursiveDestroy (KdTreeNodeSharedPtr <T>& node);

    void recursivePrint (const KdTreeNodeSharedPtr <T>& node, const int level, const char* child);

    void nearestNeighbourHelper (const KdTreeNodeSharedPtr <T> node, 
                                                      const DataPoint <T>& point,
                                                      KdTreeSearchDataStructure <T>& nearest_node);

    typedef std::stack <KdTreeNodeSharedPtr <T> > KdTreeNodeStack;

    void findPathToLeafNode (KdTreeNodeStack& stack, KdTreeNodeSharedPtr <T> node,
                             const DataPoint <T>& point);

    bool hypersphereCondition (const DataPoint <T>& point, 
                                      const KdTreeNodeSharedPtr <T>& node, 
                                      const double best_distance);

  public:

    KdTree ()
    {
      split_criteria_str = SPLIT_CRITERIA_MAX_VARIANCE;
    }

    KdTree (const PointSetSharedPtr<T>& _point_set)
    {
      KdTree ();
      point_set = _point_set;
      for (int i = 0; i < point_set->size (); i++) {
        indices.push_back (i);
      }
    }

    void buildTree ();

    void destroyTree ();

    void setPointSet (const PointSetSharedPtr<T>& _point_set) 
    { 
      point_set = _point_set;
      for (int i = 0; i < point_set->size (); i++) {
        indices.push_back (i);
      }
    }

    void setSplitCriteria (const std::string _split_criteria_str)
    {
      split_criteria_str = _split_criteria_str; 
    }

    std::string getSplitCriteria () const
    {
      return split_criteria_str;
    }

    const PointSetSharedPtr<T> getPointSet () const
    { 
      return point_set; 
    }

    const KdTreeNodeSharedPtr <T> getRootNode () const
    {
      return root;
    }

    void setRootNode (const KdTreeNodeSharedPtr <T>& node)
    {
      root = node;
    }

    void nearestNeighbour (const DataPoint <T>& point, 
                            int& nearest_point_index,
                            DataPoint<T>& nearest_point,
                            double& nearest_point_distance);

    void printTree ();

    size_t getTotalPoints () const
    {
      if (point_set) {
        return point_set->size ();
      }
      return 0;
    }

    ~KdTree ()
    {
    }

  private:

};

template <class T>
struct KdTreeSearchDataStructure
{
  KdTreeNodeSharedPtr <T> node;
  double distance;

  KdTreeSearchDataStructure ()
  {
    node = nullptr;
    distance = 0.;
  }

  ~KdTreeSearchDataStructure ()
  {
  }
};

}
