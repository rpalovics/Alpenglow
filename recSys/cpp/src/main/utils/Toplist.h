#ifndef TOPLIST
#define TOPLIST

#include<exception>
#include<queue>
#include<iostream>
#include"SortPairDescendingBySecond.h"

using namespace std;

template<typename T,bool (*F)(T,T)>
class Toplist{
  public:
    Toplist(int max_length){
      max_length_=max_length;
      heap_ = new priority_queue<T,vector<T>,comparator_type >(F);
    }
    Toplist(){
      heap_ = new priority_queue<T,vector<T>,comparator_type >(F);
    }
    void set_max_length(int max_length){
      max_length_=max_length;
    }
    const T get_min() const {
      if(size()==0) cerr << "Baj van" << endl;
      return heap_->top();
    }
    void insert(T t){
      if(heap_->size()<max_length_ or !F(heap_->top(),t)){
        heap_->push(t);
      } 
      if(heap_->size()>max_length_) delete_min();
    }
    int const size() const{
      return heap_->size();
    }
    void delete_min(){
      heap_->pop();
    }
    void clear(){
      while(heap_->size()>0){
        heap_->pop();
      }
    }
    typedef bool (*comparator_type)(T,T);
  private:
    priority_queue<T,vector<T>,comparator_type >* heap_;
    //std::function<bool(T,T)> f = [](T a, T b){return a.first > b.first;};
    int max_length_;
};
#endif
