#include "MaxHeap.h"
#include <exception>

/*
MaxHeap::MaxHeap(double* array, int length) : _vector(length){
  for(int i = 0; i < length; ++i){
    _vector[i] = array[i];
  }
  heapify();
}

MaxHeap::MaxHeap(const vector<double>& vector) : _vector(vector){
  heapify();
}
*/

void MaxHeap::heapify(){
  int length = _vector.size();
  for(int i=length-1; i>=0; --i){
    bubble_down(i);
  }
}

void MaxHeap::bubble_down(int index){
    int length = _vector.size();
    int left_child_index = 2*index + 1;
    int right_child_index = 2*index + 2;

    if(left_child_index >= length)
        return; //index is a leaf

    int MaxIndex = index;

    if(_vector[index]->distance < _vector[left_child_index]->distance){
        MaxIndex = left_child_index;
    }
    
    if((right_child_index < length) && (_vector[MaxIndex]->distance < _vector[right_child_index]->distance)){
        MaxIndex = right_child_index;
    }

    if(MaxIndex != index){
        //need to swap
        Tweet* temp = _vector[index];
        _vector[index] = _vector[MaxIndex];
        _vector[MaxIndex] = temp;
        bubble_down(MaxIndex);
    }
}

void MaxHeap::bubble_up(int index){
    if(index == 0)
        return;

    int parent_index = (index-1)/2;

    if(_vector[parent_index]->distance < _vector[index]->distance){
        Tweet* temp = _vector[parent_index];
        _vector[parent_index] = _vector[index];
        _vector[index] = temp;
        bubble_up(parent_index);
    }
}

void MaxHeap::insert(Tweet* tweet)
{
    int length = _vector.size();
    if(length < top_k or get_max()->distance > tweet->distance){
      _vector.push_back(tweet);
      bubble_up(length);
    }
    length = _vector.size();
    if(length > top_k) delete_max();
}

Tweet* MaxHeap::get_max(){
  //if(_vector.size()>0){
    return _vector[0];
  //} else {
  //  throw exception();
  //}
}
    
void MaxHeap::delete_max(){
    int length = _vector.size();

    if(length == 0){
        return;
    }
    
    _vector[0] = _vector[length-1];
    _vector.pop_back();

    bubble_down(0);
}
