#include "Recency.h"

double PowerLawRecency::get(int entity, double actual_time){
  std::map <int,double> :: iterator it = prev_time_.find(entity);
  if(it!=prev_time_.end()){
    double t  = actual_time - it->second;
    double val =  1-pow(1+delta_t_/t,-exponent_);
    return val;
  }
  else return 0; 
}

void PowerLawRecency::update(int entity, double actual_time){
  prev_time_[entity] = actual_time;
}
