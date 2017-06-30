//#ifndef MAXHEAP
//#define MAXHEAP
//#include <iostream>
//#include <vector>
//
//#include "../recommender_data/RecommenderData.h"
//
//
//using namespace std;
//
////forras: http://www.codeproject.com/Tips/816934/Max-Binary-Heap-Implementation-in-Cplusplus
//
//class MaxHeap{
//public:
//  //MaxHeap(double* array, int length);
//  //MaxHeap(const vector<double>& vector);
//  MaxHeap(int _top_k){top_k = _top_k;};
//
//  void insert(Tweet* tweet);
//  Tweet* get_max();
//  void delete_max();
//  int size(){ return _vector.size(); }
//  void show(){for(uint ii=0;ii<_vector.size(); ii++){cerr << _vector[ii]->distance << " " ;}cerr << endl;};
//  void clear(){ _vector.clear(); }
//private:
//  vector<Tweet*> _vector;
//  void bubble_down(int index);
//  void bubble_up(int index);
//  void heapify();
//  int top_k;
//};
//
//#endif
