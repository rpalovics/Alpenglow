#include "MinHeap.h"
#include <exception>

/*
MinHeap::MinHeap(double* array, int length) : _vector(length){
  for(int i = 0; i < length; ++i){
    _vector[i] = array[i];
  }
  heapify();
}

MinHeap::MinHeap(const vector<double>& vector) : _vector(vector){
  heapify();
}
*/

void MinHeap::heapify(){
  int length = _vector.size();
  for(int i=length-1; i>=0; --i){
    bubble_down(i);
  }
}

void MinHeap::bubble_down(int index){
    int length = _vector.size();
    int left_child_index = 2*index + 1;
    int right_child_index = 2*index + 2;

    if(left_child_index >= length)
        return; //index is a leaf

    int min_index = index;

    if(_vector[index].score > _vector[left_child_index].score){
        min_index = left_child_index;
    }
    
    if((right_child_index < length) && (_vector[min_index].score > _vector[right_child_index].score)){
        min_index = right_child_index;
    }

    if(min_index != index){
        //need to swap
        RecDat temp = _vector[index];
        _vector[index] = _vector[min_index];
        _vector[min_index] = temp;
        bubble_down(min_index);
    }
}

void MinHeap::bubble_up(int index){
    if(index == 0)
        return;

    int parent_index = (index-1)/2;

    if(_vector[parent_index].score > _vector[index].score){
        RecDat temp = _vector[parent_index];
        _vector[parent_index] = _vector[index];
        _vector[index] = temp;
        bubble_up(parent_index);
    }
}

void MinHeap::insert(RecDat rec_dat)
{
    int length = _vector.size();
    if(length < top_k or get_min().score < rec_dat.score){
      _vector.push_back(rec_dat);
      bubble_up(length);
    }
    length = _vector.size();
    if(length > top_k) delete_min();
}

RecDat MinHeap::get_min(){
  if(_vector.size()>0){
    return _vector[0];
  } else {
    throw exception();
  }
}
    
void MinHeap::delete_min(){
    int length = _vector.size();

    if(length == 0){
        return;
    }
    
    _vector[0] = _vector[length-1];
    _vector.pop_back();

    bubble_down(0);
}
