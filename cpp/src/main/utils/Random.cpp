#include "Random.h"
#include <cmath>
int Random::get_linear(int max){
  step(); return sqrt(state/(mod + 1.0))*max;
}
double Random::get_linear(){
 step(); return sqrt(state/(mod + 1.0));
}

//deprecated
int Random::get_geometric(int length, double prob, int threshold){
  if(length > sample_num.size()) sample_num.resize(length);
  int idx = 0;
  while( ((rand()/(RAND_MAX+1.0)) > prob  or sample_num[length - idx - 1]> threshold) and idx<length) idx++;
  return idx;
}

int Random::get_geometric(double param, int max) {
	step();
	double x = state/(mod + 1.0);
	return log((pow(param,max)-1)*x+1)/log(param); 
}

int Random::get_arctg(double y, int max) {
  return max*get_arctg(y);
}

double Random::get_arctg(double y) {
  step();
  double area = atan(y); //integrate from 0 to y: 1/(1+x^2)
  //distribtuion fn (0:y): atan(x)/area
  //distribution fn (0:1):atan(x*y)/area
  //inverse: tan(x*area)/y
  return tan(state/(mod + 1.0) *area)/y;
}

