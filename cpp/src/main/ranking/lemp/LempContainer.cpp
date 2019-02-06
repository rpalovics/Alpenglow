#include "LempContainer.h"

#include <stdexcept>
#include <tuple>
#include <iostream>

int LempContainer::size(){
  return item_buckets_.size();
}

multiset<LempBucket*>::iterator LempContainer::buckets_begin(){
  perform_scheduled_updates();
  return buckets_.begin();
}

multiset<LempBucket*>::iterator LempContainer::buckets_end(){
  return buckets_.end();
}

void LempContainer::rebuild_from_items(vector<tuple<int,double,vector<double>>> &items){
  for(auto b : buckets_){
    delete b;
  }
  buckets_.clear();
  item_buckets_.clear();

  sort(
    items.begin(),
    items.end(),
    [](const tuple<int,double,vector<double>> & t1, const tuple<int,double,vector<double>> & t2){ 
      return get<1>(t1) > get<1>(t2);
  });

  int step = bucket_size_/2;
  for(int i=0; i<(int)items.size(); i+=step){
    auto start = items.begin()+i;
    auto end = (i+step < (int)items.size()) ? start + step : items.end();

    LempBucket* new_bucket = new LempBucket();
    // the bucket goes at the end anyways, but we need the iterator
    // to set item_buckets_[id], so we might as well insert immediately
    auto bucket_it = buckets_.insert(buckets_.end(), new_bucket);
    for(auto j = start; j!=end; ++j){
      int id;
      double length;
      vector<double> direction;
      tie(id,length,direction) = *j;
      new_bucket->item_ids_.push_back(id);
      new_bucket->item_lengths_.push_back(length);
      new_bucket->item_directions_.push_back(direction);
      item_buckets_[id] = bucket_it;
    }
  }

  if(buckets_.size() == 0){
    LempBucket *first_bucket = new LempBucket();
    buckets_.insert(first_bucket);
  }
}

LempBucket* LempContainer::insert_item(int id){
  vector<double> item_factor = get_factor_item(id);
  multiset<LempBucket*>::iterator container_bucket_it = find_closest_bucket(Util::norm(&item_factor));
  LempBucket* container_bucket = *container_bucket_it;
  double container_bucket_max = container_bucket->get_bucket_max();
  container_bucket->insert_item(id, &item_factor);
  item_buckets_[id] = container_bucket_it;
  repair_bucket(container_bucket_it,container_bucket_max);

  return container_bucket;
}

void LempContainer::perform_scheduled_updates(){
  if(scheduled_updates_.size() > item_buckets_.size()/2){
    scheduled_updates_.clear();
    auto items = get_factor_items();
    rebuild_from_items(items);
  } else {
    decltype(scheduled_updates_) scheduled_updates_copy(scheduled_updates_);
    scheduled_updates_.clear();
    for(int i : scheduled_updates_copy){
      update_item(i);
    }
  }
}


void LempContainer::update_item(int id){
  remove_item(id);
  insert_item(id);
}


multiset<LempBucket*>::iterator LempContainer::find_closest_bucket(double length){
  LempBucket fake_bucket;
  fake_bucket.item_lengths_ = {length};
  auto bucket = buckets_.upper_bound(&fake_bucket);
  if(bucket != buckets_.begin()){
    bucket--;
  }
  return bucket;
}

LempBucket* LempContainer::remove_item(int id){
  auto it = item_buckets_.find(id);
  if(it != item_buckets_.end()){
    multiset<LempBucket*>::iterator container_bucket_it = it->second;
    LempBucket* container_bucket = *container_bucket_it;
    double container_bucket_max = container_bucket->get_bucket_max();

    container_bucket->erase_item(id);

    repair_bucket(container_bucket_it, container_bucket_max);

    return container_bucket;
  }
  return NULL;
}


void LempContainer::repair_bucket(multiset<LempBucket*>::iterator bucket, double previous_bucket_max){
  LempBucket* container_bucket = *bucket;
  if(container_bucket->size() <= bucket_size_ / 4) {
    eliminate_bucket(bucket);
  } else if(container_bucket->size() >= bucket_size_){
    split_bucket(bucket);
  }
}


void LempContainer::split_bucket(multiset<LempBucket*>::iterator bucket){
  LempBucket* container_bucket = *bucket;
  LempBucket* new_bucket = new LempBucket(container_bucket->split(container_bucket->size()/2));
  multiset<LempBucket*>::iterator hint(bucket);
  hint++;
  auto new_pos = buckets_.insert(hint, new_bucket);
  for(int i : new_bucket->item_ids_){
    item_buckets_[i] = new_pos;
  }
}


void LempContainer::eliminate_bucket(multiset<LempBucket*>::iterator bucket){
  if(buckets_.size() == 1){
    return;
  }
  LempBucket* container_bucket = *bucket;
  buckets_.erase(bucket);
  for(int i : container_bucket->item_ids_){
    insert_item(i);
  }
  delete container_bucket;
}


void LempContainer::schedule_update_item(int id){
  scheduled_updates_.insert(id);
}
