#ifndef TOPLIST
#define TOPLIST

#include<exception>
#include<queue>
#include<iostream>
#include"SortPairDescendingBySecond.h"

using namespace std;

template<typename T,bool (*f)(T,T)>
class Toplist{
  public:
    Toplist(int max_length){
      max_length_=max_length;
      heap_ = new priority_queue<T,vector<T>,bool(*)(T,T) >(f);
    }
    Toplist(){
      heap_ = new priority_queue<T,vector<T>,bool(*)(T,T) >(f);
    }
    ~Toplist(){
      delete heap_;
    }
    void set_max_length(int max_length){
      max_length_=max_length;
    }
    const T get_min() const {
      if(size()==0) throw exception();
      return heap_->top();
    }
    void insert(T t){
      if(heap_->size()<max_length_ or !f(heap_->top(),t)){
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
  private:
    priority_queue<T,vector<T>,bool(*)(T,T) >* heap_;
    int max_length_ = -1;
};
#endif
