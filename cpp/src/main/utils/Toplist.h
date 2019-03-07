#ifndef TOPLIST_H
#define TOPLIST_H

#include<stdexcept>
#include<queue>
#include<iostream>

using namespace std;

// Helper class for generating a top list of max_length.
// The second template parameter should be true exactly if the first variable is
// strictly larger than the second.

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
      if(size()==0) throw runtime_error("Accessing head of empty toplist.");
      return heap_->top();
    }
    void insert(T t){
      if((int)heap_->size()<max_length_ or f(t,heap_->top())){
        heap_->push(t);
      } 
      if((int)heap_->size()>max_length_) delete_min();
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
    bool self_test(){
      bool ok = true;
      if (heap_ == NULL) ok = false;
      if (max_length_ == 0) {
        ok = false;
        cerr << "Toplist::max_length_ is not set." << endl;
      }
      return ok;
    }
  private:
    priority_queue<T,vector<T>,bool(*)(T,T) >* heap_ = NULL;
    int max_length_ = -1;
};
#endif /* TOPLIST_H */
