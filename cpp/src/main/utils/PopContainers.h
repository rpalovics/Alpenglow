#ifndef POP_H
#define POP_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <map>
#include <utility>

using namespace std;

//SIP_AUTOCONVERT
class PopContainer{
/**
    Container for storing the popularity of items.
*/
  public:
    void increase(int item);
/**
      increase(int item)
      Increases the popularity of the item.

      Parameters
      ----------
      item : int
          The item.
*/
    void reduce(int item);
/**
      reduce(int item)
      Reduces the popularity of the item.

      Parameters
      ----------
      item : int
          The item.
*/
    int get(int item);
/**
      get(int item)
      Returns the popularity value of the item.

      Parameters
      ----------
      item : int
          The item.

      Returns
      -------
      int
          The popularity value of the item.
*/
    void clear(){popularities_.clear();}
/**
      Clears the container. After this operations, the popularity of all the items is 0.
*/
  private:
    void resize(int item);
    vector<int> popularities_;
};

class TopPopContainer{
/**
    Helper class for storing the items sorted by popularity.

    Sample code

    .. code-block:: python
      :linenos:
      
      x = rs.TopPopContainer()
      x.increase(1)
      x.increase(1)
      x.increase(3)
      x.increase(4)
      x.increase(1)

      print("The most popular item is")
      print(x.get_item(0)) #returns 1
      print("The second most popular item is")
      print(x.get_item(1)) #returns 3 or 4
//      print("The popularity of the second most popular item is")
//      print(x.get(1).second) #returns 1, the item might be 3 or 4
*/
  public:
    void increase(int item);
/**
      increase(int item)
      Increases the popularity of the item.
*/
    pair<int,double> get(int idx);
///**
//      get(int index)
//      Returns the index'th item from the popularity toplist and its popularity.
//
//      Parameters
//      ----------
//      index : int
//          Index of the item in the popularity toplist. The index of the most popular item is 0.
//
//      Returns
//      -------
//      pair<int,double>
//          The item and its popularity value.
//*/
    int get_item(int idx){return top_list_[idx];} 
/**
      get_item(int index)
      Returns the index'th item from the popularity toplist.

      Parameters
      ----------
      index : int
          Index of the item in the popularity toplist. The index of the most popular item is 0.

      Returns
      -------
      int
          The appropriate item from the toplist.
*/
    int size(){return top_list_.size();} 
/**
      Returns the length of the complete toplist, i.e. the number of items having at least a popularity of 1.


      Returns
      -------
      int
          The length of the complete toplist.
*/
  private:
    void swap_positions(int idx1, int idx2);
    vector<int> idxs_;
    vector<int> top_list_,popularities_;
    map<int,int> popularity_boundaries_;
};

#endif
