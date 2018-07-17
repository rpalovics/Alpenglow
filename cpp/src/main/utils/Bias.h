#ifndef BIAS
#define BIAS

//SIP_AUTOCONVERT

#include <vector>
#include <iostream>

class Bias{
  public:
    Bias(){};
    ~Bias(){};
    double get(int entity){
      if(entity<(int)biases.size()) return biases[entity];
      else return 0;
    };
    void update(int entity, double val){
      biases[entity]+=val;
    }
    void init(int entity){
      if(entity>=(int)biases.size()) biases.resize(entity+1,0);
    }
    void clear(){
      biases.clear();
    }
  private:
    std::vector <double> biases;
};

#endif
