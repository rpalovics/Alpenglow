#include "AvailabilityFilter.h"

bool AvailabilityFilter::active(RecDat* r){
  if(experiment_environment_->get_time()!=time_) update();
  auto available = available_items_set_.find(r->item);
  return available != available_items_set_.end();
}

vector<int> AvailabilityFilter::get_whitelist(int user){
  //NOTE: the set of available items does not depend on the user
  if(experiment_environment_->get_time()!=time_) update();
  return available_items_;
}

void AvailabilityFilter::add_availability(double time, int id, int duration){
  availabilites_.push(tuple<double, int, int>(time, id, duration));
}

void AvailabilityFilter::update(){
  time_=experiment_environment_->get_time();
  while(availability_ends_.size() != 0 && get<0>(availability_ends_.top()) <= time_){
    auto item = availability_ends_.top();
    availability_ends_.pop();
    available_items_set_.erase(get<1>(item));
  }
  while(availabilites_.size() != 0 && get<0>(availabilites_.top()) <= time_){
    auto item = availabilites_.top();
    availabilites_.pop();
    available_items_set_.insert(get<1>(item));
    availability_ends_.push(make_pair(get<0>(item)+get<2>(item), get<1>(item)));
  }
  available_items_ = vector<int>(available_items_set_.begin(), available_items_set_.end());
}
