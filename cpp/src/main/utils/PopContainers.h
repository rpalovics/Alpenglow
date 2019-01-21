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
  public:
    void increase(int item);
    void reduce(int item);
    int get(int item);
    void clear(){popularities_.clear();}
  private:
    void resize(int item);
    vector<int> popularities_;
};

class TopPopContainer{
  public:
    void increase(int item); ///increase popularity of item
    pair<int,double> get(int idx); ///get the idxth item in the toplist and it's popularity
    int get_item(int idx){return top_list_[idx];} ///get the idxth item in the toplist
    int size(){return top_list_.size();} ///get the length of the toplist = the number of the items
  private:
    void swap_positions(int idx1, int idx2);
    vector<int> idxs_;
    vector<int> top_list_,popularities_;
    map<int,int> popularity_boundaries_;
};

#endif
