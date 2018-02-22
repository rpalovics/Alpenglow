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

class PopContainer{
  public:
    void increase(int item);
    void reduce(int item);
    void resize(int item);
    int get(int item);
    void clear(){popularities.clear();};
  private:
    vector<int> popularities;
};

class TopPopContainer{
  public:
    TopPopContainer(){idxs.clear();};
    ~TopPopContainer(){};
    void increase( int item);
    void reduce(int item);
    map <int,double> * get_recommendation(int num);
    pair <int,double> get(int idx);
    pair <int,double> get_normed(int idx);
    int get_item(int idx){return names[idx];};
    int get_index(int item){return idxs[item];}
    int size(){return names.size();};
    void set_threshold(int num){threshold=num;};
    bool has_changed(){return changed;}
  private:
    void swap_up(int item);
    void swap_down(int item);
    void swap(int idx1, int idx2, vector <int> * vec);
    void squeeze(int item);
    vector <int> idxs;
    vector <int> names,popularities;
    bool changed; 
    int threshold;
};

#endif
