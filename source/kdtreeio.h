#pragma once

#include "kdtree.h"

namespace kdtree {

template <class T>
struct KdTreeDiskNode
{
  int node_id;
  int parent_id;
  char left_right;
  bool leaf_node;

  union {
    int sort_dimension;
    int index;
  };

  KdTreeDiskNode ()
  {
    parent_id = -1;
    node_id = -1;
    left_right = '0';
    leaf_node = false;

    sort_dimension = -1;
  }


  T threshold;

};

template <class T>
std::ostream& operator << (std::ostream& cout, const KdTreeDiskNode <T>& disk_node)
{
  cout << "[" << disk_node.parent_id << "," << disk_node.left_right << "," 
       << disk_node.leaf_node << ","
       << disk_node.threshold << "]";
  return cout;
}

template <class T>
bool saveKdTreeToDisk (const std::string filename, const KdTree <T>& kdtree);

template <class T>
bool loadKdTreeFromDisk (const std::string filename, KdTree <T>& kdtree);

}
