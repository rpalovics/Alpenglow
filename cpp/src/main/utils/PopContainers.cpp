#include "PopContainers.h"

void PopContainer::increase(int item){
  resize(item);
  popularities_[item]++;
}

void PopContainer::reduce(int item){
  popularities_[item]--;
}

void PopContainer::resize(int item){
  if((int)popularities_.size()<item+1) popularities_.resize(item+1);
}

int PopContainer::get(int item) const { 
  if((int)popularities_.size()<item+1) return 0;
  else return popularities_[item];
}

// TopPopContainer

void TopPopContainer::increase(int item) { 
  if ((int)idxs_.size()<item+1) {
    idxs_.resize(item+1,-1);
  }
  int idx = -1;
  int popularity = -1;
  if (-1 == idxs_[item]) {
    popularity = 1;
    top_list_.push_back(item);
    popularities_.push_back(popularity);
    idx = top_list_.size()-1;
    idxs_[item] = idx;
  } else {
    int original_idx = idxs_[item];
    int original_popularity = popularities_[original_idx];
    idx = popularity_boundaries_[original_popularity];
    swap_positions(original_idx,idx); //move item to the head of items having the same popularity
    popularities_[idx]++;
    popularity = original_popularity+1;
    if((int)popularities_.size()<=idx+1 or
        popularities_[idx+1]!=original_popularity){ //was the only item having original_popularity
      popularity_boundaries_.erase(original_popularity);
    } else {
      popularity_boundaries_[original_popularity]++;
    }
  } 
  if (popularity_boundaries_.find(popularity)==popularity_boundaries_.end()){
    popularity_boundaries_[popularity]=idx;
  }
}
void TopPopContainer::create(int item){
  //create with 0 pop if doesnt exist
  if ((int)idxs_.size()<item+1) {
    idxs_.resize(item+1,-1);
  }
  if (-1 == idxs_[item]) {
    int popularity = 0;
    top_list_.push_back(item);
    popularities_.push_back(popularity);
    int idx = top_list_.size()-1;
    idxs_[item] = idx;
    if (popularity_boundaries_.find(popularity)==popularity_boundaries_.end()){ //no items yet with 0 popularity
      popularity_boundaries_[popularity]=idx;
    }
  } 
}

pair <int,double> TopPopContainer::get(int idx) const {
  pair <int,double> p;
  if(idx<(int)top_list_.size()){
    p.first=top_list_[idx];
    p.second=popularities_[idx];
  }
  return p;
}

void TopPopContainer::swap_positions(int idx1, int idx2){
  ///swaps two items in the toplist that have identical popularity
  int item1 = top_list_[idx1];
  int item2 = top_list_[idx2];
  top_list_[idx1]=item2;
  top_list_[idx2]=item1;
  idxs_[item1]=idx2;
  idxs_[item2]=idx1;
}
