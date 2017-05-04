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
    bubbleDown(i);
  }
}

void MaxHeap::bubbleDown(int index){
    int length = _vector.size();
    int leftChildIndex = 2*index + 1;
    int rightChildIndex = 2*index + 2;

    if(leftChildIndex >= length)
        return; //index is a leaf

    int MaxIndex = index;

    if(_vector[index]->distance < _vector[leftChildIndex]->distance){
        MaxIndex = leftChildIndex;
    }
    
    if((rightChildIndex < length) && (_vector[MaxIndex]->distance < _vector[rightChildIndex]->distance)){
        MaxIndex = rightChildIndex;
    }

    if(MaxIndex != index){
        //need to swap
        Tweet* temp = _vector[index];
        _vector[index] = _vector[MaxIndex];
        _vector[MaxIndex] = temp;
        bubbleDown(MaxIndex);
    }
}

void MaxHeap::bubbleUp(int index){
    if(index == 0)
        return;

    int parentIndex = (index-1)/2;

    if(_vector[parentIndex]->distance < _vector[index]->distance){
        Tweet* temp = _vector[parentIndex];
        _vector[parentIndex] = _vector[index];
        _vector[index] = temp;
        bubbleUp(parentIndex);
    }
}

void MaxHeap::insert(Tweet* tweet)
{
    int length = _vector.size();
    if(length < topK or getMax()->distance > tweet->distance){
      _vector.push_back(tweet);
      bubbleUp(length);
    }
    length = _vector.size();
    if(length > topK) deleteMax();
}

Tweet* MaxHeap::getMax(){
  //if(_vector.size()>0){
    return _vector[0];
  //} else {
  //  throw exception();
  //}
}
    
void MaxHeap::deleteMax(){
    int length = _vector.size();

    if(length == 0){
        return;
    }
    
    _vector[0] = _vector[length-1];
    _vector.pop_back();

    bubbleDown(0);
}
