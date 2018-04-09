#ifndef MINHEAP
#define MINHEAP
#include <iostream>
#include <vector>

#include "../recommender_data/RecommenderData.h"


using namespace std;

//forras: http://www.codeproject.com/Tips/816934/Min-Binary-Heap-Implementation-in-Cplusplus

class MinHeap{
public:
  //MinHeap(double* array, int length);
  //MinHeap(const vector<double>& vector);
  MinHeap(int _top_k){top_k = _top_k;}
  MinHeap(){}

  void set_top_k(int _top_k){ top_k = _top_k; }
  void insert(RecDat rec_dat);
  RecDat get_min();
  void delete_min();
  int size(){ return _vector.size(); }
  void show(){for(uint ii=0;ii<_vector.size(); ii++){cerr << _vector[ii].score << " " ;}cerr << endl;};
  void clear(){ _vector.clear(); }
  bool self_test(){
    bool ok = true;
    if(top_k<1) ok=false;
    return ok;
  }
private:
  vector<RecDat> _vector;
  void bubble_down(int index);
  void bubble_up(int index);
  void heapify();
  int top_k;
};

#endif
