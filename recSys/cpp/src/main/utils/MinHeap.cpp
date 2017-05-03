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
    bubbleDown(i);
  }
}

void MinHeap::bubbleDown(int index){
    int length = _vector.size();
    int leftChildIndex = 2*index + 1;
    int rightChildIndex = 2*index + 2;

    if(leftChildIndex >= length)
        return; //index is a leaf

    int minIndex = index;

    if(_vector[index].score > _vector[leftChildIndex].score){
        minIndex = leftChildIndex;
    }
    
    if((rightChildIndex < length) && (_vector[minIndex].score > _vector[rightChildIndex].score)){
        minIndex = rightChildIndex;
    }

    if(minIndex != index){
        //need to swap
        RecDat temp = _vector[index];
        _vector[index] = _vector[minIndex];
        _vector[minIndex] = temp;
        bubbleDown(minIndex);
    }
}

void MinHeap::bubbleUp(int index){
    if(index == 0)
        return;

    int parentIndex = (index-1)/2;

    if(_vector[parentIndex].score > _vector[index].score){
        RecDat temp = _vector[parentIndex];
        _vector[parentIndex] = _vector[index];
        _vector[index] = temp;
        bubbleUp(parentIndex);
    }
}

void MinHeap::insert(RecDat recDat)
{
    int length = _vector.size();
    if(length < topK or getMin().score < recDat.score){
      _vector.push_back(recDat);
      bubbleUp(length);
    }
    length = _vector.size();
    if(length > topK) deleteMin();
}

RecDat MinHeap::getMin(){
  //if(_vector.size()>0){
    return _vector[0];
  //} else {
  //  throw exception();
  //}
}
    
void MinHeap::deleteMin(){
    int length = _vector.size();

    if(length == 0){
        return;
    }
    
    _vector[0] = _vector[length-1];
    _vector.pop_back();

    bubbleDown(0);
}
