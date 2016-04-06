#include "kdtreeio.h"
#include "pointset.h"
#include "stringswitch.h"

#include <fstream>
#include <map>
#include <queue>
#include <typeinfo>
using namespace std;

namespace kdtree {

enum class DataType : std::uint8_t
{
  FLOAT,
  DOUBLE,
  INT,
  LONG
};

// typeid name maps for g++ 4.8.4
static std::map <string, string> typeid_data_type_map = { make_pair ("f", "float"),
                                            make_pair ("d", "double"),
                                            make_pair ("i", "int"),
                                            make_pair ("l", "long")
                                           };

// inverse map is constructed from the map defined above in saveKdTreeToDisk () 
// and loadKdTreeFromDisk () functions
static std::map <string, string> typeid_data_type_map_inverse;

struct ParentChildInfo
{
  int parent_id;
  char left_right;

  ParentChildInfo ()
  {
    parent_id = -1;
  }

  ParentChildInfo (const int _parent_id)
  {
    parent_id = _parent_id;
  }
};

static ParentChildInfo findParentId (const int child_node_id, const std::map <int, ParentChildInfo>& child_parent_map)
{
  // condition for root node
  if (child_node_id == 0) {
    return std::move (ParentChildInfo ());
  }

  std::map <int, ParentChildInfo>::const_iterator iter = child_parent_map.find (child_node_id);
  if (iter == child_parent_map.end ()) {
    cout << "Unable to find parent for child node "<< child_node_id << endl;
    return std::move (ParentChildInfo (-2));
  }

  return iter->second;
}

template <class T>
bool saveKdTreeToDisk (const string filename, const KdTree <T>& kdtree)
{

  if (typeid_data_type_map_inverse.size () == 0) {
    for (auto element : typeid_data_type_map) {
      typeid_data_type_map_inverse.insert (make_pair (element.second, element.first));
    }
  }

  string data_type = typeid (T).name ();
  cout << "data type " << data_type << endl;

  uint8_t c_data_type = 0;

  BEGIN_STRING_SWITCH_(data_type)

    CASE_(typeid_data_type_map_inverse["float"])
                cout << "Data type : float\n";
                c_data_type = static_cast <std::uint8_t> (DataType::FLOAT);

    CASE_(typeid_data_type_map_inverse["double"])
                cout << "Data type : double\n";
                c_data_type = static_cast <std::uint8_t> (DataType::DOUBLE);

    CASE_(typeid_data_type_map_inverse["int"])
                cout << "Data type : int\n";
                c_data_type = static_cast <std::uint8_t> (DataType::INT);
    
    CASE_(typeid_data_type_map_inverse["long"])
                cout << "Data type : long\n";
                c_data_type = static_cast <std::uint8_t> (DataType::LONG);

    DEFAULT_
                cout << "Data type : unknown\n";
                return false;

  END_STRING_SWITCH_

  cout << "Writing to file " << filename << endl;
  ofstream ofile (filename.c_str (), ios::binary);
  ofile.write (reinterpret_cast <char*> (&c_data_type), sizeof (c_data_type));

  PointSetSharedPtr <T> point_set = kdtree.getPointSet ();
  size_t n_dimensions = 0;
  if (point_set) {
    n_dimensions = point_set->dimensions;
  }
  ofile.write (reinterpret_cast <char*> (&n_dimensions), sizeof (n_dimensions));

  size_t n_points = 0;
  if (point_set) {
    n_points = point_set->size ();
  }
  ofile.write (reinterpret_cast <char*> (&n_points), sizeof (n_points));

  for (const DataPoint <T>& data_point : point_set->points) {
    for (const T& val : data_point.data) {
      ofile.write (reinterpret_cast <const char*> (&val), sizeof (val));
    }
  }

  const KdTreeNodeSharedPtr <T> root = kdtree.getRootNode ();
  std::queue <KdTreeNodeSharedPtr <T> > node_queue;
  node_queue.push (root);

  int node_cnt = 0;
  std::queue <int> node_id_queue;
  node_id_queue.push (node_cnt);

  std::map <int, ParentChildInfo> child_parent_map;

  while (not node_queue.empty ()) {
    const KdTreeNodeSharedPtr <T> node = node_queue.front ();
    KdTreeDiskNode <T> disk_node;

    int node_id = node_id_queue.front ();
    ParentChildInfo parent_child_info = findParentId (node_id, child_parent_map);
    int parent_id = parent_child_info.parent_id; 
    if (parent_id == -2) {
      cout << "\nError saving node to disk .. \n";
      return false; 
    }

    disk_node.node_id = node_id;
    disk_node.parent_id = parent_id;
    disk_node.left_right = parent_child_info.left_right;
    //cout << "Node " << disk_node.node_id << " parent " << disk_node.parent_id << " ";
    //cout << (char) std::toupper (disk_node.left_right) << "\n";

    disk_node.leaf_node = (node->leaf_node != NULL);
    if (!disk_node.leaf_node) {
      disk_node.sort_dimension = node->sort_dimension;
      disk_node.threshold = node->threshold;
    }
    else {
      disk_node.index = node->leaf_node->index;
      disk_node.threshold = -1;
    }

    //if (!disk_node.leaf_node) {
    //  node->printCondition ();
    //}
    //else {
    //  cout << kdtree.getPointSet ()->points[node->leaf_node->index];
    //  cout << "\t";
    //}

    ofile.write (reinterpret_cast <char*> (&disk_node), sizeof (disk_node));
    //cout << "Writing node " << disk_node << " : offset " << ofile.tellp () << " ";
    //cout << disk_node.node_id << " " << disk_node.parent_id << " leaf " << disk_node.leaf_node << endl;

    if (node->left) {
      node_queue.push (node->left);
      node_cnt++;
      node_id_queue.push (node_cnt);

      ParentChildInfo parent_child_info;
      parent_child_info.parent_id = node_id;
      parent_child_info.left_right = 'l';

      child_parent_map.insert (make_pair (node_cnt, parent_child_info));
    }
    if (node->right) {
      node_queue.push (node->right);
      node_cnt++;
      node_id_queue.push (node_cnt);

      ParentChildInfo parent_child_info;
      parent_child_info.parent_id = node_id;
      parent_child_info.left_right = 'r';

      child_parent_map.insert (make_pair (node_cnt, parent_child_info));
    }

    node_queue.pop ();
    node_id_queue.pop ();
  }

  cout << "Number of kdtree nodes written " << node_cnt << endl;

  ofile.close ();
  cout << "\n";

  return true;
}

template <class T>
bool loadKdTreeFromDisk (const string filename, KdTree <T>& kdtree)
{
  if (typeid_data_type_map_inverse.size () == 0) {
    for (auto element : typeid_data_type_map) {
      typeid_data_type_map_inverse.insert (make_pair (element.second, element.first));
    }
  }

  cout << "Loading " << filename << "..\n";
  ifstream ifile (filename.c_str (), ios::binary);
  DataType c_data_type;

  //ifile.read (static_cast <char*> (&c_data_type), sizeof (c_data_type));
  ifile.read (reinterpret_cast <char*> (&c_data_type), sizeof (c_data_type));

  switch (c_data_type) {
    case DataType::FLOAT :
                  cout << "Data type : float \n";
                  break;
    case DataType::DOUBLE :
                  cout << "Data type : double \n";
                  break;
    case DataType::INT :
                  cout << "Data type : int \n";
                  break;
    case DataType::LONG :
                  cout << "Data type : long \n";
                  break;
    default:
                  cout << "Datatype : unknown\n";
                  return false;
  }

  string data_type = typeid (T).name ();
  if (c_data_type == DataType::FLOAT and data_type.compare (typeid_data_type_map_inverse["float"])) {
    cout << "Data type mismatch..\n";
    cout << "KdTree object type (" << typeid_data_type_map[data_type] 
         << ") is different from the kdtree datatype (float) on the disk .\n";
    return false;
  }
  if (c_data_type == DataType::DOUBLE and data_type.compare (typeid_data_type_map_inverse["double"])) {
    cout << "Data type mismatch..\n";
    cout << "KdTree object type (" << typeid_data_type_map[data_type] 
         << ") is different from the kdtree datatype (double) on the disk  .\n";
    return false;
  }
  if (c_data_type == DataType::INT and data_type.compare (typeid_data_type_map_inverse["int"])) {
    cout << "Data type mismatch..\n";
    cout << "KdTree object type (" << typeid_data_type_map[data_type] 
         << ") is different from the kdtree datatype (int) on the disk .\n";
    return false;
  }
  if (c_data_type == DataType::LONG and data_type.compare (typeid_data_type_map_inverse["long"])) {
    cout << "Data type mismatch..\n";
    cout << "KdTree object type (" << typeid_data_type_map[data_type] 
         << ") is different from the kdtree datatype (long) on the disk .\n";
    return false;
  }

  size_t n_dimensions = 0;
  ifile.read (reinterpret_cast <char*> (&n_dimensions), sizeof (n_dimensions));

  cout << "Number of dimensions " << n_dimensions << endl;

  size_t n_points = 0;
  ifile.read (reinterpret_cast <char*> (&n_points), sizeof (n_points));
  cout << "Number of points " << n_points << endl;

  if (n_points == 0) {
    ifile.close ();
    return false;
  }

  PointSetSharedPtr <T> point_set;
  point_set.reset (new PointSet <T> ());

  for (int i = 0; i < n_points; i++) {
    DataPoint <T> data_point;
    for (int d = 0; d < n_dimensions; d++) {
      T val = 0.;
      ifile.read (reinterpret_cast <char*> (&val), sizeof (val));
      data_point.data.push_back (val);
    }
    point_set->points.push_back (data_point);
  }
  point_set->npoints = point_set->points.size ();
  point_set->dimensions = n_dimensions;

  kdtree.setPointSet (point_set);

  cout << "Loaded " << point_set->points.size () << " points ..\n";

  std::map <int, KdTreeNodeSharedPtr <T> > node_id_map;
  int node_cnt = -1;
  KdTreeDiskNode <T> disk_node;
  while (ifile.read (reinterpret_cast <char*> (&disk_node), sizeof (disk_node))) {
    //cout << "Reading node " << disk_node << " : Offset " << ifile.tellg () << " ";
    //cout << disk_node.node_id << " " << disk_node.parent_id << " leaf " << disk_node.leaf_node << endl;

    if (!disk_node.leaf_node) {
      KdTreeNodeSharedPtr <T> kdtree_node (new KdTreeNode <T> ());
      kdtree_node->threshold = disk_node.threshold;
      kdtree_node->sort_dimension = disk_node.sort_dimension;

      node_id_map.insert (make_pair (disk_node.node_id, kdtree_node));

      if (disk_node.parent_id == -1) {
        kdtree.setRootNode (kdtree_node);
        node_cnt++;
        continue;
      }

      typename std::map <int, KdTreeNodeSharedPtr <T> >::iterator iter = node_id_map.find (disk_node.parent_id);
      if (iter == node_id_map.end ()) {
        cout << "map search **ERROR**\n";
      }

      KdTreeNodeSharedPtr <T>& parent_kdtree_node = iter->second;
      if (disk_node.left_right == 'r') {
        parent_kdtree_node->right = kdtree_node;
      }
      else if (disk_node.left_right == 'l') {
        parent_kdtree_node->left = kdtree_node;
      }
      else {
        cout << "Parent child info **** WRONG ****\n";
      }
    }
    else if (disk_node.leaf_node) {

      KdTreeNodeSharedPtr <T> kdtree_node (new KdTreeNode <T> ());

      KdTreeLeafNodeSharedPtr kdtree_leaf_node (new KdTreeLeafNode ());
      kdtree_leaf_node->index = disk_node.index;

      typename std::map <int, KdTreeNodeSharedPtr <T> >::iterator iter = node_id_map.find (disk_node.parent_id);
      if (iter == node_id_map.end ()) {
        cout << "map search **ERROR**\n";
      }
      KdTreeNodeSharedPtr <T>& parent_kdtree_node = iter->second;

      if (disk_node.left_right == 'r') {
        parent_kdtree_node->right = kdtree_node;
      }
      else if (disk_node.left_right == 'l') {
        parent_kdtree_node->left = kdtree_node;
      }

      kdtree_node->leaf_node = kdtree_leaf_node;
    }

    node_cnt++;
  }
  cout << "Number of kdtree nodes read = " << node_cnt << endl;

  ifile.close ();
  return true;
}

template 
bool saveKdTreeToDisk (const string filename, const KdTree <double>& kdtree);

template 
bool saveKdTreeToDisk (const string filename, const KdTree <float>& kdtree);

template 
bool saveKdTreeToDisk (const string filename, const KdTree <int>& kdtree);

template 
bool saveKdTreeToDisk (const string filename, const KdTree <long>& kdtree);


template 
bool loadKdTreeFromDisk (const string filename, KdTree <double>& kdtree);

template 
bool loadKdTreeFromDisk (const string filename, KdTree <float>& kdtree);

template 
bool loadKdTreeFromDisk (const string filename, KdTree <int>& kdtree);

template 
bool loadKdTreeFromDisk (const string filename, KdTree <long>& kdtree);


}
