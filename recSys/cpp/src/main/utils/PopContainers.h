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
    PopContainer(){};
    ~PopContainer(){};
    void increase(int item);
    void reduce(int item);
    void resize(int item);
    int get(int item);
    void clear(){popularities.clear();};
  private:
    vector <int> popularities;
};

class TopPopContainer{
  public:
    TopPopContainer(){idxs.clear();};
    ~TopPopContainer(){};
    void increase( int item);
    void reduce(int item);
    map <int,double> * getRecommendation(int num);
    pair <int,double> get(int idx);
    pair <int,double> getNormed(int idx);
    int getItem(int idx){return names[idx];};
    int getIndex(int item){return idxs[item];}
    int size(){return names.size();};
    void setThreshold(int num){threshold=num;};
    bool hasChanged(){return changed;}
  private:
    void swapUp(int item);
    void swapDown(int item);
    void swap(int idx1, int idx2, vector <int> * vec);
    void squeeze(int item);
    vector <int> idxs;
    vector <int> names,popularities;
    bool changed; 
    int threshold;
};

#endif
