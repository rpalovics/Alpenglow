#include "Factors.h"

//void Factors::print(int idx){
//  std::cerr << factors[idx]->at(0) << " " << factors[idx]->at(1) << " " << factors[idx]->at(2) << std::endl;  
//}
void Factors::set_parameters(FactorsParameters parameters){
  begin_min=parameters.begin_min;
  begin_max=parameters.begin_max;
  dimension=parameters.dimension;
}

void Factors::resize(int idx){
  if((int)factors.size()<idx+1){
    factors.resize(idx+1,NULL);
  }
}

void Factors::setZero(int idx){
  if(factors[idx]!=NULL) delete factors[idx];
  factors[idx]=new vector <double> (dimension,0);
}

void Factors::setRand(int idx){
  if(factors[idx]!=NULL) delete factors[idx];
  factors[idx] = new vector <double> (dimension,0);
  for(int ii=0; ii<dimension; ii++){
    (*factors[idx])[ii] = ((double)rand()/(double)RAND_MAX)*(begin_max-begin_min)+begin_min;
  } 
}
void Factors::init(int idx){
  resize(idx);
  if(factors[idx]==NULL){
    setRand(idx);
  } 
}

void Factors::initZero(int idx){
  resize(idx);
  if(factors[idx]==NULL){
    setZero(idx);
  }
}

void Factors::initRand(int idx){
  resize(idx);
  if(factors[idx]==NULL){
    setRand(idx);
  }
}

vector <double> *  Factors::get(int idx){
  if (idx < (int) factors.size() ) {
		return factors[idx];
	}
  else return NULL;
}

void Factors::set(int idx, vector <double> * value){
  resize(idx);
  setZero(idx);
  (*factors[idx])=*value;
}

void Factors::add(int idx, vector <double> * value){
  if(factors[idx]==NULL) init(idx);
  for(int ii=0; ii<dimension; ii++){
    (*factors[idx])[ii]+=(*value)[ii];
  }
}

void Factors::add(int idx, vector <double> * value, double coef){
  if(factors[idx]==NULL) init(idx);
  for(int ii=0; ii<dimension; ii++){
    (*factors[idx])[ii]+=(*value)[ii]*coef;
  }
}

void Factors::add(int idx, int dim, double val){
  (*factors[idx])[dim]+=val;
}

void Factors::set(int idx, int dim, double val){
  (*factors[idx])[dim]=val;
}

int Factors::getSize() {
  return factors.size();
}

void Factors::write(ofstream& file) {
  //count vectors
  int counter = 0;
  for(uint i=0;i<factors.size();i++){
    if(factors[i]!=NULL) counter++;
  }
  //write number of vectors
  file.write(reinterpret_cast<char*>(&counter), sizeof counter);
  //write vectors
  for(uint i=0;i<factors.size();i++){
    if(factors[i]!=NULL){
      file.write(reinterpret_cast<char*>(&i),sizeof i);
      for(int j=0;j<dimension;j++){
        file.write(reinterpret_cast<char*>(&(factors[i]->at(j))),sizeof factors[i]->at(j));
      }
    }
  } 
}

void Factors::read(ifstream& file){
  if (file.fail()){
    cerr << "Factors::read file error, did not read anything." << endl;
    return;
  }
  int counter;
  file.read(reinterpret_cast<char*>(&counter), sizeof counter);
  cerr << "Reading in factors for " << counter << " entities." << endl;

  for(int i=0;i<counter;i++){
    int index;
    file.read(reinterpret_cast<char*>(&index), sizeof index);
    vector<double> factor;
    for(int j=0;j<dimension;j++){
      double d;
      file.read(reinterpret_cast<char*>(&d), sizeof d);
      factor.push_back(d);
    }
    set(index,&factor); 
  }
}

void Factors::clear(){
  for (vector<double>* factor : factors){
    if (factor != NULL) {
      delete factor;
    }
  }
  factors.resize(0);
}

vector <int> Factors::getNonnullIndices() {
  vector <int> indices;
  for (size_t ii = 0; ii < factors.size(); ++ii){
    if (factors[ii] != NULL) {
      indices.push_back(ii);
    }
  }
  return indices;
}

void Factors::allToZero() {
  for (size_t ii = 0; ii < factors.size(); ++ii){
    if (factors[ii] != NULL) {
      setZero(ii);
    }
  }
}

