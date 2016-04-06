#include "kdtree.h"
#include "split_criteria.h"
#include <iostream>
#include <algorithm>
using namespace std;

namespace kdtree {

template <class T>
void KdTree <T>::buildTree ()
{
  int level = 0,
      start = 0,
      end = indices.size ()-1;

  //split_criteria_str = SPLIT_CRITERIA_ROUND_ROBIN;
  cout << "** USING SPLIT CRITERIA " << split_criteria_str << " ****\n";
  split_criteria.reset (SplitCriteria <T>::getSplitCriteriaObject (split_criteria_str));
  root = recursiveBuild (indices, level, start, end);
}

template <class T>
void KdTree <T>::destroyTree ()
{
  recursiveDestroy (root);
  root = nullptr;
}

template <class T>
void KdTree <T>::recursiveDestroy (KdTreeNodeSharedPtr <T>& node)
{
  if (node->leaf_node) {
    node->leaf_node.reset ();
    node->leaf_node = nullptr;
    return;
  }

  recursiveDestroy (node->left);
  recursiveDestroy (node->right);
  node.reset ();
  node = nullptr;
}

template <class T>
KdTreeNodeSharedPtr <T> KdTree <T>::recursiveBuild (vector <int>& indices, const int level, 
                                                    const int start, const int end)
{
  if (start > end) {
    return nullptr;
  }

  if (start == end) {
    KdTreeNodeSharedPtr <T> node (new KdTreeNode <T> ());
    node->leaf_node.reset (new KdTreeLeafNode (indices[end]));
    return node;
  }

  int sort_dimension = split_criteria->getSplitDimension (point_set, indices, start, end, level);

  //cout << "level " << level << " sort dimension " << sort_dimension << endl;

  int median = (start + end) / 2;

  // Handling this case is important. Otherwise it recurses forever
  if ((start == median)) {
    median++;
  }

  sortByDimension (indices, sort_dimension, start, end+1, median);

  T threshold = point_set->points[indices[median]].data[sort_dimension];

  KdTreeNodeSharedPtr <T> node (new KdTreeNode <T> (threshold, sort_dimension));

  node->left = recursiveBuild (indices, level+1, start, median-1);
  node->right = recursiveBuild (indices, level+1, median, end);

  return node;
}

template <class T>
void KdTree <T>::nearestNeighbour (const DataPoint <T>& point, 
                                   int& nearest_point_index,
                                   DataPoint<T>& nearest_point, 
                                   double& nearest_point_distance)
{
  if (!root) {
    cout << "Tree is empty ..\n";
    return;
  }

  //cout << "Point " << point << "\t";
  KdTreeSearchDataStructure <T> nearest_node;
  nearest_node.distance = numeric_limits <double>::max ();

  nearestNeighbourHelper (root, point, nearest_node);

  nearest_point_index = nearest_node.node->leaf_node->index;
  nearest_point = point_set->points[nearest_node.node->leaf_node->index];
  nearest_point_distance = nearest_node.distance;
  //cout << "Nearest Point " << nearest_point << endl;
}

template <class T>
void KdTree <T>::nearestNeighbourHelper (const KdTreeNodeSharedPtr <T> node, 
                                                                 const DataPoint <T>& point,
                                                                 KdTreeSearchDataStructure <T>& nearest_node)
{
  KdTreeNodeStack stack;
  findPathToLeafNode (stack, node, point);
  const DataPoint <T> leaf_point = point_set->points[stack.top ()->leaf_node->index];

  KdTreeSearchDataStructure <T> current_guess;
  current_guess.node = stack.top ();
  current_guess.distance = DataPoint <T>::distance (point, leaf_point);

  if (double (current_guess.distance) < double (nearest_node.distance) ){
    nearest_node = current_guess;
  }

  stack.pop ();

  KdTreeNodeSharedPtr <T> prev_nnode = current_guess.node;
  while (!stack.empty ()) {
    const KdTreeNodeSharedPtr <T> nnode = stack.top ();
    stack.pop ();

    if (hypersphereCondition (point, nnode, nearest_node.distance)) {

      if (nnode->left == prev_nnode) {
        nearestNeighbourHelper (nnode->right, point, nearest_node);
      }
      else if (nnode->right == prev_nnode) {
        nearestNeighbourHelper (nnode->left, point, nearest_node);
      }
    }
    prev_nnode = nnode;
  }
}

template <class T>
bool KdTree <T>::hypersphereCondition (const DataPoint <T>& point, 
                                      const KdTreeNodeSharedPtr <T>& node, 
                                      const double best_distance)
{
  double distance = fabs (node->threshold - point.data[node->sort_dimension]);

  if (distance < best_distance) {
    return true;
  }
  return false;
}

template <class T>
void KdTree <T>::findPathToLeafNode (KdTreeNodeStack& stack, KdTreeNodeSharedPtr <T> node,
                                    const DataPoint <T>& point)
{
  while (true) {
    stack.push (node);
    if (node->leaf_node) {
      break;
    }

    int sort_dimension = node->sort_dimension;
    if (point.data[sort_dimension] < node->threshold) {
      node = node->left;
      continue;
    }
    node = node->right;
  }
}

template <class T>
void KdTree <T>::printTree ()
{
  int level = 0;
  recursivePrint (root, level, "root");
}

template <class T>
void KdTree <T>::recursivePrint (const KdTreeNodeSharedPtr <T>& node, const int level,
                                const char* child)
{
  if (!node) {
    return;
  }

  cout << child << " child ";
  cout << "level " << level << " ";
  cout << "condition [" << node->sort_dimension << " " << node->threshold << "]\n";
  if (node->leaf_node) {
    const DataPoint <T>& point = point_set->points[node->leaf_node->index];

    for (auto& i : point.data) {
      cout << i << " ";
    }
    cout << endl;
  }

  recursivePrint (node->left, level+1, "left");
  recursivePrint (node->right, level+1, "right");
}

template <class T>
void KdTree <T>::sortByDimension (std::vector <int>& indices, const int dimension, 
                                             const int start, 
                                             const int end,
                                             const int middle)
{
  auto sortFunctor = [&] (const int& i1, const int& i2) { 
                           return (point_set->points[i1].data[dimension] < 
                                   point_set->points[i2].data[dimension]); 
                         };

  std::nth_element (indices.begin () + start, indices.begin () + middle, 
                    indices.begin () + end, sortFunctor);
}

//
// template instantiations for various data types
//
template class KdTree <double>;
template class KdTree <float>;
template class KdTree <int>;
template class KdTree <long>;

template struct KdTreeSearchDataStructure <double>;
template struct KdTreeSearchDataStructure <float>;
template struct KdTreeSearchDataStructure <int>;
template struct KdTreeSearchDataStructure <long>;

}
