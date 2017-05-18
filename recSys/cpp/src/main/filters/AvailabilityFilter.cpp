#include "AvailabilityFilter.h"

void AvailabilityFilter::run(RecDat* rec_dat){
	double time = rec_dat->time;
	while(availability_ends_.size() != 0 && get<0>(availability_ends_.top()) <= time){
		auto item = availability_ends_.top();
		availability_ends_.pop();
		pair<int, double> npair = make_pair(get<1>(item), -1);
		available_items_set_.erase(npair);
	}
	while(availabilites_.size() != 0 && get<0>(availabilites_.top()) <= time){
		auto item = availabilites_.top();
		availabilites_.pop();
		pair<int, double> npair = make_pair(get<1>(item), -1);
		available_items_set_.insert(npair);
		availability_ends_.push(make_pair(get<0>(item)+get<2>(item), get<1>(item)));
	}
	available_items_ = vector<pair<int, double>>(available_items_set_.begin(), available_items_set_.end());
}

vector<pair<int,double>>* AvailabilityFilter::get_global_items(){
	return &available_items_;
}

vector<pair<int,double>>* AvailabilityFilter::get_global_users(){
	return NULL;
}

bool AvailabilityFilter::active(RecDat* r){
	auto available = available_items_set_.find(make_pair(r->item, -1));
	return available != available_items_set_.end();
}

void AvailabilityFilter::add_availability(double time, int id, int duration){
	availabilites_.push(tuple<double, int, int>(time, id, duration));
}