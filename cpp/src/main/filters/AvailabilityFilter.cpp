#include "AvailabilityFilter.h"

bool AvailabilityFilter::active(RecDat* r){
  auto available = available_items_set_.find(r->item);
  return available != available_items_set_.end();
}

vector<int> AvailabilityFilter::get_white_list(int user){
  return available_items_;
}

void AvailabilityFilter::update(RecDat* rec_dat){
  double time = rec_dat->time;
  while(availability_ends_.size() != 0 && get<0>(availability_ends_.top()) <= time){
    auto item = availability_ends_.top();
    availability_ends_.pop();
    available_items_set_.erase(get<1>(item));
  }
  while(availabilites_.size() != 0 && get<0>(availabilites_.top()) <= time){
    auto item = availabilites_.top();
    availabilites_.pop();
    available_items_set_.insert(get<1>(item));
    availability_ends_.push(make_pair(get<0>(item)+get<2>(item), get<1>(item)));
  }
  available_items_ = vector<int>(available_items_set_.begin(), available_items_set_.end());
}

void AvailabilityFilter::add_availability(double time, int id, int duration){
  availabilites_.push(tuple<double, int, int>(time, id, duration));
}

//DEPRECATED functions
void AvailabilityFilter::run(RecDat* rec_dat){
  update(rec_dat);
}

vector<pair<int,double>>* AvailabilityFilter::get_global_items(){
  return NULL; //&available_items_;
}

